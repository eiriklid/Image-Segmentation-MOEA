#pragma once

//#define FILENAME "triangle.jpg"
//#define FILENAME "..//Test Image//1//img1_xxs.png"
#define FILENAME "..//Test Image//7//Test image.jpg"
//#define FILENAME "img1_xxxxs.png"

//NB to må være på sammtidig
#define USE_OVERALL_DEVIATION 1
#define USE_EDGE_VALUE 1
#define USE_CONNECTIVITY_MEASURE 1
#define NUM_OBJECTIVES (USE_OVERALL_DEVIATION+USE_EDGE_VALUE+USE_CONNECTIVITY_MEASURE)

#define MIN_NUM_SEGMENTS 5
#define MAX_NUM_SEGMENTS 25

#define INIT_MIN_NUN_SEGMENTS 6
#define INIT_MAX_NUM_SEGMENTS 19


//Sannsynligheter
//n betyr 1/n. n==0 betyr probabiliy = 0
#define CLONE_RATE 0
#define MUTATION_SPLIT_RATE 2
#define MUTATION_MERGE_RATE 2
#define MUTATION_MERGE_SMALLEST_SEGMENT_RATE 2
//#define MUTATION_RANDOM_MERGE_RATE 3 //TODO

//NSGA-II
#define POPPULATION_SIZE 30
#define TOURNEY_SIZE 2
#define TOURNEY_ERROR_RATE 20