#include <iostream>
#include "graph.h"

int main() {
	Graph graph(5);
	graph.addVertex(Graph::Vertex("F"));
	graph.addEdge(5, graph.searchByName("A"), graph.searchByName("F"));
	graph.deleteEdge(graph.searchByName("A"), graph.searchByName("F"));
	graph.addEdge(7, graph.searchByName("B"), graph.searchByName("C"));
	graph.addEdge(4, graph.searchByName("B"), graph.searchByName("D"));
	graph.addEdge(6, graph.searchByName("B"), graph.searchByName("A"));

	graph.printGraph();
	std::cout << std::endl << std::endl << "Vertices connected to B: " << std::endl;

	for (auto edge : graph.getEdgesFromVertex(graph.searchByName("B")))
		std::cout << edge.endVertex->name << std::endl;
}
