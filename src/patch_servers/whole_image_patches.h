//
// Created by antoinewdg on 11/22/16.
//

#ifndef PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
#define PATCH_MATCH_WHOLE_IMAGE_PATCHES_H

#include <boost/iterator/iterator_facade.hpp>
#include <opencv2/core/core.hpp>
#include <random>

#include "pixel_iterators.h"

namespace pm {

    using cv::Vec2i;
    using cv::Size;

    class WholeImagePatches {

    public:
        typedef ConstPixelIterator const_iterator;
        typedef ConstReversePixelIterator const_reverse_iterator;

        WholeImagePatches(const Size &size, int P) :
                m_begin(P / 2, P / 2),
                m_end(size.height - P / 2, size.width - P / 2),
                m_size(size.width - P + 1, size.height - P + 1),
                i_dist(m_begin[0], m_end[0] - 1),
                j_dist(m_begin[1], m_end[1] - 1) {

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
        inline Vec2i pick_random(RandomEngine &generator) {
            return Vec2i(i_dist(generator), j_dist(generator));
        }


        inline Size size() const {
            return m_size;
        }

        inline bool contains_patch(const Vec2i &p) const {
            return p[0] >= m_begin[0] && p[0] < m_end[0] &&
                   p[1] >= m_begin[1] && p[1] < m_end[1];
        }

    private:
        Vec2i m_begin, m_end;
        Size m_size;
        std::uniform_int_distribution<int> i_dist, j_dist;
    };
}

#endif //PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
