//
// Created by antoinewdg on 11/15/16.
//

#ifndef PATCH_MATCH_PATCH_MATCHER_H
#define PATCH_MATCH_PATCH_MATCHER_H

#include <array>
#include <random>

namespace pm {

    using cv::Vec2i;
    using cv::Vec3b;
    using cv::Mat_;
    using std::array;

    template<class SourcePatches, class TargetPatches,
            class DistanceFunction>
    class PatchMatcherUtils;

    template<
            class SourcePatches, class TargetPatches,
            class DistanceFunction,
            class OffsetMap, class DistanceMap
    >
    class PatchMatcher {
    public:
        friend class PatchMatcherUtils<SourcePatches, TargetPatches, DistanceFunction>;

        typedef typename DistanceFunction::output_type distance_type;
        typedef PatchMatcherUtils<SourcePatches, TargetPatches, DistanceFunction> utils_type;
        typedef typename SourcePatches::patch_type s_patch_type;
        typedef typename TargetPatches::patch_type t_patch_type;
        typedef typename OffsetMap::offset_type offset_type;


        PatchMatcher(const SourcePatches &s, const TargetPatches &t,
                     const DistanceFunction &patch_distance, unsigned int seed = 0) :
                s(s), t(t),
                patch_distance(patch_distance),
                offset_map(s, t), distance_map(s, t) {
            if (seed == 0) {
                std::random_device r;
                seed = r();
            }
            generator = std::default_random_engine(seed);
        }

        void initialize_offset_map_randomly() {
            for (const s_patch_type &p : s) {
                t_patch_type q = t.pick_random(generator);
                offset_map(p) = offset_map.to_offset(p, q);
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

        inline void set_offset(const s_patch_type &p, const offset_type &offset) {
            offset_map(p) = offset;
            distance_map(p) = patch_distance(p, offset_map.from_offset(p, offset));
        }

        const OffsetMap &get_offset_map() const {
            return offset_map;
        }


        const DistanceMap &get_distance_map() const {
            return distance_map;
        }


    private:

        distance_type _propagate_distance(const s_patch_type &p, const s_patch_type &n, distance_type max_d) {

            if (!s.contains_patch(n)) {
                return std::numeric_limits<distance_type>::max();
            }

            t_patch_type q = offset_map.from_offset(p, offset_map(n));
            if (!t.contains_patch(q)) {
                return std::numeric_limits<distance_type>::max();
            }
            return patch_distance(p, q, max_d);
        }

        void _propagate(const s_patch_type &p, const array<Vec2i, 2> &neighbors) {
            t_patch_type q = offset_map.from_offset(p, offset_map(p));
            int &min_d = distance_map(p);

            for (int n = 0; n < neighbors.size(); n++) {
                int d = _propagate_distance(p, neighbors[n], min_d);
                if (d <= min_d) {
                    min_d = d;
                    offset_map(p) = offset_map(neighbors[n]);
                }
            }
        }

        void _random_search(const s_patch_type &p) {
            t_patch_type q = offset_map.from_offset(p, offset_map(p));
            int r = t.get_max_window_size(q);
            int &min_d = distance_map(p);

            while (r >= 1) {
                t_patch_type new_q = t.pick_random_in_window(generator, q, r);

                if (t.contains_patch(new_q)) {
                    int d = patch_distance(p, new_q);
                    if (d < min_d) {
                        offset_map(p) = offset_map.to_offset(p, new_q);
                    }
                }


                r /= 2;
            }
        }

        void _iterate_rd() {
            for (const s_patch_type &p : s) {
                _propagate(p, {p - Vec2i(1, 0), p - Vec2i(0, 1)});
                _random_search(p);
            }
        }

        void _iterate_lu() {
            auto it = s.rbegin();
            for (; it != s.rend(); it++) {
                const s_patch_type &p = *it;
                _propagate(p, {p + Vec2i(1, 0), p + Vec2i(0, 1)});
                _random_search(p);
            }
        }

        DistanceFunction patch_distance;
        SourcePatches s;
        TargetPatches t;
        std::default_random_engine generator;
        OffsetMap offset_map;
        DistanceMap distance_map;

    };
}

#endif //PATCH_MATCH_PATCH_MATCHER_H
