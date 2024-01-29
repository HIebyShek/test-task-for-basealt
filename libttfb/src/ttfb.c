#include <complex.h>
#include <libttfb/ttfb.h>

#include "macro.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/printbuf.h>

#include <assert.h>
#include <string.h>

size_t
WRITEFUNCTION_callback(void* ptr, size_t size, size_t nmemb, void* s)
{
    printbuf* const buf = s;
    printbuf_memappend_fast(buf, ptr, (ssize_t)(size * nmemb));
    return size * nmemb;
}

const char*
ttfb_branch_to_cstr(const ttfb_branch branch)
{
    switch (branch)
    {
    case TTFB_BRANCH_sisyphus:
        return "sisyphus";
    case TTFB_BRANCH_p10:
        return "p10";
    case TTFB_BRANCH_p9:
        return "p9";

    default:
        assert(0);
    }
    // suppress warning
    return NULL;
}

ttfb_branch
ttfb_branch_from_cstr(char const* const branch)
{
    static char const* const p10 = "p10";
    static char const* const p9 = "p9";
    static char const* const sisyphus = "sisyphus";
    if (0 == strncmp(p10, branch, strlen(p10)))
    {
        return TTFB_BRANCH_p10;
    }
    else if (0 == strncmp(p9, branch, strlen(p9)))
    {
        return TTFB_BRANCH_p9;
    }
    else if (0 == strncmp(sisyphus, branch, strlen(sisyphus)))
    {
        return TTFB_BRANCH_sisyphus;
    }

    return TTFB_BRANCH_UNKNOWN;
}

printbuf*
make_url_from_branch(const ttfb_branch branch)
{
    printbuf* buf = printbuf_new();
    printbuf_strappend(buf,
                       "https://rdb.altlinux.org/api/export/branch_binary_packages/");
    char const* const branch_cstr = ttfb_branch_to_cstr(branch);
    if (NULL == branch_cstr)
    {
        LOG("error mapping branch to char* . Such brunch dont exist or programmer forgot "
            "add char* mapping for it. Enable warnings! Stupid.");
        goto error_cleanup;
    }
    printbuf_memappend_fast(buf, branch_cstr, (ssize_t)strlen(branch_cstr));

    goto cleanup;

error_cleanup:
    printbuf_free(buf);
    buf = NULL;

cleanup:
    return buf;
}

int
is_error_response_code(const uint code)
{
    return code >= 400 || code < 100 ? 1 : 0;
}

printbuf const*
make_GET_request(char const* const url)
{
    CURL* const curl = curl_easy_init();
    if (NULL == curl)
    {
        LOG("error in curl_easy_init()");
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    printbuf* const data = printbuf_new();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WRITEFUNCTION_callback);

    CURLcode errc = curl_easy_perform(curl);
    if (CURLE_OK != errc)
    {
        LOG("error sending request: %s", curl_easy_strerror(errc));
        goto error_cleanup;
    }

    long status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    if (is_error_response_code(status_code))
    {
        LOG("Client or server error. Response status code: %ld\n"
            "Response data: %s",
            status_code,
            data->buf);
        goto error_cleanup;
    }

    goto cleanup;

error_cleanup:
    printbuf_free(data);

cleanup:
    curl_easy_cleanup(curl);

    return data;
}

json_object*
ttfb_get_binary_packages(const ttfb_branch branch)
{
    json_object* ret = NULL;

    printbuf const* const url = make_url_from_branch(branch);
    if (NULL == url)
    {
        LOG("error making altlinux api url from branch: %d", branch);
        return NULL;
    }
    printbuf const* const data = make_GET_request(url->buf);
    if (NULL == data)
    {
        LOG("error making get request to url: %s", url->buf);
        goto cleanup__url;
    }
    json_object* const response = json_tokener_parse((char const* const)data->buf);
    assert(json_object_is_type(response, json_type_object));

    ret = json_object_get(json_object_object_get(response, "packages"));
    assert(json_object_is_type(ret, json_type_array));

    json_object_put(response);

    // suppress -Wincompatible-pointer-types-discards-qualifiers.
    // Else how I should deleting constants?
    printbuf_free((printbuf*)data);

cleanup__url:
    printbuf_free((printbuf*)url);

    return ret;
}

json_object*
ttfb_set_difference(json_object* const lhs,
                    json_object* const rhs,
                    ttfb_json_object_compare const cmp)
{
    assert(lhs);
    assert(rhs);
    assert(cmp);
    assert(json_object_is_type(lhs, json_type_array));
    assert(json_object_is_type(rhs, json_type_array));

    typedef int (*internal_json_cmp)(void const*, void const*);
    json_object_array_sort(lhs, (internal_json_cmp)cmp);
    json_object_array_sort(rhs, (internal_json_cmp)cmp);

    size_t ret_max_len = json_object_array_length(lhs);
    json_object* ret = json_object_new_array_ext(ret_max_len);

    size_t i = 0;
    size_t j = 0;

    size_t iend = json_object_array_length(lhs);
    size_t jend = json_object_array_length(rhs);

    json_object* lval = NULL;
    json_object* rval = NULL;
    while (i < iend && j < jend)
    {
        lval = json_object_array_get_idx(lhs, i);
        rval = json_object_array_get_idx(rhs, j);

        if (cmp(&lval, &rval) > 0)
        {
            ++j;
            continue;
        }

        if (0 == cmp(&lval, &rval))
        {
            ++i;
            ++j;
            continue;
        }

        // may be need to handle error, but it is not documented
        json_object_array_add(ret, json_object_get(lval));
        ++i;
    }

    while (i < iend)
    {
        lval = json_object_array_get_idx(lhs, i);

        // may be need to handle error, but it is not documented
        json_object_array_add(ret, json_object_get(lval));
        ++i;
    }

    size_t ret_len = json_object_array_length(ret);
    json_object_array_shrink(ret, (ssize_t)ret_max_len - ret_len);

    return ret;
}
