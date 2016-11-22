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


#endif //PATCH_MATCH_UTILS_H
