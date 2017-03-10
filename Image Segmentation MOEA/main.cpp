#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>


using namespace cv;
using namespace std;


namespace std {
	template<> struct hash<Point> {
		size_t operator()(const Point &pt) const {
			return std::hash<int>()(pt.x) ^ std::hash<int>()(pt.y);
		}
	};
}

typedef std::unordered_set <cv::Point2i> points_set_t;





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
	*/

	Point2i a = Point2i(0, 0);
	Point2i b = Point2i(1, 1);
	Point2i c = Point2i(2, 2);

	points_set_t points;

	points.insert(a);
	points.insert(b);
	points.insert(c);

	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		std::cout << "(" << it->x << "," << it->y << ")" << std::endl;
	}


	waitKey(0);                                          // Wait for a keystroke in the window

	
	

	return 0;
}