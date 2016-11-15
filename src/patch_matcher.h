//
// Created by antoinewdg on 11/15/16.
//

#ifndef PATCH_MATCH_PATCH_MATCHER_H
#define PATCH_MATCH_PATCH_MATCHER_H

#include <array>
#include <random>

#include "opencv2/core/core.hpp"

namespace pm {

    using cv::Vec2i;
    using cv::Mat_;
    using std::array;

    template<class SourcePatches, class TargetPatches, class DistanceFunction>
    class PatchMatcher {
    public:
        PatchMatcher(const SourcePatches &s, const TargetPatches &t,
                     const DistanceFunction &patch_distance, unsigned int seed = 0) :
                s(s), t(t), patch_distance(patch_distance) {
            if (seed == 0) {
                std::random_device r;
                seed = r();
            }
            generator = std::default_random_engine(seed);
        }

        void initialize_offset_map() {
            offset_map = Mat_<Vec2i>(s.size(), Vec2i(0, 0));
            std::uniform_int_distribution<int> dist(0, t.n_patches() - 1);
            for (const Vec2i &p : s) {
                int index = dist(generator);
                Vec2i q = t[index];
                offset_map(p) = q - p;
                distance_map(p) = patch_distance(p, q);
            }
        }


    private:

        int _propagate_distance(const Vec2i &p, const Vec2i &n, int max_d) {

            // Check that neighbor is not out of bounds of nnf
            if (n[0] < P / 2 || n[1] < P / 2
                || n[0] >= offset_map.rows - (P / 2)
                || n[1] >= offset_map.cols - (P / 2)) {
                return std::numeric_limits<int>::max();
            }

            Vec2i q = offset_map(n) + p;
            if (q[0] < 0 || q[1] < 0
                || q[0] >= t.rows
                || q[1] >= t.cols
                || t.contains_patch(q)) {
                return std::numeric_limits<int>::max();
            }

            return patch_distance(p, q, max_d);
        }

        void _propagate(const Vec2i &p, const array<Vec2i, 2> &neighbors) {
            Vec2i q = offset_map(p) + p;
            int &min_d = patch_distance(p, q);
            for (int n = 0; n < neighbors.size(); n++) {
                int d = _propagate_distance(p, neighbors[n], min_d);
                if (d < min_d) {
                    min_d = d;
                    offset_map(p) = offset_map(neighbors[n]);
                }
            }
        }


        DistanceFunction patch_distance;
        SourcePatches s;
        TargetPatches t;
        std::default_random_engine generator;
        Mat_<Vec2i> offset_map;
        Mat_<int> distance_map;
        int P;

    };
}

#endif //PATCH_MATCH_PATCH_MATCHER_H
