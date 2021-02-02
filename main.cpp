#include <iostream>
#include <opencv2/core/utility.hpp>

#include "uinterface.h"
#include "image_classifier.h"

using namespace std;
using namespace cv;

string keys =
        "{ help h      | | Print help message. }"
        "{@path        | | Path to directory with images. }"
        "{ model m     | | Path to a binary file of model. }"
        "{ config c    | | Path to a text file of model contains network configuration. }"
        "{ framework f | | Optional name of an origin framework of the model. Detect it automatically if it does not set. }"
        "{ classes     | | Optional path to a text file with names of classes. }"
        "{ mean mn     | | Preprocess input image by subtracting mean values. Mean values should be in BGR order and delimited by spaces. }"
        "{ scale sc    | 1.0 | Preprocess input image by multiplying on a scale factor. }"
        "{ rgb r       | | Indicate that model works with RGB input images instead BGR ones. }"
        "{ width wt    | 600 | Width of main window. }"
        "{ height ht   | 800 | Height of main window. }"
        "{ image_height iht |  | Preprocess input image by resizing to a specific width.}"
        "{ image_width iwt |  | Preprocess input image by resizing to a specific height.}"
        "{ backend     | 0 | Choose one of computation backends: "
        "0: automatically (by default), "
        "1: Halide language (http://halide-lang.org/), "
        "2: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
        "3: OpenCV implementation }"
        "{ target      | 0 | Choose one of target computation devices: "
        "0: CPU target (by default), "
        "1: OpenCL, "
        "2: OpenCL fp16 (half-float precision), "
        "3: VPU }";

int main(int argc, char **argv) {
    CommandLineParser parser(argc, argv, keys);
    parser = CommandLineParser(argc, argv, keys);

    parser.about("Viewing of images from directory. Esc for exit.");

    if (argc == 1 || parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    string path_to_directory = parser.get<string>(0);

    int window_width = parser.get<int>("width");
    int window_height = parser.get<int>("height");

    bool with_classifier = parser.has("model");

    if (with_classifier) {
        string path_to_model = samples::findFile(parser.get<string>("model"));
        string path_to_config = samples::findFile(parser.get<string>("config"));
        string framework = parser.get<string>("framework");
        string classes = parser.get<string>("classes");

        int backend = parser.get<int>("backend");
        int target = parser.get<int>("target");

        Scalar mean = parser.get<Scalar>("mean");
        float scale = parser.get<float>("scale");
        bool swapRB = parser.get<bool>("rgb");

        int image_width = parser.get<int>("image_width");
        int image_height = parser.get<int>("image_height");

        if (!parser.check()) {
            parser.printErrors();
            return -1;
        }

        init_net(path_to_model, path_to_config, framework, classes, backend, target);
        init_preprocessing(image_width, image_height, move(mean), scale, swapRB);
    }

    switch (start_ui(path_to_directory, window_width, window_height, with_classifier)) {
        case NO_IMAGES:
            cout << "Target directory doesn't have images" << endl;
            return 0;
        case OK:
            cout << "Exit" << endl;
            return 0;
    }
}
