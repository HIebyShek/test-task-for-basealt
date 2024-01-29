#include <doctest/doctest.h>

#include <libttfb/version.h>

TEST_CASE("version_new_from_cstr")
{

    SUBCASE("1.2.3")
    {
        version ver;
        CHECK(version_new_from_cstr(&ver, "1.2.3") == 0);
        CHECK(ver.major == 1);
        CHECK(ver.minor == 2);
        CHECK(ver.patch == 3);
    }

    SUBCASE("1.2.3")
    {
        version ver;
        CHECK(version_new_from_cstr(&ver, "4.5") == 0);
        CHECK(ver.major == 4);
        CHECK(ver.minor == 5);
        CHECK(ver.patch == 0);
    }

    SUBCASE("1.2.3")
    {
        version ver;
        CHECK(version_new_from_cstr(&ver, "6") == 0);
        CHECK(ver.major == 6);
        CHECK(ver.minor == 0);
        CHECK(ver.patch == 0);
    }

    SUBCASE("1.2.3")
    {
        version ver;
        CHECK(version_new_from_cstr(&ver, "7.8.9.10") == -1);
    }
}

TEST_CASE("version_cmp")
{
    SUBCASE("Equal versions")
    {
        version v1 = { 1, 2, 3 };
        version v2 = { 1, 2, 3 };

        CHECK(version_cmp(v1, v2) == 0);
    }

    SUBCASE("First version is less than second version")
    {
        version v1 = { 1, 2, 3 };
        version v3 = { 1, 2, 4 };

        CHECK(version_cmp(v1, v3) < 0);
    }

    SUBCASE("First version is greater than second version")
    {
        version v3 = { 1, 2, 4 };
        version v1 = { 1, 2, 3 };

        CHECK(version_cmp(v3, v1) > 0);
    }

    SUBCASE("Versions with different major numbers")
    {
        version v4 = { 2, 0, 0 };
        version v5 = { 1, 9, 9 };

        CHECK(version_cmp(v4, v5) > 0);
        CHECK(version_cmp(v5, v4) < 0);
    }

    SUBCASE("Versions with different minor numbers")
    {
        version v6 = { 0, 1, 0 };
        version v7 = { 0, 0, 1 };

        CHECK(version_cmp(v6, v7) > 0);
        CHECK(version_cmp(v7, v6) < 0);
    }
}
