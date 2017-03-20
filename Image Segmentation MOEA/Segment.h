#pragma once
#include <unordered_set>
#include <opencv2/core/core.hpp>
#include "Parameters.h"
#include "pixel_functions.h"

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

	RGB average;

	Segment(); //For allocation only
	Segment(cv::Mat* image_ptr);
	Segment(cv::Mat* image_ptr, cv::Point2i pixel);
	//segment(Segment a, Segment b);
	//Segment(cv::Mat* image_ptr, Segment a, Segment b);

	void calc_average();
	RGB read_average() { return average; }
	//~Segment();
	void insert_pixel(int x, int y);
	void insert_pixel(cv::Point2i pixel);
	bool erase_pixel(int x, int y); 
	bool erase_pixel(cv::Point2i pixel);
	void print();
	points_set_t* get_points();
	cv::Mat* get_image_ptr();
	//points_set_t get_edge() const;	
	void get_edge(points_set_t* edge_pixels) const;
	void get_neighbouring_pixels(points_set_t* neighbour_pixels) const;

	void calc_fitness();
	double calc_overall_deviation();
	double calc_edge_value();
	double calc_conectivity_measure();

	double read_overall_deviation() const { return overall_deviation; }
	double read_edge_value() const { return edge_value; }
	double read_connectivity_measure() const { return connectivity_measure; }

	//bool neighbour(const Segment& seg);//burde renames til isNeighbour
	void insert(const Segment& seg);
	bool erase(const Segment& seg);

	bool operator ==(const Segment& rhs) {
		return points == rhs.points;
	}
};

