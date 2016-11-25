//
// Created by antoinewdg on 11/23/16.
//

#ifndef PATCH_MATCH_2D_OFFSET_MAP_H
#define PATCH_MATCH_2D_OFFSET_MAP_H

#include <opencv2/core/core.hpp>

namespace pm {
    namespace opencv {
        using cv::Vec2i;
        using cv::Mat_;
        using cv::Size;

        class OffsetMap2D {
        public:

            typedef Vec2i s_patch_type;
            typedef Vec2i t_patch_type;
            typedef Vec2i offset_type;

            OffsetMap2D(Size size) : m_data(size) {}

            inline offset_type to_offset(const s_patch_type &p, const t_patch_type &q) {
                return q - p;
            }

            inline t_patch_type from_offset(const s_patch_type &p, const offset_type &offset) {
                return p + offset;
            }

            inline const offset_type &operator()(const s_patch_type &p) const {
                return m_data(p);
            }

            inline offset_type &operator()(const s_patch_type &p) {
                return m_data(p);
            }

            inline const Mat_<offset_type> &to_mat() const {
                return m_data;
            }


        private:
            Mat_<Vec2i> m_data;
        };

        template<class T>
        class DistanceMap2d {
        public:

            typedef Vec2i patch_type;
            typedef T distance_type;

            DistanceMap2d(Size size) : m_data(size) {}

            inline distance_type operator()(const patch_type &p) const {
                return m_data(p);
            }

            inline distance_type &operator()(const patch_type &p) {
                return m_data(p);
            }

            inline const Mat_<distance_type> &to_mat() const {
                return m_data;
            }


        private:
            Mat_<distance_type> m_data;
        };
    }
}

#endif //PATCH_MATCH_2D_OFFSET_MAP_H
