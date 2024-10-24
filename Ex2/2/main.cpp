#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat binary, final, blend;
int alpha_slider = 75;
const int alpha_slider_max = 100;

void on_trackbar(int, void*) {
    double alpha = static_cast<double>(alpha_slider) / alpha_slider_max;
    double beta = 1.0 - alpha;
    
    Mat binary_color;
    cvtColor(binary, binary_color, COLOR_GRAY2BGR);
    
    addWeighted(binary_color, beta, final, alpha, 0.0, blend);
    
    imshow("Blended Image", blend);
}

int main() {
    Mat src = imread("../image.jpg", IMREAD_GRAYSCALE);

    double threshValue = 180.0;
    threshold(src, binary, threshValue, 255, THRESH_BINARY_INV);

    Mat kernel = (Mat_<signed char>(9, 9) << 
        -1,  1,  1,  1,  1,  1,  1,  1,  1,
        -1, -1,  1,  1,  1,  1,  1,  1,  1,
        -1, -1, -1,  1,  1,  1,  1,  1,  1,
        -1, -1, -1, -1,  1,  1,  1,  1,  1,
        -1, -1, -1, -1, -1,  1,  1,  1,  1,
        -1, -1, -1, -1, -1,  1,  1,  1,  1,
        -1, -1, -1, -1,  1,  1,  1,  1,  1,
        -1, -1, -1,  1,  1,  1,  1,  1,  1,
        -1, -1,  1,  1,  1,  1,  1,  1,  1
    );

    Mat hitMiss;
    morphologyEx(binary, hitMiss, MORPH_HITMISS, kernel);

    hitMiss *= 255;
    cvtColor(hitMiss, final, COLOR_GRAY2BGR);

    vector<Point> detectedPoints;
    for (int y = 0; y < hitMiss.rows; y++) {
        for (int x = 0; x < hitMiss.cols; x++) {
            if (hitMiss.at<uchar>(y, x) == 255) {
                detectedPoints.emplace_back(Point(x, y));
                circle(final, Point(x, y), 5, Scalar(0, 255, 0), 1);
                string coord = "(" + to_string(x) + "," + to_string(y) + ")";
                putText(final, coord, Point(x + 5, y - 5), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 255, 0), 1);
            }
        }
    }

    cv::imwrite("output_image_threshold.jpg", binary);

    namedWindow("Blended Image", WINDOW_AUTOSIZE);
    createTrackbar("Alpha", "Blended Image", &alpha_slider, alpha_slider_max, on_trackbar);

    on_trackbar(alpha_slider, 0);

    waitKey(0);
    
    return 0;
}










// #include <opencv2/opencv.hpp>
// #include <iostream>

// int main() {
//     // Load the image
//     cv::Mat img = cv::imread("../2.jpg");

//     // Check if the image was loaded
//     if (img.empty()) {
//         std::cerr << "Error: Unable to load image!" << std::endl;
//         return -1;
//     }

//     // Convert the image to grayscale
//     cv::Mat grayImg;
//     cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);

//     // Apply a binary threshold
//     cv::Mat threshImg;
//     double thresholdValue = 180.0; // Adjust this value as needed
//     cv::threshold(grayImg, threshImg, thresholdValue, 255, cv::THRESH_BINARY_INV);

//     // Save the thresholded image
//     cv::imwrite("output_image_threshold.jpg", threshImg);

//     return 0;
// }
