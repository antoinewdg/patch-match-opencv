//
// Created by antoinewdg on 11/17/16.
//

#ifndef PATCH_MATCH_PATCH_DISTANCES_H
#define PATCH_MATCH_PATCH_DISTANCES_H

#include <iostream>
#include <limits>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace pm {

    using cv::Mat_;
    using cv::Vec;
    using cv::Vec2i;
    using cv::Rect;

    template<int P, class I, class O, int n_channels>
    class EuclidianSquaredDistance {

    public:
        typedef Vec2i left_type;
        typedef Vec2i right_type;
        typedef O output_type;
        typedef Mat_<Vec<I, n_channels>> mat_type;


        EuclidianSquaredDistance(const mat_type &a, const mat_type &b) :
                m_a(a), m_b(b) {
        }

        inline output_type operator()(const Vec2i &p, const Vec2i &q,
                                      output_type max_d = std::numeric_limits<output_type>::max()) {
            return _sq_euclidian_distance(
                    m_a(_get_subrect(p)),
                    m_b(_get_subrect(q)),
                    max_d
            );
        }

    private:

        inline Rect _get_subrect(const Vec2i &p) {
            return Rect(p[1] - P / 2, p[0] - P / 2, P, P);
        }

        inline output_type _sq_euclidian_distance(const mat_type &s, const mat_type &t, output_type max_d) {
            output_type d = output_type(0);

            for (int i = 0; i < P; i++) {
                for (int j = 0; j < P; j++) {

                    for (int k = 0; k < 3; k++) {
                        output_type temp = output_type(s(i, j)[k] - t(i, j)[k]);
                        d += temp * temp;
                    }
                    if (d > max_d) {
                        return d;
                    }
                }
            }

            return d;
        }

        const mat_type &m_a, &m_b;
    };
}

#endif //PATCH_MATCH_PATCH_DISTANCES_H
