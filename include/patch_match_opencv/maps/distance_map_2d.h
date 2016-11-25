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
