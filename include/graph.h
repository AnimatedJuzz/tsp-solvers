#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <memory>
#include <string>

const int INDENT = 4;

const static char START_VAL = 'A';
const static char MAX_VAL = 'z';
const static int DEFAULT_SIZE = 3;

static int CURRENT_SIZE = 0;
static char CURRENT_VAL = START_VAL;

class Graph {

public:

	typedef struct Name {
		std::string val;

		bool operator==(const Name &rhs) const { return (val == rhs.val); }
		bool operator==(const std::string &rhs) const { return (val == rhs); }
		Name& operator++() {for (int i = 0; i < this->val.size(); i++) { if (val[i] < MAX_VAL) {val[i]++; break;}}; return *this; };

		Name(std::string a) : val(a) { };
		Name() : val() {
			if (CURRENT_VAL < MAX_VAL)
				val = Name(std::string(CURRENT_SIZE, MAX_VAL)+std::string(1, CURRENT_VAL++)).val;
			else
			{
				val = Name(std::string(CURRENT_SIZE++, MAX_VAL) + std::string(1, START_VAL)).val;
				CURRENT_VAL = START_VAL;
			}
		};

	} Name;

	typedef struct Vertex {
		Vertex() : name(), pos(-1) {};
		Vertex(const std::string name) : name(name), pos(-1) {};
		//Vertex(int pos) : name(1, DEFAULT_NAME++), pos(pos) {};
		Vertex(int pos) : name(Name().val), pos(pos) {};
		std::string name;
		int pos;
	} Vertex;

	typedef struct Edge {
		Edge() : distance(-1), startVertex(), endVertex() {};
		Edge(double distance) : distance(distance), startVertex(), endVertex() {};
		Edge(double distance, Vertex* startVertex, Vertex* endVertex) :
			distance(distance), startVertex(startVertex), endVertex(endVertex) {};

		bool operator<(const Edge &rhs) const { return this->distance < rhs.distance; }

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
