# Image-Segmentation-MOEA

TODO:
	- [x] Få tak i RGB for pixel
	- [x] RGB-Distanse mellom piksler
	- [x] Putte pixel i segment-set(Lage genotype)
	- [] Fitness for segment
		- [x] Find centroid(average)
		- [] Find border-pixels
			- 
		- [] For pixel find neighbour pixels
			- [x] dynamic? vector!
			- [] make 4 & 8 neighbourhood
			- [x] Iterator for point_vec_t
	- [] Find neighbour-sements of segment
	- [] Mutation
		- [] Merge
		- [] Split
	- [] Crossover
		- [] sort segment from best to worse
		- [] Fix "broken" segments
	- [] Initialize Solution
	- [] Fitness for Solution
		- [] overall deviation
		- [] edge value
		- [] connectivity measure 