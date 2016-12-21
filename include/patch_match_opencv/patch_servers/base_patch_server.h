//
// Created by antoinewdg on 12/21/16.
//

#ifndef PATCH_MATCH_OPENCV_BASE_PATCH_SERVER_H
#define PATCH_MATCH_OPENCV_BASE_PATCH_SERVER_H

#include <array>
#include <random>

#include <opencv2/core/core.hpp>

namespace pm {
    namespace opencv {
        using cv::Vec2i;
        using cv::Size;

        class BasePatchServer {
        public:
            typedef Vec2i patch_type;
            typedef int window_size_type;
            typedef std::array<patch_type, 2> predecessors_type;

            BasePatchServer(Size size, int P) :
                    m_size(size), P(P), half_p(P / 2) {}

            inline window_size_type get_max_window_size(const patch_type &c) {
                return std::max({m_size.height - half_p - c[0],
                                 m_size.width - half_p - c[1],
                                 c[0] - half_p + 1, c[1] - half_p + 1});
            }

            template<class RandomEngine>
            inline patch_type pick_random_in_window(RandomEngine &generator,
                                                    const patch_type &c,
                                                    window_size_type r) {
                std::uniform_int_distribution<int> k_dist(c[0] - r, c[0] + r);
                std::uniform_int_distribution<int> l_dist(c[1] - r, c[1] + r);
                return patch_type(k_dist(generator), l_dist(generator));
            }

            inline predecessors_type get_regular_predecessors(const patch_type &p) const {
                return {p - Vec2i(1, 0), p - Vec2i(0, 1)};
            };

            inline predecessors_type get_reverse_predecessors(const patch_type &p) const {
                return {p + Vec2i(1, 0), p + Vec2i(0, 1)};
            };

            inline Size size() const {
                return m_size;
            }

        protected:

            inline bool _is_patch_inside_boundaries(const patch_type &p) const {
                return p[0] >= half_p && p[0] < m_size.height - half_p &&
                       p[1] >= half_p && p[1] < m_size.width - half_p;
            }

            Size m_size;
            int P, half_p;
            std::uniform_int_distribution<int> i_dist, j_dist;
        };
    }
}
#endif //PATCH_MATCH_OPENCV_BASE_PATCH_SERVER_H
