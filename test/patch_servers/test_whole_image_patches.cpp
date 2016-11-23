#include "catch.hpp"

#include "patch_servers/whole_image_patches.h"
#include "test_utils.h"

#include <iostream>

using namespace pm;

using cv::Mat_;

TEST_CASE("WholeImagePatches 8x10 with P=5") {

    WholeImagePatches patches(Size(8, 10), 5);

    SECTION("Forward iteration, contains_patch") {
        auto it = patches.begin();
        for (int i = 2; i < 10 - 2; i++) {
            for (int j = 2; j < 8 - 2; j++) {
                REQUIRE(*it == Vec2i(i, j));
                REQUIRE(patches.contains_patch(Vec2i(i, j)));
                it++;
            }
        }
        REQUIRE(it == patches.end());
    }

    SECTION("Reverse iteration") {
        auto it = patches.rbegin();
        for (int i = 10 - 2 - 1; i >= 2; i--) {
            for (int j = 8 - 2 - 1; j >= 2; j--) {
                REQUIRE(*it == Vec2i(i, j));
                it++;
            }
        }
        REQUIRE(it == patches.rend());
    }

    SECTION("size") {
        REQUIRE(patches.size() == Size(4, 6));
    }

    SECTION("pick_random") {
        std::default_random_engine generator(9978);
        Mat_<int> n_picked(6, 4, 0);
        for (int i = 0; i < 100; i++) {
            Vec2i p = patches.pick_random(generator);
            n_picked(p - Vec2i(2,2))++;
            if (!patches.contains_patch(p)) {
                FAIL("Randomly generated patch " << p << " is not contained");
            }
        }

        // There are at most two unpicked patches after 100 iterations
        REQUIRE(24 -cv::countNonZero(n_picked) < 2);
    }

}
