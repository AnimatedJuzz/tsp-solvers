#include <iostream>
#include "graph.h"
#include "tour.h"

int main() {
	Tour tour(10);
	tour.printGraph();
	std::vector< Graph::Edge > solution = tour.solveRandom();

	std::cout << std::endl << "Random guessing: " << std::endl;

	for (Graph::Edge edge : solution)
		std::cout << edge.startVertex->name << "->" << edge.endVertex->name << " ";

	std::cout << std::endl << "Tour length: " << tour.getTourLength(solution) << std::endl;
}
