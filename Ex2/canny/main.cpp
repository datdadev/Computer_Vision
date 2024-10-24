#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    unsigned char data[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 255, 255, 255, 0, 0, 0,
        0, 255, 255, 255, 255, 0, 0,
        0, 255, 255, 255, 255, 255, 0,
        0, 255, 255, 255, 255, 0, 0,
        0, 255, 255, 255, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };

    Mat src(7, 7, CV_8UC1, data);
    cout << "Original matrix:" << endl;
    cout << src << endl
         << endl;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    double max_area = 0;
    int max_contour_index = -1;

    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            max_contour_index = static_cast<int>(i);
        }
    }

    Mat dst = Mat::zeros(src.size(), CV_8UC1);
    drawContours(dst, contours, max_contour_index, Scalar(255, 255, 255), 1);

    cout << "Matrix with largest contour:" << endl;
    cout << dst << endl;

    // Resize images for display
    Mat resizedSrc, resizedDst;
    resize(src, resizedSrc, Size(), 32, 32, INTER_NEAREST);
    resize(dst, resizedDst, Size(), 32, 32, INTER_NEAREST);

    namedWindow("Original", WINDOW_NORMAL);
    namedWindow("Largest Contour", WINDOW_NORMAL);
    
    imshow("Original", resizedSrc);
    imshow("Largest Contour", resizedDst);

    waitKey(0);

    return 0;
}
