#ifndef IMAGEVIEWER_UINTERFACE_H
#define IMAGEVIEWER_UINTERFACE_H

#include <vector>
#include <string>

enum result_t {
    OK = 0,
    NO_IMAGES
};

class UInterface {
private:
    std::vector<std::string> images;

    int current_index;
    int max_index;

    const int width;
    const int height;
public:
    UInterface(const std::string &path_to_directory, int w, int h);

    result_t start();
};

#endif //IMAGEVIEWER_UINTERFACE_H
