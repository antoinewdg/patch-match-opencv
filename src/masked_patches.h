//
// Created by antoinewdg on 11/15/16.
//

#ifndef PATCH_MATCH_MASKED_PATCHES_H
#define PATCH_MATCH_MASKED_PATCHES_H

#include <vector>

#include "opencv2/core/core.hpp"

namespace pm {

    using cv::Vec2i;
    using cv::Mat_;
    using cv::Rect;
    using cv::Size;

    using std::vector;

    template<class Predicate>
    class MaskedPatches {
    public:

        typedef vector<Vec2i>::const_iterator const_iterator;
        typedef vector<Vec2i>::const_reverse_iterator const_reverse_iterator;

        MaskedPatches(const Mat_<bool> mask, int P) : P(P), patches_mask(mask.size(), false) {
            int p_2 = P / 2;
            Predicate predicate;
            for (int i = p_2; i < mask.rows - p_2; i++) {
                for (int j = p_2; j < mask.cols - p_2; j++) {
                    if (predicate(mask, i, j, P)) {
                        patches.emplace_back(i, j);
                        patches_mask(i, j) = true;
                    }
                }
            }
        }

        const_iterator begin() const {
            return patches.begin();
        }

        const_iterator end() const {
            return patches.end();
        }

        const_reverse_iterator rbegin() const {
            return patches.rbegin();
        }

        const_reverse_iterator rend() const {
            return patches.rend();
        }

        const Vec2i &operator[](int i) const {
            return patches[i];
        }

        Size size() const {
            return patches_mask.size();
        }

        bool contains_patch(const Vec2i &p) {
            return patches_mask(p);
        }

    private:
        int P;
        Mat_<bool> patches_mask;
        vector<Vec2i> patches;
    };

    struct AtLeastOneInPatch {
        bool operator()(const Mat_<bool> &mask, int i, int j, int P) {
            int p_2 = P / 2;
            int n = cv::countNonZero(mask(Rect(j - p_2, i - p_2, P, P)));
            return n > 0;
        }
    };

    struct AllInPatch {
        bool operator()(const Mat_<bool> &mask, int i, int j, int P) {
            int p_2 = P / 2;
            int n = cv::countNonZero(mask(Rect(j - p_2, i - p_2, P, P)));
            return n == P * P;
        }
    };

    typedef MaskedPatches<AtLeastOneInPatch> PartiallyMaskedPatches;
    typedef MaskedPatches<AllInPatch> TotallyMaskedPatches;




}

#endif //PATCH_MATCH_MASKED_PATCHES_H
