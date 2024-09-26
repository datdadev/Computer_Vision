#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>

using namespace cv;
using namespace std;

void white_pixels_counter(const char *path) {
  Mat img = cv::imread(path, IMREAD_GRAYSCALE);
  if (img.empty()) {
    cerr << "Error: Image not found at " << path << endl;
    return;
  }

  Point center(img.cols / 2, img.rows / 2);

  const char *outputDir = "./result";
  mkdir(outputDir, 0777);

  string imagePath(path);

  size_t lastSlash = imagePath.find_last_of("/");
  size_t lastDot = imagePath.find_last_of(".");

  string filename = imagePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
  string outputFileName = string(outputDir) + "/" + filename + ".txt";

  ofstream outFile(outputFileName);

  int radii[] = {1, 5, 10, 15, 20, 25, 30, 35, 40};
  int numRadii = sizeof(radii) / sizeof(radii[0]);

  for (int i = 0; i < numRadii; i++) {
    int count = 0;

    // Create a mask for the circle
    Mat mask = Mat::zeros(img.size(), CV_8U);
    circle(mask, center, radii[i], Scalar(255), -1);

    // Count white pixels
    for (int y = 0; y < img.rows; ++y) {
      for (int x = 0; x < img.cols; ++x) {
        if (mask.at<uchar>(y, x) == 255 && img.at<uchar>(y, x) == 255)
          count++;
      }
    }

    outFile << "Radius: " << setw(2) << radii[i]
            << " - White pixels: " << setw(3) << count << endl;
  }

  outFile.close();
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <image_path1> <image_path2> ..." << endl;
    return -1;
  }

  for (int i = 1; i < argc; i++)
    white_pixels_counter(argv[i]);

  return 0;
}
