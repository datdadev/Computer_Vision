#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include <vector>

using namespace cv;
using namespace std;

// Epsilon to avoid division by zero
const double EPSILON = 1e-6;

vector<int> white_pixels_counter(const char *path, const vector<int> &radii) {
  Mat img = cv::imread(path, IMREAD_GRAYSCALE);
  if (img.empty()) {
    cerr << "Error: Image not found at " << path << endl;
    return {};
  }

  vector<int> whitePixelCounts;

  Point center(img.cols / 2, img.rows / 2);

  for (int r : radii) {
    int count = 0;

    Mat mask = Mat::zeros(img.size(), CV_8U);
    circle(mask, center, r, Scalar(255), -1);

    for (int y = 0; y < img.rows; ++y) {
      for (int x = 0; x < img.cols; ++x) {
        if (mask.at<uchar>(y, x) == 255 && img.at<uchar>(y, x) == 255)
          count++;
      }
    }

    // Save the white pixel count for this radius
    whitePixelCounts.push_back(count);
  }

  return whitePixelCounts;
}

// Function to calculate the correlation error between two images' white pixel
// counts
double calculate_error(const vector<int> &countsX, const vector<int> &countsY) {
  int n = countsX.size();
  double errorSum = 0;

  for (int i = 0; i < n; i++) {
    double numX = static_cast<double>(countsX[i]);
    double numY = static_cast<double>(countsY[i]);

    if (numX == 0 && numY == 0) {
      continue;
    }

    if (numY == 0) {
      errorSum += 1.0;
    } else {
      double ratio = numX / (numY + EPSILON);
      double diff = 1 - ratio;
      errorSum += diff * diff;
    }
  }

  return errorSum / n;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <image_path1> <image_path2> ..." << endl;
    return -1;
  }

  vector<int> radii = {1,  5,  10, 15, 20, 25, 30, 35, 40, 45,
                       50, 60, 65, 70, 75, 80, 85, 90, 95, 100};

  // Store the white pixel counts for all images
  vector<vector<int>> allCounts;

  for (int i = 1; i < argc; i++) {
    vector<int> counts = white_pixels_counter(argv[i], radii);
    if (!counts.empty()) {
      allCounts.push_back(counts);
    }
  }

  // Compare each pair of images and compute the error and correlation
  int numImages = allCounts.size();
  for (int i = 0; i < numImages; i++) {
    for (int j = i + 1; j < numImages; j++) {
      double error = calculate_error(allCounts[i], allCounts[j]);
      double correlation = 1 - error;

      cout << "Error between image " << i + 1 << " and image " << j + 1
           << " is " << error << endl;
      cout << "Correlation between image " << i + 1 << " and image " << j + 1
           << " is " << correlation << endl;

      if (correlation > 0.9) {
        cout << "Images " << i + 1 << " and " << j + 1
             << " are similar (correlation > 0.9)." << endl;
      } else {
        cout << "Images " << i + 1 << " and " << j + 1
             << " are not similar (correlation <= 0.9)." << endl;
      }

      cout << endl;
    }
  }

  return 0;
}
