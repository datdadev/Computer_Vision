#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void drawHistogram(const string &windowName, const Mat &image) {
  int bins = 256;
  int histSize[] = {bins};

  float range[] = {0, 255};
  const float *ranges[] = {range};

  Mat hist;
  int channels[] = {0};
  int histHeight = 255;
  Mat histImage = Mat::zeros(histHeight, bins, CV_8UC3);

  // Compute the histogram
  calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);

  // Find the maximum value in the histogram
  double maxVal = 0;
  minMaxLoc(hist, 0, &maxVal);

  // Draw the histogram
  for (int i = 0; i < bins; i++) {
    float binValue = hist.at<float>(i);
    int height = cvRound(binValue * histHeight / maxVal);
    line(histImage, Point(i, histHeight - height), Point(i, histHeight),
         Scalar::all(255));
  }

  imshow(windowName, histImage);
}

int main() {
  float id = 12;
  float imgData[16] = {id * 10, 20,  30,      id * 10, 80,  id * 10, 100, 110,
                       120,     160, id * 10, 150,     220, 230,     240, 250};

  Mat originalImage = Mat(4, 4, CV_32F, imgData);
  Mat equalizedImage;

  originalImage.convertTo(originalImage, CV_8UC1);

  // Apply histogram equalization
  equalizeHist(originalImage, equalizedImage);

  // Output the matrices
  cout << "Image before histogram equalization:" << endl
       << originalImage << endl
       << endl;
  cout << "Image after histogram equalization:" << endl
       << equalizedImage << endl
       << endl;

  namedWindow("Original Image", WINDOW_NORMAL | WINDOW_OPENGL);
  namedWindow("Equalized Image", WINDOW_NORMAL | WINDOW_OPENGL);

  Mat resizedOriginal, resizedEqualized;
  resize(originalImage, resizedOriginal, Size(), 64, 64, INTER_NEAREST);
  resize(equalizedImage, resizedEqualized, Size(), 64, 64, INTER_NEAREST);

  imshow("Original Image", resizedOriginal);
  imshow("Equalized Image", resizedEqualized);

  drawHistogram("Original Image Histogram", originalImage);
  drawHistogram("Equalized Image Histogram", equalizedImage);

  waitKey();
  return 0;
}
