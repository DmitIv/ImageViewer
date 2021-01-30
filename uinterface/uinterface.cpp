#include <opencv2/opencv.hpp>
#include <algorithm>

#include "uinterface.h"
#include "image_viewer.h"

#include <iostream>

static void show_image(const std::string &image_file) {
    cv::Mat image, hist;

    try {
        image = cv::imread(image_file, 1);
        hist = make_histogram_image(image);
    } catch (cv::Exception &exception) {
        std::cout << "Error with image reading: " << exception.err << std::endl;
        image = cv::Mat::zeros(128, 128, CV_8UC3);
        hist = cv::Mat::zeros(400, 512, CV_8UC3);
    }

    cv::imshow("Histogram", hist);
    cv::imshow("Image viewer", image);
}

static void on_trackbar(int pos, void *userdata) {
    auto data = (std::vector<std::string> *) userdata;
    std::cout << "Image: " << data->at(pos) << std::endl;
    show_image(data->at(pos));
}

UInterface::UInterface(const std::string &path_to_directory, int w, int h) :
        images(std::move(list_images(path_to_directory))),
        current_index(0),
        max_index(images.size()),
        width(w),
        height(h) {
}


result_t UInterface::start() {
    if (images.size() == 0) {
        return NO_IMAGES;
    }

    cv::namedWindow("Image viewer", cv::WINDOW_NORMAL);
    cv::resizeWindow("Image viewer", width, height);

    cv::namedWindow("Histogram", cv::WINDOW_NORMAL);
    cv::resizeWindow("Histogram", 512, 400);

    if (images.size() > 1) {
        std::cout << "Open trackbar for switching between images" << std::endl;
        cv::createTrackbar("Image number", "Image viewer", &current_index, max_index - 1, on_trackbar,
                           &images);
        on_trackbar(current_index, &images);
    }
    cv::waitKey(0);

    return OK;
}