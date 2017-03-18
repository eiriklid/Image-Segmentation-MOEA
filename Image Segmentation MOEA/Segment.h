#pragma once
#include <unordered_set>
#include <opencv2/core/core.hpp>
#include "Parameters.h"

namespace std {
	template<> struct hash<cv::Point> {
		size_t operator()(const cv::Point &pt) const {
			return std::hash<int>()(pt.x) ^ std::hash<int>()(pt.y);
		}
	};
}

typedef std::unordered_set <cv::Point2i> points_set_t;

class Segment
{
private:
	cv::Mat* image_ptr;

	double overall_deviation;
	double edge_value;
	double connectivity_measure;

public:
	points_set_t points; //public for crossover?
	bool isChanged;

	Segment(); //For allocation only
	Segment(cv::Mat* image_ptr);
	Segment(cv::Mat* image_ptr, cv::Point2i pixel);
	Segment(Segment a, Segment b);
	Segment(cv::Mat* image_ptr, Segment a, Segment b);


	//~Segment();
	void insert_pixel(int x, int y);
	void insert_pixel(cv::Point2i pixel);
	bool erase_pixel(int x, int y); //return if found?
	bool erase_pixel(cv::Point2i pixel);
	void print();
	points_set_t* get_points();
	cv::Mat* get_image_ptr();
	cv::Vec3d average();		//private?
	points_set_t get_edge() const;	//private?
	void get_edge(points_set_t* edge_pixels) const;
	void get_neighbouring_pixels(points_set_t* neighbour_pixels) const;

	double calc_fitness();
	double calc_overall_deviation();
	double calc_edge_value();
	double calc_conectivity_measure();

	double read_overall_deviation() { return overall_deviation; }
	double read_edge_value() { return edge_value; }
	double read_connectivity_measure() { return connectivity_measure; }

	bool neighbour(const Segment& seg);
	void insert(const Segment& seg);
	bool erase(const Segment& seg);

};

