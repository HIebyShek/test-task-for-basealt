#include <linkhash.h>
#include <ttfb/method.h>

#include "macro.h"

#include <libttfb/package.h>
#include <libttfb/version.h>

#include <json_object.h>
#include <json_types.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int
cmp_name_arch(json_object** lhs, json_object** rhs)
{
    json_object const* const l = *lhs;
    json_object const* const r = *rhs;
    assert(l);
    assert(r);
    assert(json_object_is_type(l, json_type_object));
    assert(json_object_is_type(r, json_type_object));

    package lpkg = package_new();
    int errc = package_init_from_json(&lpkg, l);
    assert(0 == errc);

    package rpkg = package_new();
    errc = package_init_from_json(&rpkg, r);
    assert(0 == errc);

    int cmp_names = strcmp(lpkg.name, rpkg.name);
    if (0 != cmp_names)
    {
        return cmp_names;
    }

    return strcmp(lpkg.arch, rpkg.arch);
}

int
packagesets_difference(ttfb_branch const lhs, ttfb_branch const rhs)
{
    int ret = EXIT_SUCCESS;

    json_object* lhs_set = ttfb_get_binary_packages(lhs);
    if (NULL == lhs_set)
    {
        LOG("error in ttfb_get_binary_packages()");
        return EXIT_FAILURE;
    }

    json_object* rhs_set = ttfb_get_binary_packages(rhs);
    if (NULL == rhs_set)
    {
        LOG("error in ttfb_get_binary_packages()");
        ret = EXIT_FAILURE;
        goto cleanup__lhs_set;
    }

    json_object* diff = ttfb_set_difference(lhs_set, rhs_set, cmp_name_arch);
    ret = json_object_to_fd(fileno(stdout), diff, 0);
    if (0 != ret)
    {
        LOG("%s", json_util_get_last_err());
        goto cleanup;
    }

cleanup:
    json_object_put(diff);
    json_object_put(rhs_set);

cleanup__lhs_set:
    json_object_put(lhs_set);

    return ret;
}

int
cmp_name_arch_version(json_object** lhs, json_object** rhs)
{
    json_object const* const l = *lhs;
    json_object const* const r = *rhs;
    assert(json_object_is_type(l, json_type_object));
    assert(json_object_is_type(r, json_type_object));

    package lpkg = package_new();
    assert(0 == package_init_from_json(&lpkg, l));

    package rpkg = package_new();
    assert(0 == package_init_from_json(&rpkg, r));

    int cmp_names = strcmp(lpkg.name, rpkg.name);
    int cmp_arch = strcmp(lpkg.arch, rpkg.arch);
    int cmp_ver = version_cmp(lpkg.ver, rpkg.ver);

    if (0 != cmp_names)
    {
        return cmp_names;
    }
    if (0 != cmp_arch)
    {
        return cmp_arch;
    }
    if (cmp_ver > 0)
    {
        return cmp_ver;
    }

    return 0;
}

int
packagesets_difference_by_version(ttfb_branch const lhs, ttfb_branch const rhs)
{
    int ret = EXIT_SUCCESS;

    json_object* lhs_set = ttfb_get_binary_packages(lhs);
    if (NULL == lhs_set)
    {
        LOG("error in ttfb_get_binary_packages()");
        return EXIT_FAILURE;
    }

    json_object* rhs_set = ttfb_get_binary_packages(rhs);
    if (NULL == rhs_set)
    {
        LOG("error in ttfb_get_binary_packages()");
        ret = EXIT_FAILURE;
        goto cleanup__lhs_set;
    }

    json_object* diff = ttfb_set_difference(lhs_set, rhs_set, cmp_name_arch_version);
    ret = json_object_to_fd(fileno(stdout), diff, 0);
    if (0 != ret)
    {
        LOG("%s", json_util_get_last_err());
        goto cleanup;
    }

cleanup:
    json_object_put(diff);
    json_object_put(rhs_set);

cleanup__lhs_set:
    json_object_put(lhs_set);

    return ret;
}
