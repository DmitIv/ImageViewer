#include <opencv2/opencv.hpp>
#include <algorithm>

#include "uinterface.h"
#include "image_viewer.h"

#include <iostream>

using namespace std;
using namespace cv;

static void show_image(const string &image_file) {
    Mat image, hist;

    try {
        image = imread(image_file, 1);
        hist = make_histogram_image(image);
    } catch (Exception &exception) {
        cout << "Error with image reading: " << exception.err << endl;
        image = Mat::zeros(128, 128, CV_8UC3);
        hist = Mat::zeros(400, 512, CV_8UC3);
    }

    imshow("Histogram", hist);
    imshow("Image viewer", image);
}

static void on_trackbar(int pos, void *userdata) {
    auto data = (vector<string> *) userdata;
    cout << "Image: " << data->at(pos) << endl;
    show_image(data->at(pos));
}

UInterface::UInterface(const string &path_to_directory, int w, int h) :
        images(move(list_images(path_to_directory))),
        current_index(0),
        max_index(images.size()),
        width(w),
        height(h) {
}


result_t UInterface::start() {
    if (images.size() == 0) {
        return NO_IMAGES;
    }

    namedWindow("Image viewer", WINDOW_NORMAL);
    resizeWindow("Image viewer", width, height);

    namedWindow("Histogram", WINDOW_NORMAL);
    resizeWindow("Histogram", 512, 400);

    if (images.size() > 1) {
        cout << "Open trackbar for switching between images" << endl;
        createTrackbar("Image number", "Image viewer", &current_index, max_index - 1, on_trackbar,
                           &images);
        on_trackbar(current_index, &images);
    }
    for (;;) {
        if (auto res = waitKey(0); res == 27) {
            return OK;
        }
    }
}