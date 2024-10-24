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

    Mat src = Mat(7, 7, CV_8UC1, data);
    cout << "Original matrix:" << endl
         << src << endl
         << endl;

    Mat binary;
    threshold(src, binary, 127, 1, THRESH_BINARY);
    cout << "Original binary matrix:" << endl
         << binary << endl
         << endl;

    Mat kernels[] = {
      (Mat_<signed char>(3, 3) <<
        -1, -1,  0,
        -1,  1,  1,
         0,  1,  0
      ),
      (Mat_<signed char>(3, 3) <<
        -1, -1, -1,
        1,  1, -1,
        0,  1,  0
      ),
      (Mat_<signed char>(3, 3) <<
        0,  1,  0,
        -1,  1,  1,
        -1, -1,  0
      ),
      (Mat_<signed char>(3, 3) <<
        0, -1, -1,
        0,  1, -1,
        0, -1, -1
      )
    };

    Mat dst(src.size(), CV_8UC1, Scalar(0));

    // Loop over the kernels and apply hit-miss transform
    for (int i = 0; i < 4; ++i) {
        Mat output_image(src.size(), CV_8UC1, Scalar(0));
        morphologyEx(binary, output_image, MORPH_HITMISS, kernels[i]);

        // Combine results
        bitwise_or(output_image, dst, dst);
    }

    cout << "Hit-Miss corner detection matrix:" << endl
         << dst << endl
         << endl;

    // Resize images for display
    Mat resizedSrc, resizedDst;
    dst *= 255;
    resize(binary * 255, resizedSrc, Size(), 32, 32, INTER_NEAREST);
    resize(dst, resizedDst, Size(), 32, 32, INTER_NEAREST);

    imshow("Original", resizedSrc);
    imshow("Hit-Miss Corners", resizedDst);
    waitKey(0);

    return 0;
}
