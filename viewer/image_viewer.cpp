#include <algorithm>
#include <unordered_set>

#include "image_viewer.h"

using namespace std;
using namespace cv;

static const unordered_set<string_view> available_extensions{
        ".jpg", ".jpeg", ".png"
};

static bool is_not_image_file(const string &filename) {
    if (int ind = filename.rfind('.'); ind > 0) {
        return available_extensions.find(string_view{filename.data() + ind}) == available_extensions.end();
    }
    return true;
}

vector<string> list_images(const string &path_to_directory) {
    vector<string> result{};
    glob(path_to_directory, result);
    result.erase(remove_if(result.begin(), result.end(), is_not_image_file), result.end());
    return result;
}

void make_histogram_image(const Mat &image, Mat &hist_image) {
    int hist_size = 256;
    float range[] = {0.0, 256.0};
    const float *hist_range = {range};

    vector<Mat> bgr_image;
    split(image, bgr_image);

    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_image[0], 1, 0, Mat(), b_hist, 1, &hist_size, &hist_range, true, false);
    calcHist(&bgr_image[1], 1, 0, Mat(), g_hist, 1, &hist_size, &hist_range, true, false);
    calcHist(&bgr_image[2], 1, 0, Mat(), r_hist, 1, &hist_size, &hist_range, true, false);

    int hist_w = hist_image.cols, hist_h = hist_image.rows;
    int bin_w = cvRound((double) hist_w / hist_size);

    normalize(b_hist, b_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < hist_size; i++) {
        line(hist_image, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
        line(hist_image, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);
        line(hist_image, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }
}