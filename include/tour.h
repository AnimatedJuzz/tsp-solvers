#ifndef TOUR_H
#define TOUR_H

#include <graph.h>
#include <algorithm>
#include <cmath>

const double MAX_DIST_CITY = 999.0;
const double MIN_DIST_CITY = 100.0;

const long SEED = 42;

typedef std::vector< Graph::Edge > edges;

class Tour: public Graph {
public:
	Tour();
	Tour(int numCities);

	double getTourLength(std::vector< Graph::Edge >& tour);

	edges solveRandom();
	edges solveRandomWithSwitches(double maxLength, int maxTries);
	edges solveSimulatedAnnealing(double initialTemp, double finalTemp, double tempLoss);

	static void printPath(const std::vector< Graph::Edge > path);
private:
	edges getRandomPath();
	std::pair<int, int> pickRand(int size);
	edges swap(const std::vector< Graph::Edge >& tour, int firstVertex, int secondVertex);

	std::shared_ptr< edges > currentTour;
};

#endif // TOUR_H
