#pragma once

//#define FILENAME "triangle.jpg"
//#define FILENAME "..//Test Image//1//img1_xxs.png"
#define FILENAME "..//Test Image//1//Test image.jpg"
//#define FILENAME "img1_xxxxs.png"

#define USE_OVERALL_DEVIATION 1
#define USE_EDGE_VALUE 1
#define USE_CONNECTIVITY_MEASURE 1
#define NUM_OBJECTIVES (USE_OVERALL_DEVIATION+USE_EDGE_VALUE+USE_CONNECTIVITY_MEASURE)

#define INIT_MIN_NUN_SEGMENTS 4
#define INIT_MAX_NUM_SEGMENTS 20


//Sannsynligheter
//n betyr 1/n. n==0 betyr probabiliy = 0
#define CLONE_RATE 5
#define MUTATION_SPLIT_RATE 0
#define MUTATION_MERGE_RATE 5
//#define MUTATION_RANDOM_MERGE_RATE 3 //TODO

//NSGA-II
#define POPPULATION_SIZE 10
#define TOURNEY_SIZE 2
#define TOURNEY_ERROR_RATE 20