#include <opencv2/opencv.hpp>
#include <algorithm>

#include "uinterface.h"
#include "image_viewer.h"

#include <iostream>

using namespace std;
using namespace cv;

int w_width, w_height;
const int hist_window_width = 512, hist_window_height = 400;
const string main_window_name = "Image viewer", histogram_window_name = "Histogram";

static void show_image(const string &image_file) {
    Mat image, hist;

    try {
        image = imread(image_file, 1);
        hist = make_histogram_image(image);
    } catch (Exception &exception) {
        cout << "Error with image reading: " << exception.err << endl;
        image = Mat::zeros(w_height, w_width, CV_8UC3);
        hist = Mat::zeros(hist_window_height, hist_window_width, CV_8UC3);
    }

    imshow("Histogram", hist);
    imshow("Image viewer", image);
}

static void on_trackbar(int pos, void *userdata) {
    auto data = (vector<string> *) userdata;
    cout << "Image: " << data->at(pos) << endl;
    show_image(data->at(pos));
}

result_t start_ui(const std::string &path_to_images, int window_width, int window_height) {
    const vector<string> images = list_images(path_to_images);
    if (images.empty()) {
        return NO_IMAGES;
    }

    const int max_index = images.size();
    int current_index = 0;

    w_width = window_width, w_height = window_height;

    namedWindow(main_window_name, WINDOW_NORMAL);
    resizeWindow(main_window_name, w_width, w_height);

    namedWindow(histogram_window_name, WINDOW_NORMAL);
    resizeWindow(histogram_window_name, hist_window_width, hist_window_height);

    if (max_index > 1) {
        createTrackbar("Image number", main_window_name, &current_index, max_index - 1, on_trackbar,
                       (void *) &images);
        on_trackbar(current_index, (void *) &images);
    }

    for (;;) {
        if (auto res = waitKey(0); res == 27) {
            return OK;
        }
    }
}
