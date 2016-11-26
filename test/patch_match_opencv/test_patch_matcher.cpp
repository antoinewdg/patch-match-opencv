#include "catch.hpp"

#include "iostream"
#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/adapters.h"
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


TEST_CASE("PatchMatcher compared to exhaustive search") {
    typedef EuclidianSquaredDistance<5, uchar, int, 3> DistanceType;
    typedef PatchMatcher<WholeImagePatches, WholeImagePatches, DistanceType,
            OffsetMap2D, DistanceMap2d<int>> MatcherType;

    auto source = load_test_asset("patch_matcher/lena_32_a.png");
    auto target = load_test_asset("patch_matcher/lena_32_b.png");
    WholeImagePatches s_server(source.size(), 5);
    WholeImagePatches t_server(target.size(), 5);
    DistanceType distance(source, target);

    OffsetMap2D offset_map(source.size());
    DistanceMap2d<int> distance_map(target.size());
    MatcherType matcher(s_server, t_server, distance,
                        offset_map, distance_map, 7854);

    Mat_<Vec2i> exhaustive_offsets(source.size());
    Mat_<int> exhaustive_distances(source.size(), std::numeric_limits<int>::max());

    int max_d = -1;
    for (auto p : s_server) {
        bool found = false;
        for (auto q : t_server) {
            int d = distance(p, q);
            if (d < exhaustive_distances(p)) {
                found = true;
                exhaustive_offsets(p) = q - p;
                exhaustive_distances(p) = d;
            }
            if (d > max_d) {
                max_d = d;
            }
        }
    }

    cv::Rect r(2, 2, 28, 28);
    matcher.initialize_offset_map_randomly();


    auto diff = exhaustive_distances(r) - distance_map.to_mat()(r);
    auto initial_error = cv::norm(diff, cv::NORM_L2) / (28 * 28);

    matcher.iterate_n_times(5);
    diff = exhaustive_distances(r) - distance_map.to_mat()(r);
    auto new_error = cv::norm(diff, cv::NORM_L2) / (28 * 28);

    /*
     * This is an ad-hoc result, may be completely stupid.
     */
    REQUIRE((initial_error / new_error) > 40);
}
