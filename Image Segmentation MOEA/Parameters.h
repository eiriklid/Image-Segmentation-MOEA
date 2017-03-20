#pragma once

//#define FILENAME "triangle.jpg"
#define FILENAME "..//Test Image//1//img1_xxs.png"
//#define FILENAME "..//Test Image//7//Test image.jpg"

#define USE_OVERALL_DEVIATION 1
#define USE_EDGE_VALUE 1
#define USE_CONNECTIVITY_MEASURE 1
#define NUM_OBJECTIVES (USE_OVERALL_DEVIATION+USE_EDGE_VALUE+USE_CONNECTIVITY_MEASURE)

#define INIT_MIN_NUN_SEGMENTS 9
#define INIT_MAX_NUM_SEGMENTS 40


//Sannsynligheter
//n betyr 1/n. n==0 betyr probabiliy = 0
#define CLONE_RATE 10
#define MUTATION_SPLIT_RATE 5
#define MUTATION_MERGE_RATE 5

//NSGA-II
#define POPPULATION_SIZE 10
#define TOURNEY_SIZE 2
#define TOURNEY_ERROR_RATE 20