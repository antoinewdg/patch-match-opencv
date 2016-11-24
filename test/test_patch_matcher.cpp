#include "catch.hpp"

#include "iostream"
#include "patch_servers/whole_image_patches.h"
#include "patch_matcher.h"
#include "patch_matcher_utils.h"
#include "patch_distances.h"
#include "maps/maps_2d.h"
#include "test_utils.h"

#include "dummies/distance.h"
#include "dummies/maps.h"
#include "dummies/patch_servers.h"

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


TEST_CASE("Dummy PatchMatcher compiles") {
    typedef PatchMatcher<
            dummies::SourcePatches, dummies::TargetPatches,
            dummies::Distance,
            dummies::OffsetMap, dummies::DistanceMap
    > MatcherType;

    dummies::SourcePatches s;
    dummies::TargetPatches t;
    dummies::Distance distance;
    dummies::OffsetMap offset_map;
    dummies::DistanceMap distance_map;

    MatcherType matcher(s, t, distance, offset_map, distance_map);

    matcher.initialize_offset_map_randomly();
    matcher.iterate_n_times(2);
}