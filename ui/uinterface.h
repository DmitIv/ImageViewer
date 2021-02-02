#ifndef IMAGEVIEWER_UINTERFACE_H
#define IMAGEVIEWER_UINTERFACE_H

#include <vector>
#include <string>

enum result_t {
    OK = 0,
    NO_IMAGES
};

result_t start_ui(const std::string &path_to_images, int window_width, int window_height,
                  bool with_classifier);


#endif //IMAGEVIEWER_UINTERFACE_H
