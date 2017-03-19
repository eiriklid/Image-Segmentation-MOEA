#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>

#include "Segment.h"
#include "pixel_functions.h"
#include "Solution.h"
#include "NSGA_II.h"

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

	NSGA_II(&image);

	//Solution sol = Solution(&image);
	/*
	cv::Vec3d a(200, 200, 200); // color 1
	cv::Vec3d b(100, 100, 100); // color 2
	cout << color_distance(a, b) << endl;
	*/
	/*
	Segment a_seg(&image);
	Segment b_seg(&image);

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 32; j++) {
			a_seg.insert_pixel(Point2i(i, j));
			b_seg.insert_pixel(Point2i(i + 31, j));
		}
	}
	cout << a_seg.neighbour(b_seg);
	Segment test_seg(&image, a_seg, b_seg);
	//cout << test_seg.average();
	//test_seg.print();
	/*
	Point2i p = Point2i(0, 0);
	point_vec_t neighbour_vec = neighbours(p, &image,1);

	for (point_vec_t::iterator it = neighbour_vec.begin(); it != neighbour_vec.end(); it++) {
		cout << *it << endl;
	}
	
	//cout << test_seg.overall_deviation();
	//cout << test_seg.conectivity_measure();

	points_set_t edge = test_seg.get_edge();
	for (points_set_t::const_iterator it = edge.begin(); it != edge.end(); it++) {
		//cout << *it << endl;
		image.at<Vec3b>(*it)[0] = 0;
		image.at<Vec3b>(*it)[1] = 255;
		image.at<Vec3b>(*it)[2] = 0;
	}
	*/
	/*seg_vec_t segments = sol.get_segments();
	for (seg_vec_t::iterator seg_it = segments.begin(); seg_it != segments.end(); seg_it++) {

		points_set_t edge = seg_it->get_edge();
		for (points_set_t::const_iterator it = edge.begin(); it != edge.end(); it++) {
			//cout << *it << endl;
			image.at<Vec3b>(*it)[0] = 0;
			image.at<Vec3b>(*it)[1] = 255;
			image.at<Vec3b>(*it)[2] = 0;
		}
	}
	namedWindow("Green window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Green window", image);                   // Show our image inside it.*/
	
	waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}


