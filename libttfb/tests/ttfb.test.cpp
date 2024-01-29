#include "libttfb/version.h"
#include <doctest/doctest.h>

#include <json_types.h>
#include <libttfb/ttfb.h>

#include <json_object.h>
#include <json_tokener.h>

#include <assert.h>
#include <cstring>
#include <string_view>

TEST_CASE("ttfb_get_binary_packages")
{
    CHECK(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_sisyphus)));
    CHECK(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_p10)));
    CHECK(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_p9)));
}

extern "C" int
cmp(void* lhs, void* rhs)
{
    json_object const* const l = *(json_object**)lhs;
    json_object const* const r = *(json_object**)rhs;
    assert(json_object_is_type(l, json_type_object));
    assert(json_object_is_type(r, json_type_object));

    char const* const lname = json_object_get_string(json_object_object_get(l, "name"));
    char const* const rname = json_object_get_string(json_object_object_get(r, "name"));

    return strcmp(lname, rname);
}

TEST_CASE("ttfb_set_difference cmp")
{
    json_object* _1 = json_tokener_parse("[{\"name\":\"a\"}]");
    json_object* _2 = json_tokener_parse("[{\"name\":\"b\"}]");
    json_object* empty = json_tokener_parse("[]");

    SUBCASE("_1 - _2")
    {
        json_object* ret = ttfb_set_difference(_1, _2, (ttfb_json_object_compare)cmp);
        CHECK(json_object_equal(_1, ret));
        json_object_put(ret);
    }

    SUBCASE("_1 - _1")
    {
        json_object* ret = ttfb_set_difference(_1, _1, (ttfb_json_object_compare)cmp);
        CHECK(json_object_equal(empty, ret));
        json_object_put(ret);
    }

    SUBCASE("_1 - empty")
    {
        json_object* ret = ttfb_set_difference(_1, empty, (ttfb_json_object_compare)cmp);
        CHECK(json_object_equal(_1, ret));
        json_object_put(ret);
    }

    SUBCASE("empty - _1")
    {
        json_object* ret = ttfb_set_difference(empty, _1, (ttfb_json_object_compare)cmp);
        CHECK(json_object_equal(empty, ret));
        json_object_put(ret);
    }

    CHECK(json_object_put(_1));
    CHECK(json_object_put(_2));
    CHECK(json_object_put(empty));
}

extern "C" int
cmp_gt_ver(void* lhs, void* rhs)
{
    json_object const* const l = *(json_object**)lhs;
    json_object const* const r = *(json_object**)rhs;
    assert(json_object_is_type(l, json_type_object));
    assert(json_object_is_type(r, json_type_object));

    char const* const lname = json_object_get_string(json_object_object_get(l, "name"));
    char const* const rname = json_object_get_string(json_object_object_get(r, "name"));

    version lver = version_new();
    version rver = version_new();

    if (0 != version_new_from_cstr(
                 &lver, json_object_get_string(json_object_object_get(l, "version"))))
        lver = version_new();
    if (0 != version_new_from_cstr(
                 &rver, json_object_get_string(json_object_object_get(r, "version"))))
        rver = version_new();

    int cmp_names = strcmp(lname, rname);
    int cmp_ver = version_cmp(lver, rver);
    if (0 != cmp_names)
    {
        return cmp_names;
    }
    if (cmp_ver > 0)
    {
        return cmp_ver;
    }

    return 0;
}

TEST_CASE("ttfb_set_difference cmp_gt_ver")
{
    json_object* _1 = json_tokener_parse(
        "[{\"name\":\"a\", \"version\": \"1\"}, {\"name\":\"b\", \"version\": \"0\"}]");
    json_object* _2 = json_tokener_parse(
        "[{\"name\":\"a\", \"version\": \"0\"}, {\"name\":\"b\", \"version\": \"1\"}]");
    json_object* empty = json_tokener_parse("[]");

    SUBCASE("_1 - _2")
    {
        json_object* expected =
            json_tokener_parse("[{\"name\":\"a\", \"version\": \"1\"}]");

        json_object* ret =
            ttfb_set_difference(_1, _2, (ttfb_json_object_compare)cmp_gt_ver);
        CHECK(json_object_equal(expected, ret));

        json_object_put(ret);
        json_object_put(expected);
    }

    SUBCASE("_1 - _1")
    {
        json_object* ret =
            ttfb_set_difference(_1, _1, (ttfb_json_object_compare)cmp_gt_ver);
        CHECK(json_object_equal(empty, ret));
        json_object_put(ret);
    }

    SUBCASE("_1 - empty")
    {
        json_object* ret =
            ttfb_set_difference(_1, empty, (ttfb_json_object_compare)cmp_gt_ver);
        CHECK(json_object_equal(_1, ret));
        json_object_put(ret);
    }

    SUBCASE("empty - _1")
    {
        json_object* ret =
            ttfb_set_difference(empty, _1, (ttfb_json_object_compare)cmp_gt_ver);
        CHECK(json_object_equal(empty, ret));
        json_object_put(ret);
    }

    CHECK(json_object_put(_1));
    CHECK(json_object_put(_2));
    CHECK(json_object_put(empty));
}