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

public:
	//Segment();
	//~Segment();
	void insert_pixel(int x, int y);
	void insert_pixel(cv::Point2i pixel);
	void erase_pixel(int x, int y); //return if found?
	void erase_pixel(cv::Point2i pixel);
	void print();
};


