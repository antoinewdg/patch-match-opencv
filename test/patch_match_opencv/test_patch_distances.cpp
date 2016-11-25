#include "catch.hpp"

#include "patch_match_opencv/patch_distances.h"
#include "test_utils.h"

using namespace pm::opencv;

using cv::Vec3b;

TEST_CASE("Euclidian squared distance Vec3b, int, P=5") {

    typedef EuclidianSquaredDistance<5, uchar, int, 3> DistanceType;
    Mat_<Vec3b> im = load_test_asset("patch_distances/pink_orange.png");

    DistanceType distance(im, im);

    REQUIRE(distance(Vec2i(2, 2), Vec2i(2, 2)) == 0);
    REQUIRE(distance(Vec2i(2, 2), Vec2i(2, 4)) == 0);
    REQUIRE(distance(Vec2i(2, 2), Vec2i(2, 7)) == 0);

    REQUIRE(distance(Vec2i(2,2), Vec2i(3, 2)) == 423125);
    REQUIRE(distance(Vec2i(2,2), Vec2i(3, 4)) == 423125);
    REQUIRE(distance(Vec2i(2,2), Vec2i(3, 7)) == 423125);

    REQUIRE(distance(Vec2i(2,2), Vec2i(7, 2)) == 2115625);
    REQUIRE(distance(Vec2i(2,2), Vec2i(7, 4)) == 2115625);
    REQUIRE(distance(Vec2i(2,2), Vec2i(7, 7)) == 2115625);

    REQUIRE(distance(Vec2i(5,10), Vec2i(7,12)) == 729600);



}