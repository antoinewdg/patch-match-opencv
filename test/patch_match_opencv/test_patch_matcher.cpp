#include "catch.hpp"

#include "iostream"
#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/patch_servers/whole_image_patches.h"
#include "patch_match_opencv/patch_distances.h"
#include "patch_match_opencv/maps/maps_2d.h"
#include "test_utils.h"


using namespace pm::core;
using namespace pm::opencv;

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
        OffsetMap2D offset_map(patches.size());
        DistanceMap2d<int> distance_map(patches.size());
        MatcherType matcher(patches, patches_10, distance,
                            offset_map, distance_map, 32);

        matcher.initialize_offset_map_randomly();
        matcher.set_offset(Vec2i(2, 2), Vec2i(0, 10));
        matcher.iterate_n_times(1);


        Mat_<Vec2i> expected_offsets(508, 508, Vec2i(0, 10));
        Mat_<int> expected_distances(508, 508, 0);

        Rect r(2, 2, 508, 508);
        require_matrix_equal(distance_map.to_mat()(r), expected_distances);
        require_matrix_equal(offset_map.to_mat()(r), expected_offsets);

    }

    SECTION("Converges in 5 iterations with random initialization") {

        OffsetMap2D offset_map(patches.size());
        DistanceMap2d<int> distance_map(patches.size());
        MatcherType matcher(patches, patches_10, distance,
                            offset_map, distance_map, 3245);

        matcher.initialize_offset_map_randomly();
        matcher.iterate_n_times(5);


        Mat_<Vec2i> expected_offsets(508, 508, Vec2i(0, 10));
        Mat_<int> expected_distances(508, 508, 0);

        Rect r(2, 2, 508, 508);
        require_matrix_equal(distance_map.to_mat()(r), expected_distances);
        require_matrix_equal(offset_map.to_mat()(r), expected_offsets);


    }

}

