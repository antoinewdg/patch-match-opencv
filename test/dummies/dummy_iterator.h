//
// Created by antoinewdg on 11/24/16.
//

#ifndef PATCH_MATCH_DUMMY_ITERATOR_H
#define PATCH_MATCH_DUMMY_ITERATOR_H

#include <boost/iterator/iterator_facade.hpp>

namespace pm {
    namespace dummies {
        template <class T>
        class DummyConstIterator : public boost::iterator_facade<
                DummyConstIterator<T>, T const, boost::forward_traversal_tag> {

        private:
            friend class boost::iterator_core_access;

            inline void increment() {
            }

            inline bool equal(const DummyConstIterator &other) const {
                return true;
            }

            inline const T &dereference() const {
                return m_current;
            }

            T m_current;
        };
    }
}

#endif //PATCH_MATCH_DUMMY_ITERATOR_H
