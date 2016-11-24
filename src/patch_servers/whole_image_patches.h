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

namespace pm {

    using cv::Vec2i;
    using cv::Size;

    class WholeImagePatches {

    public:
        typedef ConstPixelIterator const_iterator;
        typedef ConstReversePixelIterator const_reverse_iterator;
        typedef Vec2i patch_type;
        typedef int window_size_type;
        typedef std::array<patch_type, 2> predecessors_type;

        WholeImagePatches(const Size &size, int P) :
                m_begin(P / 2, P / 2),
                m_end(size.height - P / 2, size.width - P / 2),
                m_size(size),
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
        inline patch_type pick_random(RandomEngine &generator) {
            return patch_type(i_dist(generator), j_dist(generator));
        }

        template<class RandomEngine>
        inline patch_type pick_random_in_window(RandomEngine &generator,
                                                const patch_type &c,
                                                window_size_type r) {
            std::uniform_int_distribution<int> k_dist(c[0] - r, c[0] + r);
            std::uniform_int_distribution<int> l_dist(c[1] - r, c[1] + r);
            return patch_type(k_dist(generator), l_dist(generator));
        }


        inline Size size() const {
            return m_size;
        }

        inline bool contains_patch(const patch_type &p) const {
            return p[0] >= m_begin[0] && p[0] < m_end[0] &&
                   p[1] >= m_begin[1] && p[1] < m_end[1];
        }

        inline window_size_type get_max_window_size(const patch_type &c) {
            return std::max({m_end[0] - c[0], m_end[1] - c[1],
                             c[0] - m_begin[0], c[1] - m_begin[1]});
        }

        inline predecessors_type get_regular_predecessors(const patch_type &p) const{
            return {p - Vec2i(1, 0), p - Vec2i(0, 1)};
        };

        inline predecessors_type get_reverse_predecessors(const patch_type &p) const{
            return {p + Vec2i(1, 0), p + Vec2i(0, 1)};
        };


    private:
        Vec2i m_begin, m_end;
        Size m_size;
        std::uniform_int_distribution<int> i_dist, j_dist;
    };
}

#endif //PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
