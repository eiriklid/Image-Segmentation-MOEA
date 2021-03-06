#include <opencv2/core/core.hpp>
#include <iostream>

#include "Segment.h"


Segment::Segment() {
	image_ptr = nullptr;
}

Segment::Segment(cv::Mat* image_ptr) {
	this->image_ptr = image_ptr;
}
Segment::Segment(cv::Mat* image_ptr, Point pixel) {
	this->image_ptr = image_ptr;
	points.insert(pixel);
}

/*Segment::Segment(cv::Mat* image_ptr, Segment a, Segment b) {
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
}*/

/*Segment::Segment(Segment a, Segment b) {
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
}*/


void Segment::insert_pixel(int x, int y) {
	this->points.insert(Point(x, y));
}
void Segment::insert_pixel(Point pixel) {
	this->points.insert(pixel);
}


bool Segment::erase_pixel(int x, int y) {
	return this->points.erase(Point(x, y));
}
bool Segment::erase_pixel(Point pixel) {
	return this->points.erase(pixel);
}

void Segment::print() {
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		std::cout << "(" << it->x << "," << it->y << ") " ;
	}
	std::cout << std::endl;
}

points_set_t* Segment::get_points() {
	return &points;
}
cv::Mat* Segment::get_image_ptr() {
	return image_ptr;
}


void Segment::calc_average() {
	double r = 0;
	double g = 0;
	double b = 0;
	RGB* current;
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); ++it) {
		current = image_ptr->ptr<RGB>(it->y)+ it->x;
		r += current->red;
		g += current->green;
		b += current->blue;
	}
	average;
	average.blue = b / points.size();
	average.green = g/ points.size();
	average.red = r/ points.size();
}

//Finds a pixel that has a neighbour outside the section
/*points_set_t Segment::get_edge() const{
	points_set_t edge_pixels;
	get_edge(&edge_pixels);

	return edge_pixels;
}*/

void Segment::get_edge(points_set_t* edge_pixels) const{
	edge_pixels->clear();
	point_vec_t neighbour_vec;

	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		neighbour_vec = neighbours(*it, image_ptr);
		for (point_vec_t::const_iterator neighbour_it = neighbour_vec.begin(); neighbour_it != neighbour_vec.end(); neighbour_it++) {
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

void Segment::calc_fitness() {
	calc_conectivity_measure();
	calc_edge_value();
	calc_overall_deviation();
}

double Segment::calc_overall_deviation() {
	calc_average();
	RGB centroid = read_average();
	double deviation = 0;
	for (points_set_t::const_iterator it = points.begin(); it != points.end(); it++) {
		deviation += color_distance(centroid, image_ptr->ptr<RGB>((*it).y)[(*it).x]);
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
	return edge_value = -distance;
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

/*bool Segment::neighbour(const Segment& seg) {
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
}*/

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