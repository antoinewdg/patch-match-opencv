//
// Created by antoinewdg on 11/22/16.
//

#ifndef PATCH_MATCH_PIXEL_ITERATORS_H
#define PATCH_MATCH_PIXEL_ITERATORS_H

#include <boost/iterator/iterator_facade.hpp>
#include <opencv2/core/core.hpp>


namespace pm {
    namespace opencv {
        using cv::Vec2i;
        using cv::Size;

        class ConstPixelIterator : public boost::iterator_facade<
                ConstPixelIterator, Vec2i const, boost::forward_traversal_tag> {
        public:
            ConstPixelIterator(const Vec2i &start, const Vec2i &end) :
                    m_current(start), m_start(start), m_end(end) {}

        private:
            friend class boost::iterator_core_access;

            inline void increment() {
                m_current[1]++;
                if (m_current[1] >= m_end[1]) {
                    m_current[0]++;
                    if (m_current[0] < m_end[0]) {
                        m_current[1] = m_start[1];
                    }
                }
            }

            inline bool equal(const ConstPixelIterator &other) const {
                return m_current == other.m_current;
            }

            inline const Vec2i &dereference() const {
                return m_current;
            }

            Vec2i m_current, m_start, m_end;
        };

        class ConstReversePixelIterator : public boost::iterator_facade<
                ConstReversePixelIterator, Vec2i const, boost::forward_traversal_tag> {
        public:
            ConstReversePixelIterator(const Vec2i &start, const Vec2i &end) :
                    m_current(end - Vec2i(1, 1)), m_start(start), m_end(end) {}

        private:
            friend class boost::iterator_core_access;

            inline void increment() {
                m_current[1]--;
                if (m_current[1] < m_start[1]) {
                    m_current[0]--;
                    if (m_current[0] >= m_start[0]) {
                        m_current[1] = m_end[1] - 1;
                    }
                }
            }

            inline bool equal(const ConstReversePixelIterator &other) const {
                return m_current == other.m_current;
            }

            inline const Vec2i &dereference() const {
                return m_current;
            }

            Vec2i m_current, m_start, m_end;
        };
    }
}
#endif //PATCH_MATCH_PIXEL_ITERATORS_H
