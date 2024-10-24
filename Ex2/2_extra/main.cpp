#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>

using namespace cv;
using namespace std;

int main()
{
  Mat src = imread("../image.jpg", IMREAD_GRAYSCALE);
  if (src.empty())
  {
    cerr << "Error: Image not found!" << endl;
    return -1;
  }

  Mat dst1, dst_norm, dst2;
  int thresh = 111;
  GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_REPLICATE);
  dst1 = Mat::zeros(src.size(), CV_32FC1);
  dst_norm = Mat::zeros(src.size(), CV_32FC1);
  int n = 0, x1, y1;
  Scalar color(255);

  cornerHarris(src, dst1, 7, 5, 0.04, BORDER_REPLICATE);
  normalize(dst1, dst_norm, 0, 255, NORM_MINMAX);
  convertScaleAbs(dst_norm, dst2);

  for (int j = 0; j < dst_norm.rows; j++) {
    for (int i = 0; i < dst_norm.cols; i++) {
      if ((int)dst2.at<uchar>(j, i) == thresh && i < (dst2.cols / 2) && j > (dst2.rows / 3)) {
        n++;
        if (n == 2) {
          circle(dst2, Point(i, j), 5, color, 2, 8, 0);
          x1 = i;
          y1 = j;
        }
      }
    }
  }

  imshow("Interest corner 4", dst2);
  cout << "The image has a size of " << dst2.cols << " by " << dst2.rows << "." << endl;
  cout << "Corner 4 is located at x = " << x1 << " and y = " << y1 << endl;

  waitKey(0);
  destroyAllWindows();

  return 0;
}
