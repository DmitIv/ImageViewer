#ifndef IMAGEVIEWER_IMAGE_CLASSIFIER_H
#define IMAGEVIEWER_IMAGE_CLASSIFIER_H

#include <string>
#include <opencv2/dnn.hpp>

void init_net(const std::string &model, const std::string &config, const std::string &framework,
              const std::string &classes, int backend, int target);

void init_preprocessing(int image_width, int image_height, cv::Scalar mean, float scale, bool swap_color);

int forward(const cv::Mat &image);

std::string label_lookup(int class_id);

#endif //IMAGEVIEWER_IMAGE_CLASSIFIER_H
