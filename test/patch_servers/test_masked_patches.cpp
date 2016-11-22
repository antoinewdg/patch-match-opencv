#include <iostream>

#include "catch.hpp"

#include "patch_servers/masked_patches.h"
#include "test_utils.h"

using namespace pm;

using std::string;

Mat_<bool> load_mask(string name) {
    Mat_<bool> im = load_test_asset<bool>("masked_patches/" + name + ".png", cv::IMREAD_GRAYSCALE);
    return im / 255;
}


TEST_CASE("Totally masked patches") {

    Mat_<bool> l_shaped_mask = load_mask("l_shaped_mask");
    Mat_<bool> expected_mask = load_mask("l_shaped_mask_total");
    TotallyMaskedPatches patches(l_shaped_mask, 5);


    SECTION("contains_patch") {
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                REQUIRE(expected_mask(i, j) == patches.contains_patch(Vec2i(i, j)));
            }
        }
    }

    SECTION("iterator") {
        Mat_<bool> constructed_mask(l_shaped_mask.size(), false);
        for (auto p : patches) {
            constructed_mask(p) = true;
        }
        auto are_matrix_equal = true;
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                if (expected_mask(i, j) != constructed_mask(i, j)) {
                    are_matrix_equal = false;
                }
            }
        }
        REQUIRE(are_matrix_equal);

    }

    SECTION("reverse iterator") {
        Mat_<bool> constructed_mask(l_shaped_mask.size(), false);
        auto it = patches.rbegin();
        for ( ; it != patches.rend() ; it++) {
            constructed_mask(*it) = true;
        }
        auto are_matrix_equal = true;
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                if (expected_mask(i, j) != constructed_mask(i, j)) {
                    are_matrix_equal = false;
                }
            }
        }
        REQUIRE(are_matrix_equal);

    }

    SECTION("operator[]") {
        REQUIRE(patches[0] == Vec2i(7, 6));
        REQUIRE(patches[64] == Vec2i(22, 8));
    }

    SECTION("size") {
        REQUIRE(patches.size() == Size(32, 32));
    }

}

TEST_CASE("Partially masked patches") {

    Mat_<bool> l_shaped_mask = load_mask("l_shaped_mask");
    Mat_<bool> expected_mask = load_mask("l_shaped_mask_partial");
    PartiallyMaskedPatches patches(l_shaped_mask, 5);


    SECTION("contains_patch") {
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                REQUIRE(expected_mask(i, j) == patches.contains_patch(Vec2i(i, j)));
            }
        }
    }

    SECTION("iterator") {
        Mat_<bool> constructed_mask(l_shaped_mask.size(), false);
        for (auto p : patches) {
            constructed_mask(p) = true;
        }
        auto are_matrix_equal = true;
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                if (expected_mask(i, j) != constructed_mask(i, j)) {
                    are_matrix_equal = false;
                }
            }
        }
        REQUIRE(are_matrix_equal);

    }

    SECTION("reverse iterator") {
        Mat_<bool> constructed_mask(l_shaped_mask.size(), false);
        auto it = patches.rbegin();
        for ( ; it != patches.rend() ; it++) {
            constructed_mask(*it) = true;
        }
        auto are_matrix_equal = true;
        for (int i = 0; i < expected_mask.rows; i++) {
            for (int j = 0; j < expected_mask.cols; j++) {
                if (expected_mask(i, j) != constructed_mask(i, j)) {
                    are_matrix_equal = false;
                }
            }
        }
        REQUIRE(are_matrix_equal);

    }

    SECTION("operator[]") {
        REQUIRE(patches[0] == Vec2i(3, 2));
        REQUIRE(patches[416] == Vec2i(26, 12));
    }

    SECTION("size") {
        REQUIRE(patches.size() == Size(32, 32));
    }

}