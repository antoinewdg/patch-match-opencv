#include "catch.hpp"

#include "patch_servers/whole_image_patches.h"
#include "test_utils.h"

#include <iostream>

using namespace pm;

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

    SECTION("operator[]"){
        REQUIRE(patches[0] == Vec2i(2,2));
        REQUIRE(patches[6] == Vec2i(3,4));
        REQUIRE(patches[23] == Vec2i(7,5));
    }

    SECTION("size"){
        REQUIRE(patches.size() == Size(4, 6));
    }

}
