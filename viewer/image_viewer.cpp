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

cv::Mat make_histogram_image(const cv::Mat &image) {
    int hist_size = 256;
    float range[] = {0.0, 256.0};
    const float *hist_range = {range};

    std::vector<cv::Mat> bgr_image;
    cv::split(image, bgr_image);

    cv::Mat b_hist, g_hist, r_hist;
    cv::calcHist(&bgr_image[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size, &hist_range, true, false);
    cv::calcHist(&bgr_image[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size, &hist_range, true, false);
    cv::calcHist(&bgr_image[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size, &hist_range, true, false);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double) hist_w / hist_size);

    cv::Mat hist_image(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::normalize(b_hist, b_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < hist_size; i++) {
        line(hist_image, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             cv::Scalar(255, 0, 0), 2, 8, 0);
        line(hist_image, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
             cv::Scalar(0, 255, 0), 2, 8, 0);
        line(hist_image, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
             cv::Scalar(0, 0, 255), 2, 8, 0);
    }

    return hist_image;
}