#pragma once

#include <vector>

#include "Segment.h"
class Solution{
private:
	std::vector<Segment> segments;


public:
	Solution();
	double f1();	// Overal deviation
	double f2();	// Edge
	double f3();	// Connectivity measure
	void fitness(double* val1, double* val2, double* val3);

	double fitness[3];
};

