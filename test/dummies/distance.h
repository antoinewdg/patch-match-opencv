//
// Created by antoinewdg on 11/24/16.
//

#ifndef PATCH_MATCH_DISTANCE_H
#define PATCH_MATCH_DISTANCE_H

#include "structs.h"

namespace pm {
    namespace dummies {
        struct Distance {
            typedef SourcePatch left_type;
            typedef TargetPatch right_type;
            typedef DistanceType output_type;

            inline output_type operator()(const left_type &p, const right_type &q) {
                return output_type();
            }

            inline output_type operator()(const left_type &p, const right_type &q, output_type max_d) {
                return output_type();
            }
        };
    }
}
#endif //PATCH_MATCH_DISTANCE_H
