#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <memory>
#include <string>

static char DEFAULT_NAME = 'A';
const int INDENT = 4;

class Graph {

public:

	typedef struct Vertex {
		Vertex() : name(), pos(-1) {};
		Vertex(const std::string name) : name(name), pos(-1) {};
		Vertex(int pos) : name(1, DEFAULT_NAME++), pos(pos) {};
		std::string name;
		int pos;
	} Vertex;

	typedef struct Edge {
		Edge() : distance(-1), startVertex(), endVertex() {};
		Edge(double distance) : distance(distance), startVertex(), endVertex() {};
		Edge(double distance, Vertex* startVertex, Vertex* endVertex) :
			distance(distance), startVertex(startVertex), endVertex(endVertex) {};
		double distance;
		Vertex* startVertex;
		Vertex* endVertex;
	} Edge;

	Graph();
	Graph(int numVertices);
	void addVertex(const Vertex& vertex);
	void addEdge(int distance, const Vertex& startVertex, const Vertex& endVertex);
	void changeEdgeWeight(int newDistance, const Vertex& startVertex, const Vertex& endVertex);
	const Graph::Vertex& getVertex(int pos);
	const std::vector<Graph::Edge> getEdgesFromVertex(const Vertex& vertex);
	void deleteVertex(const Vertex& vertex);
	void deleteEdge(const Vertex& startVertex, const Vertex& endVertex);
	void printGraph();
	const Graph::Vertex& searchByName(const std::string name);
protected:
	std::vector< std::vector<Edge> > graph;
	std::map< std::string, int* > search;
	std::vector<std::unique_ptr< Graph::Vertex > > vertices;
	int numVertices;
};
#endif // GRAPH_H
