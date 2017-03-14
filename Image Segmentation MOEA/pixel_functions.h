#pragma once
#include <opencv2/core/core.hpp>

#include <vector>


typedef std::vector<cv::Point2i> point_vec_t;
double color_distance(cv::Vec3b a, cv::Vec3b b);
point_vec_t neighbours(cv::Point2i pixel, cv::Mat& image); //Returns 4-neighbourhood
point_vec_t neighbours(cv::Point2i pixel, cv::Mat& image, int boxes); //returns 4-neighbourhood for boxes=0, 8-neighbourhood for boxes=1, 24-neighbourhood for boxes=2 etc.

