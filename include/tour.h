#ifndef TOUR_H
#define TOUR_H

#include <graph.h>
#include <algorithm>
#include <cmath>

const double MAX_DIST_CITY = 999.0;
const double MIN_DIST_CITY = 100.0;

const long SEED = 42;

class Tour: public Graph {
public:
	Tour();
	Tour(int numCities);

	double getTourLength(std::vector< Graph::Edge >& tour);

	std::vector< Graph::Edge > solveRandom();
private:
	std::vector< Graph::Edge > getRandomPath();
};

#endif // TOUR_H
