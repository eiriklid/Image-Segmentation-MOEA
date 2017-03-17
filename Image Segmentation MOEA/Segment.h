#pragma once
#include <unordered_set>
#include <opencv2/core/core.hpp>

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
	points_set_t points; //public for crossover?
	cv::Mat* image_ptr;

public:
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
	const cv::Mat* get_image_ptr();
	cv::Vec3d average();		//private?
	points_set_t get_edge() const;	//private?
	double overall_deviation();
	double edge_value();
	double conectivity_measure();
	bool neighbour(const Segment& seg);
	void insert_seg(const Segment& seg);

};

