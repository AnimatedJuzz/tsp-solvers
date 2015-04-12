#ifndef TOUR_H
#define TOUR_H

#include <graph.h>
#include <cmath>

const double MAX_DIST_CITY = 999.0;
const double MIN_DIST_CITY = 100.0;

class Tour: public Graph {
public:
	Tour();
	Tour(int numCities);
};

#endif // TOUR_H
