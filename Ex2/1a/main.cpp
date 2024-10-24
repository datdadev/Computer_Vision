#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

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
  cout << "Original Matrix:" << endl
       << src << endl
       << endl;

  // Harris corner detection
  Mat _dst1 = Mat::zeros(src.size(), CV_32FC1);
  int blockSize = 3;
  int apertureSize = 3;
  double k = 0.06;

  cornerHarris(src, _dst1, blockSize, apertureSize, k, BORDER_REPLICATE);
  normalize(_dst1, _dst1, 0, 255, NORM_MINMAX, -1, Mat());

  Mat dst1;
  threshold(_dst1, dst1, 250, 255, THRESH_BINARY);
  dst1.convertTo(dst1, CV_8UC1);

  cout << "Matrix after Harris Corner Detection: " << endl
       << dst1 << endl
       << endl;

  // Shi-Tomasi corner detection
  vector<Point2f> corners;
  int maxCorners = 10;
  double qualityLevel = 0.01;
  double minDistance = 1;
  Mat mask;
  bool useHarrisDetector = false;
  goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, minDistance, mask, blockSize, useHarrisDetector, k);

  Mat dst2 = Mat::zeros(src.size(), CV_8UC1);
  for (size_t i = 0; i < corners.size(); i++)
  {
    dst2.at<uchar>(static_cast<int>(corners[i].y), static_cast<int>(corners[i].x)) = 255;
  }

  cout << "Matrix after Shi-Tomasi Corner Detection: " << endl
       << dst2 << endl;

  // Resize images for display
  Mat resizedSrc, resizedDst1, resizedDst2;
  resize(src, resizedSrc, Size(), 32, 32, INTER_NEAREST);
  resize(dst1, resizedDst1, Size(), 32, 32, INTER_NEAREST);
  resize(dst2, resizedDst2, Size(), 32, 32, INTER_NEAREST);

  imshow("Original", resizedSrc);
  imshow("Harris Corners", resizedDst1);
  imshow("Shi-Tomasi Corners", resizedDst2);
  
  waitKey(0);

  return 0;
}
