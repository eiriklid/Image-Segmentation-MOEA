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

struct Point {
	uchar x;
	uchar y;
	Point(uchar x, uchar y) {
		this->x = x;
		this->y = y;
	}
	Point() {}
	bool operator ==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
};


typedef std::vector<Point> point_vec_t;
double color_distance(RGB a, RGB b);
double color_distance(Point a, Point b, cv::Mat *image);
point_vec_t neighbours(Point pixel, cv::Mat* image); //Returns 4-neighbourhood
point_vec_t neighbours(Point pixel, cv::Mat* image, int boxes); //returns 4-neighbourhood for boxes=0, 8-neighbourhood for boxes=1, 24-neighbourhood for boxes=2 etc.

