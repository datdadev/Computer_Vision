#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat img1, img2, diff, mask;
Mat finalResult;
int thresholdValue = 23;
const int maxThreshold = 255;
bool applyBlur = true;
bool showContourMask = true;

void applyLargestContourMask() {
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  // Find the largest contour by area
  double maxArea = 0;
  int maxIndex = -1;
  for (size_t i = 0; i < contours.size(); i++) {
    double area = contourArea(contours[i]);
    if (area > maxArea) {
      maxArea = area;
      maxIndex = static_cast<int>(i);
    }
  }

  // Create a mask for the largest contour
  Mat contourMask = Mat::zeros(mask.size(), CV_8UC1);
  if (maxIndex != -1) {
    drawContours(contourMask, contours, maxIndex, Scalar(255), FILLED);
  }

  // Apply the contour mask on the original mask
  bitwise_and(mask, contourMask, finalResult);

  imshow("Final Image", finalResult);
}

void updateMask() {
  Mat processedDiff = diff.clone();
  if (applyBlur) {
    GaussianBlur(diff, processedDiff, Size(3, 3), 3);
  }

  // Update the mask based on the current threshold value
  for (int i = 0; i < processedDiff.rows; i++) {
    for (int j = 0; j < processedDiff.cols; j++) {
      Vec3b pixel = processedDiff.at<Vec3b>(i, j);
      double dist =
          sqrt(pixel[0] * pixel[0] + pixel[1] * pixel[1] + pixel[2] * pixel[2]);
      mask.at<unsigned char>(i, j) = (dist > thresholdValue) ? 255 : 0;
    }
  }
  imshow("Final Image", mask);

  if (showContourMask) {
    applyLargestContourMask();
  }
}

int main() {
  img1 = imread("../3_1.png");
  img2 = imread("../3_2.png");

  // Resize img2 to match the size of img1
  if (img1.size() != img2.size()) {
    resize(img2, img2, img1.size());
  }

  absdiff(img1, img2, diff);
  mask = Mat::zeros(diff.rows, diff.cols, CV_8UC1);
  finalResult = Mat::zeros(diff.rows, diff.cols, CV_8UC1);

  namedWindow("Final Image");
  createTrackbar("Threshold", "Final Image", &thresholdValue, maxThreshold,
                 [](int, void *) { updateMask(); });

  // Initialize the mask once with the initial threshold value
  updateMask();

  imshow("Background Image", img1);
  imshow("Current Image", img2);
  imshow("Difference Image", diff);

  while (true) {
    int key = waitKey(30);
    if (key == 27) {
      break;
    } else if (key == 'b' || key == 'B') { // Toggle blur
      applyBlur = !applyBlur;
      updateMask();
    } else if (key == 'c' ||
               key == 'C') { // Toggle showing largest contour mask
      showContourMask = !showContourMask;
      updateMask();
    }
  }

  return 0;
}
