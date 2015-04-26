#include <iostream>
#include "graph.h"
#include "tour.h"

int main(int argc, char* argv[]) {
	Tour* tour;
	if (argc == 1)
		tour = new Tour(100);
	else
		tour = new Tour(std::stoi(argv[1]));
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

	tour->getDisplayThread()->join();

	delete tour;
}
