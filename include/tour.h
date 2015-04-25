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
	std::vector< Graph::Edge > solveRandomWithSwitches(double maxLength, int maxTries);
	std::vector< Graph::Edge > solveSimulatedAnnealing(double initialTemp, double finalTemp, double tempLoss);

	static void printPath(const std::vector< Graph::Edge > path);
private:
	std::vector< Graph::Edge > getRandomPath();
	std::pair<int, int> pickRand(int size);
	std::vector< Graph::Edge > swap(const std::vector< Graph::Edge >& tour, int firstVertex, int secondVertex);
};

#endif // TOUR_H
