//
// Created by antoinewdg on 11/24/16.
//

#ifndef PATCH_MATCH_STRUCTS_H
#define PATCH_MATCH_STRUCTS_H

namespace pm {
    namespace dummies {

        class SourcePatch {};
        class SourceWindowSize {};

        class TargetPatch {};
        class TargetWindowSize {};

        class DistanceType {};
        class Offset {};

        inline bool operator<(DistanceType a, DistanceType b){
            return true;
        }

        inline bool operator>=(TargetWindowSize s, int i){
            return false;
        }

        inline TargetWindowSize operator/=(TargetWindowSize s, int i){
            return s;
        }

    }
}

#endif //PATCH_MATCH_STRUCTS_H
