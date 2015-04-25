#include <iostream>
#include "graph.h"
#include "tour.h"

int main() {
	Tour tour(100);
	tour.printGraph();
	std::cout << std::endl;
	std::vector< Graph::Edge > twoOptSolution = tour.solveRandomWithSwitches(3500.0, 500);
	std::vector< Graph::Edge > simulatedAnnealingSolution = tour.solveSimulatedAnnealing(6000.0, 20, 1);

	std::cout << std::endl << "2-Opt Solution: " << std::endl;

	Tour::printPath(twoOptSolution);

	std::cout << std::endl << "Simulated Annealing Solution: " << std::endl;

	Tour::printPath(simulatedAnnealingSolution);

	std::cout << std::endl << "2-Opt Tour length: " << tour.getTourLength(twoOptSolution) << std::endl;
	std::cout << "Simulated Annealing Solution: " << tour.getTourLength(simulatedAnnealingSolution) << std::endl;
}
