#include <fstream>
#include <iostream>

#include "image_classifier.h"

using namespace cv;
using namespace std;

static dnn::Net net;
static cv::Scalar im_mean;

static int im_width, im_height;
static float im_scale;
static bool swapRB;

static vector<string> classes_labels;

static void read_classes_labels(const string &classes) {
    /*
     * Read classes labels from file by path
     */
    if (!classes.empty()) {
        std::ifstream ifs{classes.c_str()};
        if (!ifs.is_open()) {
            CV_Error(Error::StsError, "File with classes labels not found: " + classes);
        }
        for (string line; getline(ifs, line); classes_labels.push_back(line)) {}
    }
}


void init_net(const std::string &model, const std::string &config, const std::string &framework,
              const std::string &classes, int backend, int target) {
    /*
     * Perform initial action for network in classifier part
     */
    net = dnn::readNet(model, config, framework);
    net.setPreferableBackend(backend);
    net.setPreferableTarget(target);
    read_classes_labels(classes);
}

void init_preprocessing(int image_width, int image_height, cv::Scalar mean, float scale, bool swap_color) {
    /*
     * Perform initial action for image preprocessing in classifier part
     */
    im_width = image_width, im_height = image_height;
    im_mean = move(mean);
    im_scale = scale;
    swapRB = swap_color;
}

int forward(const Mat &image) {
    /*
     * Forward pass trough prepared network.
     * Return class id as integer.
     */
    Mat blob, prob;
    Point class_id_point;

    dnn::blobFromImage(image, blob, im_scale, Size(im_width, im_height), im_mean, swapRB, false);
    net.setInput(blob);

    prob = net.forward();
    minMaxLoc(prob.reshape(1, 1), 0, nullptr, 0, &class_id_point);

    return class_id_point.x;
}

string label_lookup(int class_id) {
    /*
     * Transform class id to string by using classes labels
     */
    return format("%s", (class_id < classes_labels.size() ? classes_labels[class_id].c_str()
                                                          : format("Class %d", class_id).c_str()));
}
