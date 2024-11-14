#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

using namespace cv;
using namespace std;

// Function to perform 8-connectivity flood fill
void floodFill8Connect(const Mat &src, Mat &visited, int x, int y, vector<Point> &component) {
  stack<Point> stack;
  stack.push(Point(x, y));
  visited.at<uchar>(y, x) = 1;

  // Define the relative positions of the 8 neighbors
  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  while (!stack.empty()) {
    Point p = stack.top();
    stack.pop();
    component.push_back(p);

    for (int k = 0; k < 8; k++) {
      int nx = p.x + dx[k];
      int ny = p.y + dy[k];

      if (nx >= 0 && nx < src.cols && ny >= 0 && ny < src.rows) {
        if (src.at<uchar>(ny, nx) == 1 && visited.at<uchar>(ny, nx) == 0) {
          stack.push(Point(nx, ny));
          visited.at<uchar>(ny, nx) = 1;
        }
      }
    }
  }
}

// Calculate moments for a given component
Moments calculateMoments(const vector<Point> &component) {
  Moments m;
  for (const auto &p : component) {
    m.m00 += 1;
    m.m10 += p.x;
    m.m01 += p.y;
    m.m20 += p.x * p.x;
    m.m02 += p.y * p.y;
    m.m11 += p.x * p.y;
  }
  return m;
}

// Calculate and display Hu Moments for each component
void calculateHuMoments(const Mat &component) {
  Moments m = moments(component, true);

  double hu[7];
  HuMoments(m, hu);

  cout << "Hu Moments: \n";
  for (int i = 0; i < 7; i++) {
    cout << "hu[" << i << "]: " << hu[i] << "\n";
  }

  cout << endl;
}

int main() {
  unsigned char data[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

  Mat src = Mat(17, 18, CV_8UC1, data);
  Mat visited = Mat::zeros(src.size(), CV_8UC1);

  int componentCount = 0;

  for (int y = 0; y < src.rows; y++) {
    for (int x = 0; x < src.cols; x++) {
      if (src.at<uchar>(y, x) == 1 && visited.at<uchar>(y, x) == 0) {
        vector<Point> component;
        floodFill8Connect(src, visited, x, y, component);

        Moments mu = calculateMoments(component);
        Point2f center(mu.m10 / mu.m00, mu.m01 / mu.m00);

        cout << "Blob " << componentCount++ << ": Center (" << center.x << ", " << center.y << ")" << endl;

        double u11 = mu.m11 - (mu.m10 * mu.m01) / mu.m00;
        double u20 = mu.m20 - (mu.m10 * mu.m10) / mu.m00;
        double u02 = mu.m02 - (mu.m01 * mu.m01) / mu.m00;
        double theta = 0.5 * atan2(2.0 * u11, u20 - u02) * 180 / CV_PI;

        cout << "Orientation Theta: " << theta << " degrees" << endl;

        Mat componentMat = Mat::zeros(src.size(), CV_8UC1);
        for (const auto &point : component) {
          componentMat.at<uchar>(point.y, point.x) = 255;
        }

        cout << componentMat / 255 << endl;

        calculateHuMoments(componentMat);
      }
    }
  }

  return 0;
}
