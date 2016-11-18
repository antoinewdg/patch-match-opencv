//
// Created by antoinewdg on 11/17/16.
//

#ifndef PATCH_MATCH_UTILS_H
#define PATCH_MATCH_UTILS_H


#include "opencv2/imgcodecs/imgcodecs.hpp"

template<class T = cv::Vec3b>
cv::Mat_<T> load_test_asset(std::string name, int flag = cv::IMREAD_COLOR) {

    cv::Mat_<T> im = cv::imread("../test/assets/" + name, flag);
    return im;
}


#endif //PATCH_MATCH_UTILS_H
