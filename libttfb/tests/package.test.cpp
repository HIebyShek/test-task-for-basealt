#include <doctest/doctest.h>

#include <libttfb/package.h>

#include <json-c/json.h>

#include <cstring>

TEST_CASE("package_from_json")
{
    SUBCASE("Valid JSON object")
    {
        package pkg;

        json_object* obj = json_object_new_object();
        json_object_object_add(obj, "name", json_object_new_string("foo"));
        json_object_object_add(obj, "epoch", json_object_new_int(1));
        json_object_object_add(obj, "version", json_object_new_string("1.2.3"));
        json_object_object_add(obj, "release", json_object_new_string("1"));
        json_object_object_add(obj, "arch", json_object_new_string("x86_64"));
        json_object_object_add(obj, "disttag", json_object_new_string("stable"));
        json_object_object_add(obj, "buildtime", json_object_new_int(1658012800));
        json_object_object_add(
            obj,
            "source",
            json_object_new_string("https://example.com/foo-1.2.3.tar.gz"));

        CHECK(package_init_from_json(&pkg, obj) == 0);
        CHECK_EQ(strcmp(pkg.name, "foo"), 0);
        CHECK_EQ(pkg.epoch, 1);
        CHECK_EQ(pkg.ver.major, 1);
        CHECK_EQ(pkg.ver.minor, 2);
        CHECK_EQ(pkg.ver.patch, 3);
        CHECK_EQ(strcmp(pkg.release, "1"), 0);
        CHECK_EQ(strcmp(pkg.arch, "x86_64"), 0);
        CHECK_EQ(strcmp(pkg.disttag, "stable"), 0);
        CHECK_EQ(pkg.buildtime, 1658012800);
        CHECK_EQ(strcmp(pkg.source, "https://example.com/foo-1.2.3.tar.gz"), 0);

        json_object_put(obj);
    }

    SUBCASE("Missing \"epoch\" key")
    {
        package pkg;

        json_object* obj = json_object_new_object();
        json_object_object_add(obj, "name", json_object_new_string("foo"));
        json_object_object_add(obj, "version", json_object_new_string("1.2.3"));
        json_object_object_add(obj, "release", json_object_new_string("1"));
        json_object_object_add(obj, "arch", json_object_new_string("x86_64"));
        json_object_object_add(obj, "disttag", json_object_new_string("stable"));
        json_object_object_add(obj, "buildtime", json_object_new_int(1658012800));
        json_object_object_add(
            obj,
            "source",
            json_object_new_string("https://example.com/foo-1.2.3.tar.gz"));

        CHECK(package_init_from_json(&pkg, obj) == -1);

        json_object_put(obj);
    }

    SUBCASE("Invalid \"epoch\" value")
    {
        package pkg;

        json_object* obj = json_object_new_object();
        json_object_object_add(obj, "name", json_object_new_string("foo"));
        json_object_object_add(obj, "epoch", json_object_new_string("1"));
        json_object_object_add(obj, "version", json_object_new_string("1.2.3"));
        json_object_object_add(obj, "release", json_object_new_string("1"));
        json_object_object_add(obj, "arch", json_object_new_string("x86_64"));
        json_object_object_add(obj, "disttag", json_object_new_string("stable"));
        json_object_object_add(obj, "buildtime", json_object_new_int(1658012800));
        json_object_object_add(
            obj,
            "source",
            json_object_new_string("https://example.com/foo-1.2.3.tar.gz"));

        CHECK(package_init_from_json(&pkg, obj) == -1);

        json_object_put(obj);
    }

    SUBCASE("Missing \"version\" key")
    {
        package pkg;

        json_object* obj = json_object_new_object();
        json_object_object_add(obj, "name", json_object_new_string("foo"));
        json_object_object_add(obj, "epoch", json_object_new_int(1));
        json_object_object_add(obj, "release", json_object_new_string("1"));
        json_object_object_add(obj, "arch", json_object_new_string("x86_64"));
        json_object_object_add(obj, "disttag", json_object_new_string("stable"));
        json_object_object_add(obj, "buildtime", json_object_new_int(1658012800));
        json_object_object_add(
            obj,
            "source",
            json_object_new_string("https://example.com/foo-1.2.3.tar.gz"));

        CHECK(package_init_from_json(&pkg, obj) == -1);

        json_object_put(obj);
    }

    SUBCASE("Invalid \"version\" value")
    {
        package pkg;

        json_object* obj = json_object_new_object();
        json_object_object_add(obj, "name", json_object_new_string("foo"));
        json_object_object_add(obj, "epoch", json_object_new_int(1));
        json_object_object_add(obj, "version", json_object_new_int(1));
        json_object_object_add(obj, "release", json_object_new_string("1"));
        json_object_object_add(obj, "arch", json_object_new_string("x86_64"));
        json_object_object_add(obj, "disttag", json_object_new_string("stable"));
        json_object_object_add(obj, "buildtime", json_object_new_int(1658012800));
        json_object_object_add(
            obj,
            "source",
            json_object_new_string("https://example.com/foo-1.2.3.tar.gz"));

        CHECK(package_init_from_json(&pkg, obj) == -1);

        json_object_put(obj);
    }
}