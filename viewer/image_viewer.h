#ifndef IMAGE_VIEWER_IMAGE_VIEWER_H
#define IMAGE_VIEWER_IMAGE_VIEWER_H

#include <opencv2/opencv.hpp>

#include <vector>
#include <string>

std::vector<std::string> list_images(const std::string &path_to_directory);

cv::Mat make_histogram_image(const cv::Mat &bgr_image);

#endif //IMAGE_VIEWER_IMAGE_VIEWER_H
