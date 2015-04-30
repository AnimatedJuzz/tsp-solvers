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
		int numCities = std::stoi(*(std::find(argv, argv + argc, NUM_CITY_DELIMITER) + 1));
		tour = new Tour(numCities);
	}
	else
	{
		std::cout << "Defaulting to 100 cities" << std::endl;
		tour = new Tour(DEFAULT_CITY_NUM);
	}
	if (std::find(argv, argv + argc, ALGORITHM_DELIMITER) != argv + argc)
	{
		std::string algorithm = std::string(*(std::find(argv, argv + argc, ALGORITHM_DELIMITER) + 1));

		if (algorithm == ALGORITHM_RANDOM)
		{
			std::cout << "Using the random algorithm" << std::endl;
		} else if (algorithm == ALGORITHM_TWO_OPT)
		{
			std::cout << "Using the 2-opt algorithm" << std::endl;
		} else if (algorithm == ALGORITHM_SIMULATED_ANNEALING)
		{
			std::cout << "Using simulated annealing" << std::endl;
		}
		else
		{
			std::cerr << "Invalid algorithm" << std::endl;
			return 1;
		}
	}

	tour->printGraph();
	std::cout << std::endl;
	std::vector< Graph::Edge > randomSolution = tour->solveRandom();
	std::vector< Graph::Edge > twoOptSolution = tour->solveRandomWithSwitches(3500.0, 500);
	std::vector< Graph::Edge > simulatedAnnealingSolution = tour->solveSimulatedAnnealing(20000.0, 20, 0.1);

	std::cout << std::endl << "Random Solution: " << std::endl;

	Tour::printPath(randomSolution);

	std::cout << std::endl << "2-Opt Solution: " << std::endl;

	Tour::printPath(twoOptSolution);

	std::cout << std::endl << "Simulated Annealing Solution: " << std::endl;

	Tour::printPath(simulatedAnnealingSolution);

	std::cout << std::endl << "Random Solution: " << tour->getTourLength(randomSolution) << std::endl;
	std::cout << "2-Opt Tour length: " << tour->getTourLength(twoOptSolution) << std::endl;
	std::cout << "Simulated Annealing Solution: " << tour->getTourLength(simulatedAnnealingSolution) << std::endl;

	if (!Tour::disableGUI)
		tour->getDisplayThread()->join();

	delete tour;

	return 0;
}
