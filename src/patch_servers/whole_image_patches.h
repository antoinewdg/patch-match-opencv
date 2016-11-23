//
// Created by antoinewdg on 11/22/16.
//

#ifndef PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
#define PATCH_MATCH_WHOLE_IMAGE_PATCHES_H

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

        WholeImagePatches(const Size &size, int P) :
                m_begin(P / 2, P / 2),
                m_end(size.height - P / 2, size.width - P / 2),
                m_size(size.width - P + 1, size.height - P + 1) {

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

        inline Vec2i operator[](int i) const {
            int l = m_end[1] - m_begin[1];
            return Vec2i(i / l + m_begin[0], i % l + m_begin[1]);
        }

        inline Size size() const {
            return m_size;
        }

        inline bool contains_patch(const Vec2i &p) const {
            return true;
        }

    private:
        Vec2i m_begin, m_end;
        Size m_size;
    };
}

#endif //PATCH_MATCH_WHOLE_IMAGE_PATCHES_H
