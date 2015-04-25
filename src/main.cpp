#include <iostream>
#include "graph.h"
#include "tour.h"

int main() {
	Tour tour(10);
	tour.printGraph();
	//std::vector< Graph::Edge > solution = tour.solveRandom();
	std::cout << std::endl;
	//std::vector< Graph::Edge > solution = tour.solveRandomWithSwitches(3500.0, 500);
	std::vector< Graph::Edge > solution = tour.solveSimulatedAnnealing(6000.0, 20, 1);

	std::cout << std::endl << "Simulated Annealing: " << std::endl;

	Tour::printPath(solution);

	std::cout << std::endl << "Tour length: " << tour.getTourLength(solution) << std::endl;
}
