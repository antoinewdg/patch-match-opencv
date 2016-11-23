//
// Created by antoinewdg on 11/17/16.
//

#ifndef PATCH_MATCH_UTILS_H
#define PATCH_MATCH_UTILS_H


#include <opencv2/imgcodecs/imgcodecs.hpp>

template<class T = cv::Vec3b>
inline cv::Mat_<T> load_test_asset(std::string name, int flag = cv::IMREAD_COLOR) {

    cv::Mat_<T> im = cv::imread("../test/assets/" + name, flag);
    return im;
}

inline cv::Mat_<bool> load_mask_asset(std::string name) {
    cv::Mat_<bool> im = cv::imread("../test/assets/" + name, cv::IMREAD_GRAYSCALE);
    return im / 255;
}

template<class T, class U>
inline void require_matrix_equal(const cv::Mat_<T> &a, const cv::Mat_<U> &b) {
    if (a.size() != b.size()) {
        FAIL("Matrices of different sizes " << a.size() << " " << b.size());
    }
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            if (a(i, j) != b(i, j)) {
                FAIL("Matrices are different at index (" << i << "," << j << "): "
                                                         << a(i, j) << " != " << b(i, j));
            }
        }
    }

    REQUIRE(true);
};

#endif //PATCH_MATCH_UTILS_H
