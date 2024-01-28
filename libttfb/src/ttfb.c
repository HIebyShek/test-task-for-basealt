#include <json_types.h>
#include <linkhash.h>
#include <ttfb/ttfb.h>

#include "macro.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/printbuf.h>

#include <assert.h>
#include <string.h>

size_t
WRITEFUNCTION_callback(void* ptr, size_t size, size_t nmemb, void* s)
{
    printbuf* buf = s;
    printbuf_memappend_fast(buf, ptr, size * nmemb);
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
        LOG("branch not found");
        return NULL;
    }
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
    printbuf_memappend_fast(buf, branch_cstr, strlen(branch_cstr));

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

printbuf*
make_GET_request(char const* const url)
{
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        LOG("error in curl_easy_init()");
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    printbuf* data = printbuf_new();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WRITEFUNCTION_callback);

    CURLcode errc = curl_easy_perform(curl);
    if (CURLE_OK != errc)
    {
        LOG("error sending request: %s", curl_easy_strerror(errc));
        goto error_cleanup;
    }

    uint status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    if (is_error_response_code(status_code))
    {
        LOG("Client or server error. Response status code: %d\n"
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
    printbuf* url = make_url_from_branch(branch);
    if (NULL == url)
    {
        LOG("error making altlinux api url from branch: %d", branch);
        return NULL;
    }

    printbuf* data = make_GET_request(url->buf);
    if (NULL == data)
    {
        LOG("error making get request to url: %s", url->buf);
        goto cleanup__url;
    }

    json_object* response = json_tokener_parse(data->buf);
    assert(json_object_is_type(response, json_type_object));

    lh_table* json = json_object_get_object(response);

    json_object* ret =
        json_object_get(lh_entry_v(lh_table_lookup_entry(json, "packages")));
    assert(json_object_is_type(ret, json_type_array));

    // cleanup:
    json_object_put(response);
    printbuf_free(data);

cleanup__url:
    printbuf_free(url);

    return ret;
}

TTFB_EXPORT json_object*
ttfb_set_difference(json_object* const lhs,
                    json_object* const rhs,
                    ttfb_json_object_compare cmp)
{
    assert(lhs);
    assert(rhs);
    assert(cmp);
    assert(json_object_is_type(lhs, json_type_array));
    assert(json_object_is_type(rhs, json_type_array));

    json_object_array_sort(lhs, cmp);
    json_object_array_sort(rhs, cmp);

    size_t ret_max_len = json_object_array_length(lhs);
    json_object* ret = json_object_new_array_ext(ret_max_len);

    struct json_object_iterator lit = json_object_iter_begin(lhs);
    struct json_object_iterator rit = json_object_iter_begin(rhs);
    struct json_object_iterator litend = json_object_iter_end(lhs);
    struct json_object_iterator ritend = json_object_iter_end(rhs);

    json_object* lval = NULL;
    json_object* rval = NULL;
    while (!json_object_iter_equal(&lit, &litend) &&
           !json_object_iter_equal(&rit, &ritend))
    {
        lval = json_object_iter_peek_value(&lit);
        rval = json_object_iter_peek_value(&rit);
        if (TTFB_COMPARE_RESULT_EQUAL == cmp(lval, rval))
        {
            json_object_iter_next(&rit);
        }

        // may be need to handle error, but it is not documented
        json_object_array_add(ret, lval);
        json_object_iter_next(&lit);
        json_object_iter_next(&rit);
    }

    while (!json_object_iter_equal(&lit, &litend))
    {
        lval = json_object_iter_peek_value(&lit);

        // may be need to handle error, but it is not documented
        json_object_array_add(ret, lval);
        json_object_iter_next(&lit);
    }

    size_t ret_len = json_object_array_length(ret);
    json_object_array_shrink(ret, (ssize_t)ret_max_len - ret_len);

    return ret;
}
