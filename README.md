# Image-Segmentation-MOEA

IT3708 Project 3
Sufficient to get full score for segmentations.  


TODO:
- [x] Få tak i RGB for pixel
- [x] RGB-Distanse mellom piksler
- [x] Putte pixel i segment-set(Lage genotype)
- [x] Fitness for segment
	- [x] Find centroid(average)
	- [x] Find border-pixels
	- [x] For pixel find neighbour pixels
		- [x] dynamic? vector!
		- [x] make 4 & 8(n-boxses) neighbourhood
		- [x] Iterator for point_vec_t
- [x] Find neighbour-segments of segment
	- [x] Check if neighbour
	- [x] Get all neighbours
- [x] Initialize Solution
- [x] Mutation
	- [x] Merge
		- [x] put segment together
		- [x] Check for conectivity
	- [x] Split
- [x] Crossover
	- [?] sort segment from best to worse
	- [?] Fix "broken" segments
- [x] Fitness for Solution
	- [x] overall deviation
	- [x] edge value
	- [x] connectivity measure 
