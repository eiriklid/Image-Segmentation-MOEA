#include <opencv2/core/core.hpp>
#include <iostream>

#include "Segment.h"
#include "pixel_functions.h"


Segment::Segment(cv::Mat* image_ptr) {
	this->image_ptr = image_ptr;
}



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

cv::Vec3d Segment::average() {
	cv::Vec3d total(0,0,0);
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		total += image_ptr->at<cv::Vec3b>(*it);
	}
	cv::Vec3d average;
	average[0] = total[0] / points.size();
	average[1] = total[1] / points.size();
	average[2] = total[2] / points.size();
	return average;
}

points_set_t Segment::get_edge() {
	point_vec_t neighbour_vec;
	points_set_t edge_pixels;

	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) == points.end()) {
				edge_pixels.insert(*it);
				break;
			}
		}
	}

	return edge_pixels;
}