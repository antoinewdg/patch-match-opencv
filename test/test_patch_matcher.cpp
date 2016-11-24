#include "catch.hpp"

#include "iostream"
#include "patch_servers/whole_image_patches.h"
#include "patch_matcher.h"
#include "patch_matcher_utils.h"
#include "patch_distances.h"
#include "maps/maps_2d.h"
#include "test_utils.h"

using namespace pm;

using cv::Vec3b;

TEST_CASE("PatchMatcher on simple translation") {

    typedef EuclidianSquaredDistance<5, uchar, int, 3> DistanceType;
    typedef PatchMatcher<WholeImagePatches, WholeImagePatches, DistanceType,
            OffsetMap2D, DistanceMap2d<int>> MatcherType;

    auto lena = load_test_asset("patch_matcher/lena.png");
    auto lena_offset_10 = load_test_asset("patch_matcher/lena_offset_10.png");
    WholeImagePatches patches(lena.size(), 5);
    WholeImagePatches patches_10(lena_offset_10.size(), 5);
    DistanceType distance(lena, lena_offset_10);


    SECTION("Converges in only one iteration if correct offset is manually set") {
        MatcherType matcher(patches, patches_10, distance, 32);
        matcher.initialize_offset_map_randomly();
        matcher.set_offset(Vec2i(2, 2), Vec2i(0, 10));
        matcher.iterate_n_times(1);

        Mat_<Vec2i> offset_map = matcher.get_offset_map().to_mat()(cv::Rect(2, 2, 508, 508));
        Mat_<Vec2i> expected(508, 508, Vec2i(0, 10));
        Mat_<int> expected_distances(508, 508, 0);

        require_matrix_equal(matcher.get_distance_map().to_mat()(cv::Rect(2, 2, 508, 508)), expected_distances);
        require_matrix_equal(offset_map, expected);

    }

    SECTION("Converges in 5 iterations with random initialization") {
        MatcherType matcher(patches, patches_10, distance, 32645);
        matcher.initialize_offset_map_randomly();
        matcher.iterate_n_times(5);

        Mat_<Vec2i> offset_map = matcher.get_offset_map().to_mat()(cv::Rect(2, 2, 508, 508));
        Mat_<Vec2i> expected(508, 508, Vec2i(0, 10));
        Mat_<int> expected_distances(508, 508, 0);

        require_matrix_equal(matcher.get_distance_map().to_mat()(cv::Rect(2, 2, 508, 508)), expected_distances);
        require_matrix_equal(offset_map, expected);


    }

}