#include <opencv2/opencv.hpp>

#include "uinterface.h"
#include "image_viewer.h"
#include "image_classifier.h"

#include <iostream>

using namespace std;
using namespace cv;

static bool with_classifier;
static int w_width, w_height;
static const int hist_window_width = 512, hist_window_height = 400;
static const string main_window_name = "Image viewer", histogram_window_name = "Histogram";

static Mat hist_image;
static vector<string> images;

static void clear_histogram() {
    /*
     * Prepare histogram image for new iteration of drawing.
     */
    for (int i = 0; i < hist_image.rows; i++) {
        int *hist_image_i = hist_image.ptr<int>(i);
        for (int j = 0; j < hist_image.cols; j++) {
            hist_image_i[j] = 0;
        }
    }
}

static void show_image(const string &image_file) {
    /*
     * Show image and histogram for it.
     * If ImageViewer was started with image classifier, perform classification for image.
     */
    Mat image;

    clear_histogram();
    try {
        image = imread(image_file, 1);
        make_histogram_image(image, hist_image);
        if (with_classifier) {
            int class_id = forward(image);
            putText(image, label_lookup(class_id), Point(0, 40),
                    FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
        }
    } catch (Exception &exception) {
        cout << "Error on image reading: " << exception.err << endl;
        image = Mat::zeros(w_height, w_width, CV_8UC3);
    }

    imshow("Histogram", hist_image);
    imshow("Image viewer", image);
}

static void on_trackbar(int pos, void *userdata) {
    /*
     * Get path to image by using position of trackbar and show it
     */
    auto data = (vector<string> *) userdata;
    cout << "Image: " << data->at(pos) << endl;
    show_image(data->at(pos));
}

result_t start_ui(const std::string &path_to_images, int window_width, int window_height,
                  bool _with_classifier) {
    /*
     * Perform initial action for image viewer part and start GUI.
     * Wait for ESC pressing for exit.
     */
    images = list_images(path_to_images);
    if (images.empty()) {
        return NO_IMAGES;
    }

    hist_image = Mat(hist_window_height, hist_window_width, CV_8UC3, Scalar(0, 0, 0));

    const int max_index = images.size();
    int current_index = 0;

    with_classifier = _with_classifier;
    w_width = window_width, w_height = window_height;

    namedWindow(main_window_name, WINDOW_NORMAL);
    resizeWindow(main_window_name, w_width, w_height);

    namedWindow(histogram_window_name, WINDOW_NORMAL);
    resizeWindow(histogram_window_name, hist_window_width, hist_window_height);

    if (max_index > 1) {
        createTrackbar("Image number", main_window_name, &current_index, max_index - 1, on_trackbar,
                       (void *) &images);
    }
    on_trackbar(current_index, (void *) &images);

    for (;;) {
        if (auto res = waitKey(0); res == 27) {
            return OK;
        }
    }
}
