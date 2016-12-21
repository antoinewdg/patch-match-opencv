//
// Created by antoinewdg on 11/22/16.
//

#ifndef PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
#define PATCH_MATCH_WHOLE_IMAGE_PATCHES_H

#include <array>
#include <random>

#include <boost/iterator/iterator_facade.hpp>
#include <opencv2/core/core.hpp>

#include "pixel_iterators.h"
#include "base_patch_server.h"

namespace pm {
    namespace opencv {
        using cv::Vec2i;
        using cv::Size;

        class WholeImagePatches : public BasePatchServer {

        public:
            typedef ConstPixelIterator const_iterator;
            typedef ConstReversePixelIterator const_reverse_iterator;

            WholeImagePatches(const Size &size, int P) :
                    m_begin(P / 2, P / 2),
                    m_end(size.height - P / 2, size.width - P / 2),
                    i_dist(m_begin[0], m_end[0] - 1),
                    j_dist(m_begin[1], m_end[1] - 1),
                    BasePatchServer(size, P) {

            }

            inline const_iterator begin() const {
                return const_iterator(m_begin, m_end);
            }

            inline const_iterator end() const {
                return const_iterator(m_end, m_end);
            }

            inline const_reverse_iterator rbegin() const {
                return const_reverse_iterator(m_begin, m_end);
            }

            inline const_reverse_iterator rend() const {
                return const_reverse_iterator(m_begin, m_begin);
            }

            template<class RandomEngine>
            inline patch_type pick_random(RandomEngine &generator) {
                return patch_type(i_dist(generator), j_dist(generator));
            }

            inline bool contains_patch(const patch_type &p) const {
                return _is_patch_inside_boundaries(p);
            }


        private:
            Vec2i m_begin, m_end;
            std::uniform_int_distribution<int> i_dist, j_dist;
        };
    }
}

#endif //PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
