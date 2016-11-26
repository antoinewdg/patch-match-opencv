//
// Created by antoinewdg on 11/15/16.
//

#ifndef PATCH_MATCH_MASKED_PATCHES_H
#define PATCH_MATCH_MASKED_PATCHES_H

#include <random>
#include <vector>

#include <opencv2/core/core.hpp>

namespace pm {
    namespace opencv {

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
            typedef Vec2i patch_type;
            typedef int window_size_type;
            typedef std::array<patch_type, 2> predecessors_type;

            MaskedPatches(const Mat_<bool> mask, int P) :
                    P(P), patches_mask(mask.size(), false), half_p(P / 2) {
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

                index_dist = std::uniform_int_distribution<int>(0, patches.size() - 1);
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

            Size size() const {
                return patches_mask.size();
            }

            bool contains_patch(const Vec2i &p) {
                return p[0] >= half_p && p[0] <= patches_mask.rows - half_p &&
                       p[1] >= half_p && p[1] <= patches_mask.cols - half_p &&
                       patches_mask(p);
            }

            template<class RandomEngine>
            inline Vec2i pick_random(RandomEngine &generator) {
                return patches[index_dist(generator)];
            }

            template<class RandomEngine>
            inline patch_type pick_random_in_window(RandomEngine &generator,
                                                    const patch_type &c,
                                                    window_size_type r) {
                std::uniform_int_distribution<int> k_dist(c[0] - r, c[0] + r);
                std::uniform_int_distribution<int> l_dist(c[1] - r, c[1] + r);
                return patch_type(k_dist(generator), l_dist(generator));
            }

            inline window_size_type get_max_window_size(const patch_type &c) {
                return std::max({patches_mask.rows - P + 1 - c[0],
                                 patches_mask.cols - P + 1 - c[1],
                                 c[0], c[1]});
            }

            inline predecessors_type get_regular_predecessors(const patch_type &p) const {
                return {p - Vec2i(1, 0), p - Vec2i(0, 1)};
            };

            inline predecessors_type get_reverse_predecessors(const patch_type &p) const {
                return {p + Vec2i(1, 0), p + Vec2i(0, 1)};
            };

        private:
            int P, half_p;
            Mat_<bool> patches_mask;
            vector<Vec2i> patches;
            std::uniform_int_distribution<int> index_dist;
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
}

#endif //PATCH_MATCH_MASKED_PATCHES_H
