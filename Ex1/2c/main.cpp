#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
  float id = 12;
  float imgData[16] = {
    id * 10, 20, 30, id * 10,
    80, id * 10, 100, 110,
    120, 160, id * 10, 150,
    220, 230, 240, 250
  };

  Mat inputImage(4, 4, CV_32F, imgData);
  Mat outputImage;

  Mat convertedImage;
  inputImage.convertTo(convertedImage, CV_8UC1);

  double thresholdValue =
      threshold(convertedImage, outputImage, 0, 255, THRESH_OTSU);

  cout << "Matrix before Otsu thresholding:" << endl
       << convertedImage << endl
       << endl;

  cout << "Otsu's threshold value: " << thresholdValue << endl << endl;

  cout << "Matrix before Otsu thresholding:" << endl
       << outputImage << endl
       << endl;

  return 0;
}
