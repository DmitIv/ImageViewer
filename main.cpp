#include <iostream>

#include "uinterface.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: ImageViewer <path_to_directory>" << std::endl;
        return -1;
    }

    int window_width = 600;
    int window_height = 800;

    switch (UInterface(argv[1], window_width, window_height).start()) {
        case NO_IMAGES:
            std::cout << "target directory doesn't have images" << std::endl;
            return -2;
        case OK:
            std::cout << "exit" << std::endl;
            return 0;
    }
}