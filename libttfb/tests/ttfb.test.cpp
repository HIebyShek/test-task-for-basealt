#include <doctest/doctest.h>

#include <ttfb/ttfb.h>

TEST_CASE("ttfb_get_binary_packages")
{
    REQUIRE(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_sisyphus)));
    REQUIRE(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_p10)));
    REQUIRE(json_object_put(ttfb_get_binary_packages(TTFB_BRANCH_p9)));

    ttfb_branch r = static_cast<ttfb_branch>(rand());
    REQUIRE(r != TTFB_BRANCH_p10);
    REQUIRE(r != TTFB_BRANCH_p9);
    REQUIRE(r != TTFB_BRANCH_sisyphus);
    REQUIRE_EQ(nullptr, ttfb_get_binary_packages(r));
}