#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>

#include "Segment.h"

using namespace cv;
using namespace std;

double color_distance(cv::Vec3b a, cv::Vec3b b);
cv::Point2i* neighbours(cv::Point2i pixel, cv::Mat& image);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.

	
	cv::Vec3d a(200, 200, 200); // color 1
	cv::Vec3d b(100, 100, 100); // color 2
	cout << color_distance(a, b) << endl;
	

	Segment test_seg;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			Point2i c = Point2i(i,j);
			test_seg.insert_pixel(c);
			image.at<Vec3b>(c)[0] = 0;
			image.at<Vec3b>(c)[1] = 255;
			image.at<Vec3b>(c)[2] = 0;


		}
	}
	test_seg.print();

	/*
	Vec3b s = image.at<Vec3b>(c.x,c.y);
	cout << s << image.cols << endl;
	Point2i* ne = neighbours(c, image);

	

	for (int i = 0; i < 4; i++) {
		cout << ne[i] << "\t";
	}

	*/

	namedWindow("Green window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Green window", image);                   // Show our image inside it.


	waitKey(0);                                          // Wait for a keystroke in the window

	return 0;
}


double color_distance(cv::Vec3b a, cv::Vec3b b) {
	cv::Vec3b d = a - b;
	return cv::norm(d);
}

cv::Point2i* neighbours(cv::Point2i pixel, cv::Mat& image) {
	cv::Point2i* neighbour_array = new cv::Point2i [4];
	
	neighbour_array[0] = ((pixel.x > 0) ? (cv::Point2i(pixel.x - 1, pixel.y)) : (cv::Point2i(-1,-1)));
	neighbour_array[1] = ((pixel.y > 0) ? (cv::Point2i(pixel.x, pixel.y - 1)) : (cv::Point2i(-1, -1)));
	neighbour_array[2] = ((pixel.x < image.cols - 1) ? (cv::Point2i(pixel.x + 1, pixel.y)) : (cv::Point2i(-1, -1))); 
	neighbour_array[3] = ((pixel.y < image.rows - 1) ? (cv::Point2i(pixel.x, pixel.y + 1)) : (cv::Point2i(-1, -1))); 

	return neighbour_array;
	
}