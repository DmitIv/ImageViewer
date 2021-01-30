#include <opencv2/opencv.hpp>

#include <algorithm>
#include <unordered_set>

#include "image_viewer.h"


static const std::unordered_set<std::string_view> available_extensions{
        ".jpg", ".jpeg", ".png"
};

static bool is_not_image_file(const std::string &filename) {
    if (int ind = filename.rfind('.'); ind > 0) {
        return available_extensions.find(std::string_view{filename.data() + ind}) == available_extensions.end();
    }
    return true;
}

std::vector<std::string> list_images(const std::string &path_to_directory) {
    std::vector<std::string> result{};
    cv::glob(path_to_directory, result);
    result.erase(std::remove_if(result.begin(), result.end(), is_not_image_file), result.end());
    return result;
}