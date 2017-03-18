#include <opencv2/core/core.hpp>
#include <iostream>

#include "Segment.h"
#include "pixel_functions.h"


Segment::Segment() {
	image_ptr = nullptr;
}

Segment::Segment(cv::Mat* image_ptr) {
	this->image_ptr = image_ptr;
}
Segment::Segment(cv::Mat* image_ptr, cv::Point2i pixel) {
	this->image_ptr = image_ptr;
	points.insert(pixel);
}
Segment::Segment(cv::Mat* image_ptr, Segment a, Segment b) {
	if (a.neighbour(b) == false) {
		std::cout << "Segments not connected!" << std::endl;
	}
	this->image_ptr = image_ptr;
	points_set_t* a_points = a.get_points();
	for (points_set_t::const_iterator it = a_points->begin(); it != a_points->end(); it++) {
		this->points.insert(*it);
	}
	points_set_t* b_points = b.get_points();
	for (points_set_t::const_iterator it = b_points->begin(); it != b_points->end(); it++) {
		this->points.insert(*it);
	}
}

Segment::Segment(Segment a, Segment b) {
	this->image_ptr = a.image_ptr;
	if (a.neighbour(b)) {

		points_set_t* a_points = a.get_points();
		for (points_set_t::const_iterator it = a_points->begin(); it != a_points->end(); it++) {
			this->points.insert(*it);
		}
		points_set_t* b_points = b.get_points();
		for (points_set_t::const_iterator it = b_points->begin(); it != b_points->end(); it++) {
			this->points.insert(*it);
		}
	}
	else {
		std::cout << "Segments not connected!" << std::endl;
	}
}


void Segment::insert_pixel(int x, int y) {
	this->points.insert(cv::Point2i(x, y));
}
void Segment::insert_pixel(cv::Point2i pixel) {
	this->points.insert(pixel);
}


bool Segment::erase_pixel(int x, int y) {
	return this->points.erase(cv::Point2i(x, y));
}
bool Segment::erase_pixel(cv::Point2i pixel) {
	return this->points.erase(pixel);
}

void Segment::print() {
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		std::cout << "(" << it->x << "," << it->y << ")" << std::endl;
	}
}

points_set_t* Segment::get_points() {
	return &points;
}
cv::Mat* Segment::get_image_ptr() {
	return image_ptr;
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

//Finds a pixel that has a neighbour outside the section
points_set_t Segment::get_edge() const{
	points_set_t edge_pixels;
	get_edge(&edge_pixels);

	return edge_pixels;
}

void Segment::get_edge(points_set_t* edge_pixels) const{
	edge_pixels->clear();
	point_vec_t neighbour_vec;

	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) == points.end()) {
				edge_pixels->insert(*it);
				break;
			}
		}
	}
}

//finds the neighbours outside the section
void Segment::get_neighbouring_pixels(points_set_t* neighbour_pixels) const{
	neighbour_pixels->clear();
	point_vec_t neighbour_vec;

	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) == points.end()) {
				//Blir ikke lagt til dobbelt pga unordred set
				neighbour_pixels->insert(*neighbour_it);
				break;
			}
		}
	}
}

double Segment::calc_overall_deviation() {
	cv::Vec3d centroid = average();
	double deviation = 0;
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		deviation += color_distance(centroid, image_ptr->at<cv::Vec3b>(*it));
	}
	return overall_deviation= deviation;
}

double Segment::calc_edge_value() {
	points_set_t edges;
	get_edge(&edges);
	point_vec_t neighbour_vec;
	double distance = 0;
	for (points_set_t::const_iterator it = edges.begin(); it != edges.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) == points.end()) {
				distance += color_distance(*it, *neighbour_it, image_ptr);
			}
		}
		
	}
	return edge_value = distance;
}

double Segment::calc_conectivity_measure() {
	points_set_t edges;
	get_edge(&edges);
	point_vec_t neighbour_vec;
	double connectivity = 0;
	for (points_set_t::const_iterator it = edges.begin(); it != edges.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		double j = 1;
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) == points.end()) {
				connectivity += 1/j;
				j++;
			}
		}
	}
	return connectivity_measure = connectivity;
}

bool Segment::neighbour(const Segment& seg) {
	points_set_t edges;
	seg.get_edge(&edges);
	point_vec_t neighbour_vec;
	double connectivity = 0;
	for (points_set_t::const_iterator it = edges.begin(); it != edges.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
			if (points.find(*neighbour_it) != points.end() ){
				return true;
			}
		}
	}
	return false;
}

void Segment::insert(const Segment& seg) {
	points.insert(seg.points.begin(), seg.points.end());
}
bool Segment::erase(const Segment& seg) {
	int didErase = 0;
	for (auto it = seg.points.begin(); it != seg.points.end(); ++it) {
		didErase += points.erase(*it);
	}
	return didErase > 0;
}