////
//// Created by antoinewdg on 11/23/16.
////
//
//#ifndef PATCH_MATCH_PATCH_MATCHER_UTILS_H
//#define PATCH_MATCH_PATCH_MATCHER_UTILS_H
//
//#define PI 3.14159265
//
//#include <array>
//#include <random>
//
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//#include "patch_matcher.h"
//
//namespace pm {
//
//    using cv::Vec2i;
//    using cv::Vec3b;
//    using cv::Mat_;
//    using std::array;
//
//    template<class SourcePatches, class TargetPatches, class DistanceFunction>
//    class PatchMatcherUtils {
//    public:
//
//        typedef PatchMatcher<SourcePatches, TargetPatches, DistanceFunction> matcher_type;
//
//        /**
//         * Transform the offset_map to a HSV image
//         *
//         *  - hue represents the orientation of the offset
//         *  - saturation is always 255
//         *  - value is the norm of the offset
//         *
//         * @param matcher
//         * @return
//         */
//        static Mat_<Vec3b> offset_map_as_hsv(const matcher_type &matcher) {
//            const Mat_<Vec2i> &offset_map = matcher.offset_map;
//            Mat_<Vec3b> hsv_values(offset_map.rows, offset_map.cols, Vec3b(0, 0, 125));
//            double max_norm = cv::norm(Vec2i(offset_map.rows, offset_map.cols), cv::NORM_L2);
//
//            for (const Vec2i &p : matcher.s) {
//                int di = offset_map(p)[0];
//                int dj = offset_map(p)[1];
//                if (di == 0 && dj == 0) {
//                    hsv_values(p[0], p[1]) = Vec3b(0, 0, 0);
//                    continue;
//                }
//                uchar hue = uchar((std::atan2(di, dj) + PI) * 90 / PI);
//
//                uchar value = uchar(255.0 * cv::norm(offset_map(p), cv::NORM_L2) / max_norm);
//                hsv_values(p[0], p[1]) = Vec3b(hue, 255, value);
//
//            }
//            cv::cvtColor(hsv_values, hsv_values, CV_HSV2BGR);
//
//            return hsv_values;
//        }
//
//    };
//}
//
//
//#endif //PATCH_MATCH_PATCH_MATCHER_UTILS_H
