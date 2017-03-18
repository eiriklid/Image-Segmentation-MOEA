#pragma once

#include <vector>
#include <opencv2/core/core.hpp>
#include "Segment.h"
#include "pixel_functions.h"
#include <Algorithm>

typedef std::vector <Segment> seg_vec_t;
//typedef std::vector <int> index_vec_t;

class Solution{
private:
	cv::Mat* image_ptr;
	double fitness[3];
public:
	seg_vec_t segments;
	Solution(); //gjør hele init prosessen
	Solution(cv::Mat* image_ptr);

	const seg_vec_t* get_segments();

	//void merge_with_all_neighbours(Segment* seg);


	//void insert(const Solution& rhs);
	//void Solution::mergeSegments(seg_vec_t::iterator seg1, seg_vec_t::iterator seg2);

	void calc_fitness();
	double* read_fitness() { return fitness; }
	double read_fitness(int i) const { return fitness[i]; }

	void split(seg_vec_t::iterator seg_it);
	int merge(Segment* seg1, std::unordered_set<int>& neighbourIDs);
	int merge(Segment* seg1, vector<Segment*>* segments, unordered_set<int>& neighbourIDs);

	//Det vi trenger top nivå:
	void mutation_merge();
	void mutation_split();
};


//ta hensyn ti at det kan hend vi ikke finner segmentet
//siden vi bare ser på ett subset av segmentet
//da bare driter vi i det puntket
void find_neighbours(const Segment* seg, std::vector<Segment>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr);
void find_neighbours(const Segment* seg, std::vector<Segment*>* segments, std::unordered_set<int>* neighbourIDs, cv::Mat* image_ptr);

