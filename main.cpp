//Shanranranran ~ sora o
//tonde kono machi o miwatasu no ~
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>
#define M_PI 3.14159265358979323846

using namespace cv;
using namespace std;


int main()
{
	Mat img, img2;
	string file;
	cout << "Enter the file name: ";
	cin >> file;
	img = imread(file, CV_LOAD_IMAGE_GRAYSCALE);

	if (!img.data) {// Check for invalid input
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	img.convertTo(img2, CV_64FC1); // uchar to double

	Mat blur = Mat::zeros(img2.size(), img2.type());

	double sigma, w, deno;
	double kernel[7];
	cout << "Please Enter the Value of Sigma: ";
	cin >> sigma;
	deno = sigma * sqrt(2 * M_PI);
	for (int x = 3, k = 0; x >= 0; x--, k++) { // 1D kernel
		w = exp((-(x * x)) / (2 * (sigma * sigma))) / deno;
		kernel[k] = w;
		kernel[6 - k] = w;
	}

	imshow("Origin", img);

	for (int y = 0; y < img.cols; y++) {
		for (int x = 3; x < img.rows - 3; x++) { // do row Gaussian
			blur.at<double>(y, x) = img2.at<double>(y, x - 3) * kernel[0] + img2.at<double>(y, x - 2) * kernel[1] + img2.at<double>(y, x - 1) * kernel[2] + img2.at<double>(y, x) * kernel[3]
				+ img2.at<double>(y, x + 1) * kernel[4] + img2.at<double>(y, x + 2) * kernel[5] + img2.at<double>(y, x + 3) * kernel[6];
		}
		for (int x = 0; x < 3; x++) {
			blur.at<double>(y, x) = img2.at<double>(y, x);
			blur.at<double>(y, img.rows - 1 - x) = img2.at<double>(y, img.rows - 1 - x);
		}
	}

	img2 = blur;

	for (int x = 0; x < img.rows; x++) { // do col Gaussian
		for (int y = 3; y < img.cols - 3; y++) {
			blur.at<double>(y, x) = img2.at<double>(y - 3, x) * kernel[0] + img2.at<double>(y - 2, x) * kernel[1] + img2.at<double>(y - 1, x) * kernel[2] + img2.at<double>(y, x) * kernel[3]
				+ img2.at<double>(y + 1, x) * kernel[4] + img2.at<double>(y + 2, x) * kernel[5] + img2.at<double>(y + 3, x) * kernel[6];
		}
	}

	blur.convertTo(blur, CV_8UC1); // double to uchar

	imshow("Gaussian Blur", blur);
	waitKey(0);

	imwrite("output.jpg", blur);

	return 0;
}