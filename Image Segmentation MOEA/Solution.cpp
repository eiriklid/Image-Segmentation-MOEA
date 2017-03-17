#include "Solution.h"
#include "Segment.h"

#include <iostream>


Solution::Solution() {

}

Solution::Solution(cv::Mat* image_ptr){
	segments = seg_vec_t(image_ptr->cols*image_ptr->rows);
	this->image_ptr = image_ptr;
	for (int i = 0; i < image_ptr->cols; i++) {
		for (int j = 0; j < image_ptr->rows; j++) {
			Segment seg = Segment(image_ptr, cv::Point2i(i, j));
			segments.push_back(seg);
		}
	}
	

}
seg_vec_t Solution::get_segments() {
	return segments;
}
/*
seg_ptr_vec_t Solution::neighbour_seg(Segment seg) {
	seg_ptr_vec_t neighbour_segs;
	for (seg_vec_t::iterator it = segments.begin(); it != segments.end(); it++) {
		if (seg.neighbour(*it)) {
			neighbour_segs.push_back(&*it);
		}
	}
	return neighbour_segs;
}


index_vec_t Solution::neighbour_seg(Segment seg) {
	seg_itr_vec_t neighbour_segs;
	for (seg_vec_t::iterator it = segments.begin(); it != segments.end(); it++) {
		if (seg.neighbour(*it)) {
			neighbour_segs.push_back(it);
		}
	}
	return neighbour_segs;
}
*/


Segment Solution::merge(Segment a, Segment b) {
	return Segment(a, b);
}

void Solution::merge_with_all_neighbours(Segment* seg) {
	for (seg_vec_t::iterator it = segments.begin(); it != segments.end(); it++) {
		if (seg->neighbour(*it)) {
			
		}

	}
}

