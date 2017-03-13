#include "Segment.h"

#include <opencv2/core/core.hpp>
#include <iostream>



void Segment::insert_pixel(int x, int y) {
	this->points.insert(cv::Point2i(x, y));
}
void Segment::insert_pixel(cv::Point2i pixel) {
	this->points.insert(pixel);
}


void Segment::erase_pixel(int x, int y) {
	this->points.erase(cv::Point2i(x, y));
}
void Segment::erase_pixel(cv::Point2i pixel) {
	this->points.erase(pixel);
}

void Segment::print() {
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		std::cout << "(" << it->x << "," << it->y << ")" << std::endl;
	}
}

points_set_t Segment::get_points() {
	return this->points;
}
