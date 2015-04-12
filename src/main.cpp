#include <iostream>
#include "graph.h"

int main() {
	Graph graph(5);
	graph.addVertex(Graph::Vertex("F"));
	graph.addEdge(5, graph.searchByName("A"), graph.searchByName("F"));
	const Graph::Vertex& deleteVertex = graph.getVertex(3);
	graph.deleteVertex(deleteVertex);
	graph.deleteEdge(graph.searchByName("A"), graph.searchByName("F"));
	graph.printGraph();
}
