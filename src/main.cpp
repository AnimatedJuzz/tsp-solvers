#include <iostream>
#include "graph.h"
#include "tour.h"

/*
 * Command-line arguments work as follows:
 *
 * TSPRunner [OPTIONS] -c [NUM_CITIES] -a [ALGORITHM]
 *
 * Options:
 *  --no-gui	Disables SFML
 *
 * Algorithm shorthand:
 * r	Random
 * t	Two-Opt
 * s	Simulated Annealing
 */

const int DEFAULT_CITY_NUM = 100;
const std::string correctUsage = "TSPRunner [OPTIONS] -c [NUM_CITIES] -a [ALGORITHM]";

const std::string NUM_CITY_DELIMITER = "-c";
const std::string ALGORITHM_DELIMITER = "-a";

const std::string ALGORITHM_RANDOM = "r";
const std::string ALGORITHM_TWO_OPT = "t";
const std::string ALGORITHM_SIMULATED_ANNEALING = "s";

const std::string NO_GUI = "--nogui";

bool Tour::disableGUI = false;

int main(int argc, char* argv[]) {
	Tour* tour;

	switch (argc)
	{
	case 2:
		if (argv[1] == NUM_CITY_DELIMITER || argv[1] == ALGORITHM_DELIMITER)
		{
			std::cerr << "Insufficient arguments. " + correctUsage << std::endl;
			return 1;
		}
		break;
	}

	if (std::find(argv, argv + argc, NO_GUI) != argv + argc)
	{
		std::cout << "GUI disabled" << std::endl;
		Tour::disableGUI = true;
		// Actually disable the GUI here
	}
	if (std::find(argv, argv + argc, NUM_CITY_DELIMITER) != argv + argc)
	{
		try {
			int numCities = std::stoi(*(std::find(argv, argv + argc, NUM_CITY_DELIMITER) + 1));
			tour = new Tour(numCities);
		} catch (...) {
			std::cerr << "City number not given. " + correctUsage << std::endl;
			return 1;
		}

	}
	else
	{
		std::cout << "Defaulting to 100 cities" << std::endl;
		tour = new Tour(DEFAULT_CITY_NUM);
	}

	path * solution;
	if (std::find(argv, argv + argc, ALGORITHM_DELIMITER) != argv + argc)
	{
		std::string algorithm;
		try {
			algorithm = std::string(*(std::find(argv, argv + argc, ALGORITHM_DELIMITER) + 1));
		} catch (...) {
			std::cerr << "Algorithm not given. " + correctUsage << std::endl;
			return 1;
		}

		if (algorithm == ALGORITHM_RANDOM)
		{
			std::cout << "Using the random algorithm" << std::endl;
			solution = new path(tour->solveRandom());
		} else if (algorithm == ALGORITHM_TWO_OPT)
		{
			std::cout << "Using the 2-opt algorithm" << std::endl;
			solution = new path(tour->solveRandomWithSwitches(3500.0, 500));
		} else if (algorithm == ALGORITHM_SIMULATED_ANNEALING)
		{
			std::cout << "Using simulated annealing" << std::endl;
			solution = new path(tour->solveSimulatedAnnealing(20000.0, 20, 0.1));
		}
		else
		{
			std::cerr << "Invalid algorithm. " + correctUsage << std::endl;
			return 1;
		}
	}
	else
		solution = new path(tour->solveSimulatedAnnealing(20000.0, 20, 0.1));

	tour->printGraph();
	std::cout << std::endl;

	Tour::printPath(*solution);

	std::cout << std::endl << "Tour Length: " << tour->getTourLength(*solution) << std::endl;

	if (!Tour::disableGUI)
		tour->getDisplayThread()->join();

	delete tour;

	return 0;
}
