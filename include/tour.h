#ifndef TOUR_H
#define TOUR_H

#include "display.h"

#include <graph.h>
#include <algorithm>
#include <cmath>
#include <thread>

const double MAX_DIST_CITY = 999.0;
const double MIN_DIST_CITY = 100.0;

const long SEED = 42;

class Tour: public Graph {
public:
	Tour();
	Tour(int numCities);
	Tour(std::string inputFile);
	~Tour();

	static bool disableGUI;

	double getTourLength(path& tour);

	path solveRandom();
	path solveRandomWithSwitches(double maxLength, int maxTries);
	path solveSimulatedAnnealing(double initialTemp, double finalTemp, double tempLoss);

	std::shared_ptr< path >& getCurrentTour();
	const std::unique_ptr<std::thread>& getDisplayThread() const;

	static void printPath(const path path);

private:
	void populateGraph(std::vector< CityLocation > cities, double xMax=-1, double yMax=-1);
	path getRandomPath();
	std::pair<int, int> pickRand(int size);
	path swap(const path& tour, int firstVertex, int secondVertex);
	void updateDisplay(double temperature=-1, double distance=-1);

	std::shared_ptr< path > currentTour;
	std::unique_ptr< Display > display;
	std::unique_ptr< std::thread > displayThread;
};

#endif // TOUR_H
