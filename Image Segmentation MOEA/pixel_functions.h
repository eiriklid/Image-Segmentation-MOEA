#pragma once
#include <opencv2/core/core.hpp>
#include <vector>

struct RGB {
	uchar blue;
	uchar green;
	uchar red;
	RGB(uchar b, uchar g, uchar r) {
		blue = b;
		green = g;
		red = r;
	}
	RGB(){}
};


typedef std::vector<cv::Point2i> point_vec_t;
double color_distance(RGB a, RGB b);
double color_distance(cv::Point2i a, cv::Point2i b, cv::Mat *image);
point_vec_t neighbours(cv::Point2i pixel, cv::Mat* image); //Returns 4-neighbourhood
point_vec_t neighbours(cv::Point2i pixel, cv::Mat* image, int boxes); //returns 4-neighbourhood for boxes=0, 8-neighbourhood for boxes=1, 24-neighbourhood for boxes=2 etc.

