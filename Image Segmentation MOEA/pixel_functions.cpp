#include "pixel_functions.h"


double color_distance(cv::Vec3b a, cv::Vec3b b) {
	cv::Vec3b d = a - b;
	return cv::norm(d);
}

point_vec_t neighbours(cv::Point2i pixel, cv::Mat& image) {
	point_vec_t neighbour_array;
	if (pixel.x > 0) {
		neighbour_array.push_back(cv::Point2i(pixel.x - 1, pixel.y));
	}
	if (pixel.y > 0) {
		neighbour_array.push_back(cv::Point2i(pixel.x, pixel.y - 1));
	}
	if (pixel.x < image.cols - 1) {
		neighbour_array.push_back(cv::Point2i(pixel.x + 1, pixel.y));
	}
	if (pixel.y < image.rows - 1) {
		neighbour_array.push_back(cv::Point2i(pixel.x, pixel.y + 1));
	}

	return neighbour_array;
}

point_vec_t neighbours(cv::Point2i pixel, cv::Mat& image, int boxes) {

	if (boxes < 1) {
		return neighbours(pixel, image);
	}
	else {
		point_vec_t neighbour_array;

		for (int i = -boxes; i <= boxes; i++) {
			for (int j = -boxes; j <= boxes; j++) {
				if ((i != 0 || j != 0) && ((pixel.x + i) >= 0 && (pixel.x + i) < image.cols) && ((pixel.y + j) >= 0 && (pixel.y + j)< image.rows)) {
					neighbour_array.push_back(cv::Point2i(pixel.x + i, pixel.y + j));
				}
			}
		}
		return neighbour_array;
	}
}
