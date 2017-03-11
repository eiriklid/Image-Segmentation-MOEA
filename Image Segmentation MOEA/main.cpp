#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>

#include "Segment.h"

using namespace cv;
using namespace std;





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

	/*
	cv::Scalar a(200, 200, 200); // color 1
	cv::Scalar b(100, 100, 100); // color 2

	cv::Vec4d d = a - b;  
	double distance = cv::norm(d);
	

	Point2i a = Point2i(1, 1);
	Point2i b = Point2i(2, 2);
	Point2i c = Point2i(3, 3);
	*/

	Segment test_seg;

	test_seg.insert_pixel(1, 1);
	test_seg.insert_pixel(1, 2);
	test_seg.insert_pixel(2, 2);
	test_seg.insert_pixel(2, 1);

	test_seg.erase_pixel(1, 2);

	test_seg.print();

	waitKey(0);                                          // Wait for a keystroke in the window

	
	

	return 0;
}