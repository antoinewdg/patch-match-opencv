//
// Created by antoinewdg on 11/24/16.
//

#ifndef PATCH_MATCH_MAPS_H
#define PATCH_MATCH_MAPS_H

#include "structs.h"
#include "patch_servers.h"

namespace pm {
    namespace dummies {
        class OffsetMap {
        public:

            typedef Offset offset_type;
            typedef SourcePatch s_patch_type;
            typedef TargetPatch t_patch_type;


            inline offset_type to_offset(const s_patch_type &p, const t_patch_type &q) {
                return offset_type();
            }

            inline t_patch_type from_offset(const s_patch_type &p, const offset_type &offset) {
                return t_patch_type();
            }

            inline offset_type operator()(const s_patch_type &p) const {
                return offset_type();
            }

            inline offset_type &operator()(const s_patch_type &p) {
                return m_dummy_offset;
            }

        private:
            offset_type m_dummy_offset;

        };

        class DistanceMap {
        public:

            typedef SourcePatch patch_type;
            typedef DistanceType distance_type;


            inline distance_type operator()(const patch_type &p) const {
                return distance_type();
            }

            inline distance_type &operator()(const patch_type &p) {
                return m_dummy_distance;
            }


        private:
            DistanceType m_dummy_distance;
        };
    }
}
#endif //PATCH_MATCH_MAPS_H
