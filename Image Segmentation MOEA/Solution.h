#pragma once

#include <vector>
#include <opencv2/core/core.hpp>
#include "Segment.h"

typedef std::vector <Segment> seg_vec_t;
//typedef std::vector <int> index_vec_t;

class Solution{
private:
	seg_vec_t segments;
	cv::Mat* image_ptr;

public:
	Solution();
	Solution(cv::Mat* image_ptr);

	double fitness[3];
	seg_vec_t get_segments();

	Segment merge(Segment a, Segment b);
	void merge_with_best(Segment seg);
	void merge_with_all_neighbours(Segment* seg);

};

