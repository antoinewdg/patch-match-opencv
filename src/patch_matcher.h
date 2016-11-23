//
// Created by antoinewdg on 11/15/16.
//

#ifndef PATCH_MATCH_PATCH_MATCHER_H
#define PATCH_MATCH_PATCH_MATCHER_H

#include <array>
#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace pm {

    using cv::Vec2i;
    using cv::Vec3b;
    using cv::Mat_;
    using std::array;

    template<class SourcePatches, class TargetPatches, class DistanceFunction>
    class PatchMatcherUtils;

    template<class SourcePatches, class TargetPatches, class DistanceFunction>
    class PatchMatcher {
    public:
        friend class PatchMatcherUtils<SourcePatches, TargetPatches, DistanceFunction>;

        typedef typename DistanceFunction::output_type distance_type;
        typedef PatchMatcherUtils<SourcePatches, TargetPatches, DistanceFunction> utils_type;


        PatchMatcher(const SourcePatches &s, const TargetPatches &t,
                     const DistanceFunction &patch_distance, unsigned int seed = 0) :
                s(s), t(t), patch_distance(patch_distance) {
            if (seed == 0) {
                std::random_device r;
                seed = r();
            }
            generator = std::default_random_engine(seed);
        }

        void initialize_offset_map_randomly() {
            int P = DistanceFunction::patch_size;
            cv::Size size(s.size().width + P - 1, s.size().height + P - 1);
            offset_map = Mat_<Vec2i>(size, Vec2i(0, 0));
            distance_map = Mat_<distance_type>(size, distance_type(0));

            for (const Vec2i &p : s) {
                Vec2i q = t.pick_random(generator);
                offset_map(p) = q - p;
                distance_map(p) = patch_distance(p, q);
            }
        }

        void iterate_n_times(int n) {
            for (int i = 0; i < n; i++) {
                if (i % 2 == 0) {
                    _iterate_rd();
                } else {
                    _iterate_lu();
                }
            }
        }

        inline void set_offset(const Vec2i &p, const Vec2i &offset) {
            offset_map(p) = offset;
            distance_map(p) = patch_distance(p, p + offset);
        }

        const Mat_<Vec2i> &get_offset_map() const {
            return offset_map;
        }


        const Mat_<distance_type> &get_distance_map() const {
            return distance_map;
        }


    private:

        distance_type _propagate_distance(const Vec2i &p, const Vec2i &n, int max_d) {

            if (!s.contains_patch(n)) {
                return std::numeric_limits<distance_type>::max();
            }

            Vec2i q = offset_map(n) + p;
            if (!t.contains_patch(q)) {
                return std::numeric_limits<distance_type>::max();
            }
            return patch_distance(p, q, max_d);
        }

        void _propagate(const Vec2i &p, const array<Vec2i, 2> &neighbors) {
            Vec2i q = offset_map(p) + p;
            int &min_d = distance_map(p);

            for (int n = 0; n < neighbors.size(); n++) {
                int d = _propagate_distance(p, neighbors[n], min_d);
                if (d <= min_d) {
                    min_d = d;
                    offset_map(p) = offset_map(neighbors[n]);
                }
            }
        }

        void _random_search(const Vec2i &p) {
            Vec2i q = (offset_map(p) + p);
            int r = t.get_max_window_size(q);
            int &min_d = distance_map(p);

            while (r > 1) {
                Vec2i new_q = t.pick_random_in_window(generator, q, r);

                if (t.contains_patch(new_q)) {
                    int d = patch_distance(p, new_q);
                    if (d < min_d) {
                        offset_map(p) = new_q - p;
                    }
                }


                r /= 2;
            }
        }

        void _iterate_rd() {
            for (const Vec2i &p : s) {
                _propagate(p, {p - Vec2i(1, 0), p - Vec2i(0, 1)});
                _random_search(p);
            }
        }

        void _iterate_lu() {
            auto it = s.rbegin();
            for (; it != s.rend(); it++) {
                const Vec2i &p = *it;
                _propagate(p, {p + Vec2i(1, 0), p + Vec2i(0, 1)});
                _random_search(p);
            }
        }

        DistanceFunction patch_distance;
        SourcePatches s;
        TargetPatches t;
        std::default_random_engine generator;
        Mat_<distance_type> distance_map;
        Mat_<Vec2i> offset_map;

    };
}

#endif //PATCH_MATCH_PATCH_MATCHER_H
