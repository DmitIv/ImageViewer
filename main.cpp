#include <iostream>
#include <opencv2/opencv.hpp>


#include <filesystem>

#include "uinterface.h"


std::string keys =
        "{ help  h     | | Print help message. }"
        "{@path        | | Path to directory with images. }";

int main(int argc, char **argv) {
    cv::CommandLineParser parser(argc, argv, keys);
    parser = cv::CommandLineParser(argc, argv, keys);

    parser.about("Viewing of images from directory.");

    if (argc == 1 || parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    int window_width = 600;
    int window_height = 800;

    switch (UInterface(parser.get<std::string>(0), window_width, window_height).start()) {
        case NO_IMAGES:
            std::cout << "target directory doesn't have images" << std::endl;
            return -2;
        case OK:
            std::cout << "exit" << std::endl;
            return 0;
    }
}
