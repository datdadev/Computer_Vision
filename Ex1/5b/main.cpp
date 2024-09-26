#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

const float pi = 3.14;

int main() {
  Mat im_1 = imread("../5_1.png", IMREAD_GRAYSCALE);
  Mat im_2 = imread("../5_2.png", IMREAD_GRAYSCALE);
  Mat im_3 = imread("../5_3.png", IMREAD_GRAYSCALE);
  Mat im_4 = imread("../5_4.png", IMREAD_GRAYSCALE);
  ofstream myfile("result.txt");
  if (!myfile.is_open())
    cout << "Unable to open file";

  cv::Size common_size(200, 200);

  Mat resized_im_1, resized_im_2, resized_im_3, resized_im_4;

  resize(im_1, resized_im_1, common_size);
  resize(im_2, resized_im_2, common_size);
  resize(im_3, resized_im_3, common_size);
  resize(im_4, resized_im_4, common_size);

  Mat scan_im_1 = resized_im_1, scan_im_2 = resized_im_2,
      scan_im_3 = resized_im_3, scan_im_4 = resized_im_4;

  int r[9] = {1, 5, 10, 15, 20, 25, 30, 35, 40};
  int white_count[4][9];

  // Write data about Image 1
  myfile << "The number of white pixels in Image 1 is: \n";
  for (int i = 0; i < 9; i++) {
    int count = 0;
    for (float phi = 0; phi < (2 * pi); phi += 0.025) {
      if (resized_im_1.at<uint8_t>(100 - r[i] * cos(phi),
                                   100 - r[i] * sin(phi)) == 255)
        count++;
      scan_im_1.at<uint8_t>(100 - r[i] * cos(phi), 100 - r[i] * sin(phi)) = 255;
    }
    white_count[0][i] = count;
    myfile << "Radius " << r[i] << ":\t" << count << endl;
  }
  imshow("Scanned Image 1", scan_im_1);

  // Write data about Image 2
  myfile << "The number of white pixels in Image 2 is: \n";
  for (int i = 0; i < 9; i++) {
    int count = 0;
    for (float phi = 0; phi < (2 * pi); phi += 0.025) {
      if (resized_im_2.at<uint8_t>(100 - r[i] * cos(phi),
                                   100 - r[i] * sin(phi)) == 255)
        count++;
      scan_im_2.at<uint8_t>(100 - r[i] * cos(phi), 100 - r[i] * sin(phi)) = 255;
    }
    white_count[1][i] = count;
    myfile << "Radius " << r[i] << ":\t" << count << endl;
  }
  imshow("Scanned Image 2", scan_im_2);

  // Write data about Image 3
  myfile << "The number of white pixels in Image 3 is: \n";
  for (int i = 0; i < 9; i++) {
    int count = 0;
    for (float phi = 0; phi < (2 * pi); phi += 0.025) {
      if (resized_im_3.at<uint8_t>(100 - r[i] * cos(phi),
                                   100 - r[i] * sin(phi)) == 255)
        count++;
      scan_im_3.at<uint8_t>(100 - r[i] * cos(phi), 100 - r[i] * sin(phi)) = 255;
    }
    white_count[2][i] = count;
    myfile << "Radius " << r[i] << ":\t" << count << endl;
  }
  imshow("Scanned Image 3", scan_im_3);

  // Write data about Image 4
  myfile << "The number of white pixels in Image 4 is: \n";
  for (int i = 0; i < 9; i++) {
    int count = 0;
    for (float phi = 0; phi < (2 * pi); phi += 0.025) {
      if (resized_im_4.at<uint8_t>(100 - r[i] * cos(phi),
                                   100 - r[i] * sin(phi)) == 255)
        count++;
      scan_im_4.at<uint8_t>(100 - r[i] * cos(phi), 100 - r[i] * sin(phi)) = 255;
    }
    white_count[3][i] = count;
    myfile << "Radius " << r[i] << ":\t" << count << endl;
  }
  imshow("Scanned Image 4", scan_im_4);

  int n, m;
  cout << "Enter the 2 images you would like to compare: ";
  cin >> n;
  cin >> m;

  n--;
  m--;

  double error = 0;

  for (int i = 0; i < 9; i++) {
    error =
        error + ((1 - (double)white_count[n][i] / (double)white_count[m][i]) *
                 (1 - (double)white_count[n][i] / (double)white_count[m][i]));
  }
  error = error / 9.0;
  error = error * 100;
  cout << "The error between image " << n + 1 << " and " << m + 1
       << " is: " << error << "%." << endl;

  if (error < 0.5)
    cout << "The 2 images are the same." << endl;
  else
    cout << "The 2 images are not the same." << endl;

  waitKey(0);
  return 0;
}
