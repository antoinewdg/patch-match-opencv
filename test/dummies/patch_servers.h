//
// Created by antoinewdg on 11/24/16.
//

#ifndef PATCH_MATCH_PATCH_SERVERS_H
#define PATCH_MATCH_PATCH_SERVERS_H

#include <array>

#include "structs.h"
#include "dummy_iterator.h"

namespace pm {
    namespace dummies {


        class SourcePatches {
        public:
            typedef SourcePatch patch_type;
            typedef DummyConstIterator<patch_type> const_iterator;
            typedef DummyConstIterator<patch_type> const_reverse_iterator;
            typedef std::array<patch_type, 8> predecessors_type;

            inline const_iterator begin() const {
                return const_iterator();
            }

            inline const_iterator end() const {
                return const_iterator();
            }

            inline const_reverse_iterator rbegin() const {
                return const_reverse_iterator();
            }

            inline const_reverse_iterator rend() const {
                return const_reverse_iterator();
            }

            inline bool contains_patch(const patch_type &p) const {
                return false;
            }

            inline predecessors_type get_regular_predecessors(const patch_type &p) const {
                return predecessors_type();
            };

            inline predecessors_type get_reverse_predecessors(const patch_type &p) const {
                return predecessors_type();
            };
        };


        class TargetPatches {
        public:
            typedef TargetPatch patch_type;
            typedef TargetWindowSize window_size_type;

            template<class RandomEngine>
            inline patch_type pick_random(RandomEngine &generator) {
                return patch_type();
            }

            template<class RandomEngine>
            inline patch_type pick_random_in_window(RandomEngine &generator,
                                                    const patch_type &c,
                                                    window_size_type r) {
                return patch_type();
            }


            inline bool contains_patch(const patch_type &p) const {
                return false;
            }

            inline window_size_type get_max_window_size(const patch_type &c) {
                return window_size_type();
            }

        };
    }
}

#endif //PATCH_MATCH_PATCH_SERVERS_H
