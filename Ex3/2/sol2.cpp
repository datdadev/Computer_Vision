#include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main() {
  int y[2];
  y[0] = 220;
  y[1] = 470;
  int x1, x2;
  int text_no = 1;

  // Import RGB source.
  Mat src = imread("./image.jpg");
  if (src.empty()) {
    cerr << "Error: Image not found!" << endl;
    return -1;
  }

  // Import Grayscale source and threshold it.
  Mat src_gray = imread("./image.jpg", IMREAD_GRAYSCALE);
  threshold(src_gray, src_gray, 170, 255, 0);

  // Canny matrix for HoughLines() operation at "a" requirement.
  Mat canny;
  Canny(src_gray, canny, 150, 210);
  vector<Vec2f> lines;
  HoughLines(canny, lines, 1.6, CV_PI / 180, 320);

  // Second Canny matrix for cars detection at "b" requirement.
  Mat canny_0;
  Canny(src, canny_0, 150, 200);
  Mat kernel = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(-1, -1));
  dilate(canny_0, canny_0, kernel);
  erode(canny_0, canny_0, kernel);
  threshold(canny_0, canny_0, 200, 200, 0);

  Mat free_mat(src.rows, src.cols, CV_8UC1, Scalar(0));

  // Translate polar coordinates into Oxy coordinates and draw lines of the parking lot.
  for (int i = 0; i < lines.size(); i++) {
    float r = lines[i][0], theta = lines[i][1];
    double a = cos(theta);
    double b = sin(theta);

    // x0 and y0 store r*cos(theta) and r*sin(theta), respectively
    double x0 = a * r;
    double y0 = b * r;

    // x1, y1, x2, y2 define the first point and the end point of the line segment
    int x1 = int(x0 + 1000 * (-b));
    int y1 = int(y0 + 1000 * (a));
    int x2 = int(x0 - 1000 * (-b));
    int y2 = int(y0 - 1000 * (a));

    // Draw lines on the original image and grayscale lines on second Canny matrix.
    cv::line(src, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255),
             2);
    cv::line(free_mat, cv::Point(x1, y1), cv::Point(x2, y2), 255, 1);
    cv::line(canny_0, cv::Point(x1, y1), cv::Point(x2, y2), 255, 1);
  }

  imshow("Lines detected", src);

  Mat src_b;
  src.copyTo(src_b);

  for (int n = 0; n < 2; n++) {
    for (int i = 0; i < free_mat.cols; i++) {
      if (int(free_mat.at<uchar>(y[n], i) == 255)) {
        for (int i2 = i + 1; i2 < free_mat.cols; i2++) {
          if (i2 == (free_mat.cols - 1))
            break;
          if (int(free_mat.at<uchar>(y[n], i2) == 255) && (i2 - i < 20))
            break;
          if (int(free_mat.at<uchar>(y[n], i2) == 255)) {
            rectangle(src, cv::Point(i, y[n] - 100), cv::Point(i2, y[n] + 100),
                      cv::Scalar(0, 255, 0), 2);
            ostringstream oss;
            oss << "Space no. " << text_no;
            string text = oss.str();
            putText(src, text, cv::Point(i, y[n] - 105), FONT_HERSHEY_SIMPLEX,
                    0.4, cv::Scalar(0, 255, 0), 1, LINE_AA);
            text_no++;
            break;
          }
        }
      }
    }
  }

  imshow("Lines and spaces detected with HoughLines", src);

  // Declare variables for b requirements.
  int count_edge;
  int count_dist;
  int thres_dist = 20;
  int thres_egde = 10;
  x1 = 0;
  x2 = 0;
  text_no = 1;

  // Find free parking spaces based on the density of edges in second Canny matrix for each slot.
  for (int n = 0; n < 2; n++) {
    for (int i = 0; i < canny_0.cols; i++) {
      if (int(canny_0.at<uchar>(y[n], i) == 255)) {
        count_dist = 0;
        count_edge = 0;
        x1 = i;
        for (int i2 = i + 1; i2 < canny_0.cols; i2++) {
          if (int(canny_0.at<uchar>(y[n], i2) == 200))
            count_edge++;
          count_dist++;
          if (i2 == (canny_0.cols - 1)) {
            count_dist = 0;
            break;
          }
          if (int(canny_0.at<uchar>(y[n], i2) == 255)) {
            x2 = i2;
            break;
          }
        }
        if (count_dist > thres_dist) {
          if (count_edge < thres_egde) {
            rectangle(src_b, cv::Point(x1, y[n] - 100),
                      cv::Point(x2, y[n] + 100), cv::Scalar(0, 255, 0), 2);
            ostringstream oss;
            oss << "Free space no. " << text_no;
            string text = oss.str();
            putText(src_b, text, cv::Point(x1, y[n] - 105),
                    FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1,
                    LINE_AA);
            text_no++;
          }
        }
      }
    }
  }

  imshow("Free parking lot detected.", src_b);

  waitKey(0);
}
