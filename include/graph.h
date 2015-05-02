#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <string>

const int INDENT = 4;

const static char START_VAL = 'A';
const static int DEFAULT_SIZE = 3;

static int CURRENT_VAL = 0;

class Graph {

public:

	const std::string DIMENSION = "DIMENSION";
	const std::string EDGE_WEIGHT_TYPE = "EUC_2D";
	const std::string NODE_COORD_SECTION = "NODE_COORD_SECTION";
	const std::string EOF_LABEL = "EOF";

	typedef struct Name {
		std::string val;

		bool operator==(const Name &rhs) const { return (val == rhs.val); }
		bool operator==(const std::string &rhs) const { return (val == rhs); }

		Name(std::string a) : val(a) { };
		Name() : val() {
			int remainder = CURRENT_VAL++;
			for (int i = DEFAULT_SIZE - 1; i >= 0; i--)
			{
				val += ((char) START_VAL + (remainder / (pow(26.0, i))) );
				remainder %= ((int) pow(26.0, i));
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
	Graph(int& numVertices);
	virtual ~Graph() { };
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

typedef std::vector< Graph::Edge > path;

#endif // GRAPH_H
