#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
  Mat input = (Mat_<float>(17, 18) <<
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  input.convertTo(input, CV_8UC1);
  threshold(input, input, 0, 255, THRESH_BINARY);

  vector<Vec2f> lines;
  HoughLines(input, lines, 0.001, CV_PI / 4, 4);
  cout << "Number of lines: " << lines.size() << endl;

  for (const auto &line : lines) {
    float rho = line[0], theta = line[1];
    if (sin(theta) == 0) {
      double x_intercept = rho / cos(theta);
      cout << "x = " << round(x_intercept) << endl;
    }
    else {
      double slope = -cos(theta) / sin(theta);
      double y_intercept = rho / sin(theta);
      if (round(slope) != 0)
        cout << "y = " << round(slope) << "x + " << round(y_intercept) << endl;
      else
        cout << "y = " << round(y_intercept) << endl;
    }
  }

  cout << endl;

  vector<Point> intersection_points;

  for (size_t i = 0; i < lines.size() - 1; i++) {
    float rho1 = lines[i][0], theta1 = lines[i][1];
    float a1 = cos(theta1), b1 = sin(theta1);
    float x1 = a1 * rho1, y1 = b1 * rho1;

    for (size_t j = i + 1; j < lines.size(); j++) {
      float rho2 = lines[j][0], theta2 = lines[j][1];
      float a2 = cos(theta2), b2 = sin(theta2);
      float x2 = a2 * rho2, y2 = b2 * rho2;
      if (-a2 * b1 + a1 * b2 != 0) {
        float t2 = (b1 * (y1 - y2) + a1 * (x1 - x2)) / (-a2 * b1 + a1 * b2);
        Point intersection_point;
        intersection_point.x = cvRound(x2 + b2 * t2);
        intersection_point.y = cvRound(y2 - a2 * t2);
        intersection_points.push_back(intersection_point);
      }
    }
  }

  cout << "Intersection points: { ";
  for (size_t i = 0; i < intersection_points.size(); i++) {
    cout << intersection_points[i];
    if (i < intersection_points.size() - 1) {
      cout << ", ";
    }
  }
  cout << " }" << endl;

  return 0;
}
