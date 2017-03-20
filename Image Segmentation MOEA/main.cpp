#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unordered_set>
#include <string>

#include "Segment.h"
#include "pixel_functions.h"
#include "Solution.h"
#include "NSGA_II.h"

using namespace cv;
using namespace std;

int main()
{
	srand(time(NULL));
	Mat image;
	image = imread(FILENAME, CV_LOAD_IMAGE_COLOR);   // Read the file

	/*cout << "Rows: " << image.rows <<  "\nColums " << image.cols << "\nChannels " << image.channels() << endl;
	cout << "continous? " << image.isContinuous() << endl;

	uchar* imagePtr = image.ptr<uchar>(0);
	cout << (int)*(imagePtr + image.rows*image.cols*image.channels()-1);
	
	system("PAUSE");*/



	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	/*
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.
	waitKey(0);                                          // Wait for a keystroke in the window
	*/

	NSGA_II(&image);

	/*
	RGB a(200, 200, 200); // color 1
	RGB b(100, 100, 100); // color 2
	cout << color_distance(a, b) << endl;
	*/
	
	/*
	Solution sol = Solution(&image);
	seg_vec_t segments = sol.segments;
	for (seg_vec_t::iterator seg_it = segments.begin(); seg_it != segments.end(); seg_it++) {

		points_set_t edge;
		seg_it->get_edge(&edge);
		for (points_set_t::const_iterator it = edge.begin(); it != edge.end(); it++) {
			//cout << *it << endl;
			image.at<Vec3b>(*it)[0] = 0;
			image.at<Vec3b>(*it)[1] = 255;
			image.at<Vec3b>(*it)[2] = 0;
		}
	}
	namedWindow("Green window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Green window", image);                   // Show our image inside it.
	waitKey(0);                                          // Wait for a keystroke in the window
	*/
	return 0;
}


