#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
  unsigned char data[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

  Mat src = Mat(17, 18, CV_8UC1, data);
  cout << "Original matrix:" << endl
       << src << endl
       << endl;

  Mat kernels[] = {
    (Mat_<signed char>(3, 3) << -1, -1, -1, 0, 1, 0, 1, 1, 1), // B1
    (Mat_<signed char>(3, 3) << 0, -1, -1, 1, 1, -1, 1, 1, 0), // B2
    (Mat_<signed char>(3, 3) << 1, 0, -1, 1, 1, -1, 1, 0, -1), // B3
    (Mat_<signed char>(3, 3) << 1, 1, 0, 1, 1, -1, 0, -1, -1), // B4
    (Mat_<signed char>(3, 3) << 1, 1, 1, 0, 1, 0, -1, -1, -1), // B5
    (Mat_<signed char>(3, 3) << 0, 1, 1, -1, 1, 1, -1, -1, 0), // B6
    (Mat_<signed char>(3, 3) << -1, 0, 1, -1, 1, 1, -1, 0, 1), // B7
    (Mat_<signed char>(3, 3) << -1, -1, 0, -1, 1, 1, 0, 1, 1)  // B8
  };

  Mat dst(src.size(), CV_8UC1, Scalar(0));

  for (int i = 0; i < 8; ++i) {
    morphologyEx(src, dst, MORPH_HITMISS, kernels[i]);
    src = src - dst;
  }

  cout << "Matrix after thinning:" << endl << src << endl;

  Mat thinnedOutput;
  resize(src * 255, thinnedOutput, Size(), 32, 32, INTER_NEAREST);
  imshow("Thinned", thinnedOutput);
  
  waitKey(0);

  return 0;
}
