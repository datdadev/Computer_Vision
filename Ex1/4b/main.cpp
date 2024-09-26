#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
  float id = 12;
  Mat mat = (Mat_<float>(5, 7) << 
    9, 10, 11, 10, 9, 10, 11,
    10, 9, 80 + id * 5, 80 + id * 5, 80 + id * 5, 10, 11,
    10, 80 + id * 5, 10, 11, 10, 80 + id * 5, 11,
    10, 9, 80 + id * 5, 80 + id * 5, 80 + id * 5, 13, 11,
    10, 10, 10, 10, 10, 13, 11
  );

  Mat sobel_x, sobel_y;
  Sobel(mat, sobel_x, CV_32F,
      1, 0, 3, 1, 0, BORDER_REPLICATE);
  Sobel(mat, sobel_y, CV_32F,
      0, 1, 3, 1, 0, BORDER_REPLICATE);

  cout << "Matrix after Sobel edge detection in X direction:" << endl
       << sobel_x << endl
       << endl;

  cout << "Matrix after Sobel edge detection in Y direction" << endl
       << sobel_y << endl
       << endl;

  return 0;
}
