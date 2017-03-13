#pragma once

#include <opencv2/core/core.hpp>
#include "Segment.h"
#include "Solution.h"

Solution crossover(Solution a, Solution b);

void mutation(const cv::Mat* image, Segment* seg);

Segment merge(Segment a, Segment b);

Segment* split(Segment seg, Solution* sol); //Ev. void 


