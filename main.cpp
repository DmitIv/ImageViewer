#include <iostream>
#include <opencv2/opencv.hpp>

#include "uinterface.h"

using namespace std;
using namespace cv;

string keys =
        "{ help h      | | Print help message. }"
        "{@path        | | Path to directory with images. }"
        "{ width wt    | 600 | Width of main window. }"
        "{ height ht   | 800 | Height of main window }";

int main(int argc, char **argv) {
    CommandLineParser parser(argc, argv, keys);
    parser = CommandLineParser(argc, argv, keys);

    parser.about("Viewing of images from directory. Esc for exit.");

    if (argc == 1 || parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    int window_width = parser.get<int>("width");
    int window_height = parser.get<int>("height");

    switch (start_ui(parser.get<string>(0), window_width, window_height)) {
        case NO_IMAGES:
            cout << "Target directory doesn't have images" << endl;
            return 0;
        case OK:
            cout << "Exit" << endl;
            return 0;
    }
}
