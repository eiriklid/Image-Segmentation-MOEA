# Image-Segmentation-MOEA

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
- [] Find neighbour-segments of segment
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

Lage egen types-fil?