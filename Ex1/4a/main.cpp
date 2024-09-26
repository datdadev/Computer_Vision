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

  Mat blur;
  GaussianBlur(
      mat, blur, Size(3, 3),
      1.2, 1.2, BORDER_REPLICATE
  );

  cout << "Matrix before Gaussian blurring" << endl
       << mat << endl
       << endl;

  cout << "Matrix after Gaussian blurring:" << endl
       << blur << endl
       << endl;

  return 0;
}
