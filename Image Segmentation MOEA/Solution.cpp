#include "Solution.h"



Solution::Solution(cv::Mat* image_ptr){
	segments = std::vector<Segment>(image_ptr->cols*image_ptr->rows);
	this->image_ptr = image_ptr;
	for (int i = 0; i < image_ptr->cols; i++) {
		for (int j = 0; j < image_ptr->rows; j++) {
			Segment seg = Segment(image_ptr, cv::Point2i(i, j));
			segments.push_back(seg);
		}
	}
}

Solution::Solution() {

}


