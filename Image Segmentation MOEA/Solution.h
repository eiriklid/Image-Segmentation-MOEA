#pragma once

#include <vector>
#include <opencv2/core/core.hpp>
#include "Segment.h"
#include "pixel_functions.h"
#include <Algorithm>

typedef std::vector <Segment> seg_vec_t;
//typedef std::vector <int> index_vec_t;

class Solution{
public:
	seg_vec_t segments;
private:
	double fitness[3];
	cv::Mat* image_ptr;
public:
	Solution(); //gjør hele init prosessen
	Solution(cv::Mat* image_ptr);

	const seg_vec_t* get_segments();

	//void merge_with_all_neighbours(Segment* seg);


	//void insert(const Solution& rhs);
	//void Solution::mergeSegments(seg_vec_t::iterator seg1, seg_vec_t::iterator seg2);

	void calc_fitness();
	double* read_fitness() { return fitness; }
	double read_fitness(int i) const { return fitness[i]; }

	//void split(seg_vec_t::iterator seg_it);
	void Solution::split(int seg_index, int n, std::vector<Point>& sepparation_points);
	void merge(Segment* seg1, std::unordered_set<int>& neighbourIDs);
	void merge(Segment* seg1, std::vector<Segment*>* segments, std::unordered_set<int>& neighbourIDs);

	//Det vi trenger top nivå:
	void mutation_merge();
	void mutation_split();


	void testIntegrity();
};


//ta hensyn ti at det kan hend vi ikke finner segmentet
//siden vi bare ser på ett subset av segmentet
//da bare driter vi i det puntket
void find_neighbours(const Segment* seg, std::vector<Segment>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr);
void find_neighbours(const Segment* seg, std::vector<Segment*>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr);

struct queue_element {
	Point pnt;
	int seg_id;
	double colourDistance;
	bool operator<(const queue_element& rhs)const {
		return colourDistance > rhs.colourDistance;
	}
};