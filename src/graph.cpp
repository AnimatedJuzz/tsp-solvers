#include "graph.h"

Graph::Graph() : graph(), search(), numVertices(0) { }

Graph::Graph(int numVertices) :
		graph(),
		search(),
		numVertices(numVertices),
		vertices(numVertices) {
	this->graph.resize(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		vertices[i] = std::unique_ptr<Graph::Vertex>(new Graph::Vertex(i));
		this->search[vertices[i]->name] = &(vertices[i]->pos);
	}

	for (int i = 0; i < numVertices; i++)
	{
		this->graph[i].resize(numVertices);

		for (int j = 0; j < numVertices; j++)
			this->graph[i][j] = Edge(0, vertices[i].get(), vertices[j].get());
	}
}

void Graph::addVertex(const Vertex& vertex) {
	this->vertices.push_back(std::unique_ptr<Graph::Vertex>(new Graph::Vertex(vertex)));
	Graph::Vertex* actualVertex = this->vertices.back().get();
	actualVertex->pos = numVertices++;

	this->graph.push_back(std::vector<Graph::Edge>(numVertices));
	for (int i = 0; i < numVertices; i++)
	{
		Graph::Vertex* startVertex = this->graph[i][i].startVertex;
		if (i == numVertices - 1)
			startVertex = actualVertex;

		this->graph.back()[i] = Edge(0, actualVertex, startVertex);

		if (i < numVertices - 1)
			this->graph[i].push_back(Edge(0, startVertex, actualVertex));
	}

	this->search[vertex.name] = &(actualVertex->pos);
}

void Graph::addEdge(int distance, const Vertex& startVertex, const Vertex& endVertex) {
	this->graph[startVertex.pos][endVertex.pos].distance = distance;
	this->graph[endVertex.pos][startVertex.pos].distance = distance;
}

const Graph::Vertex& Graph::getVertex(int pos) {
	return *(this->graph[pos][0].startVertex);
}

void Graph::deleteVertex(const Vertex& vertex) {

	try {
		this->search.erase(vertex.name);
	} catch (...) {
		std::cout << "Invalid vertex" << std::endl;
		return;
	}

	this->graph.erase(this->graph.begin() + vertex.pos);

	for (auto row = this->graph.begin(); row < this->graph.end(); row++)
	{
		row->erase(row->begin() + vertex.pos);
		for (auto edge = row->begin(); edge < row->end(); edge++)
			edge->startVertex->pos = row - this->graph.begin();
	}

	numVertices--;
	this->vertices.erase(this->vertices.begin() + vertex.pos);
}

void Graph::deleteEdge(const Vertex& startVertex, const Vertex& endVertex) {
	this->graph[startVertex.pos][endVertex.pos].distance = 0;
	this->graph[endVertex.pos][startVertex.pos].distance = 0;
}

// Start vertex is defined on the left; on the top is end vertex
// Start vertex is the row, end vertex is the column
void Graph::printGraph() {
	std::string topAxis = " ";
	for (auto row : this->graph)
		topAxis += row[0].startVertex->name;
	std::cout << topAxis << std::endl;

	for (auto row : this->graph)
	{
		std::cout << row[0].startVertex->name;
		for (auto edge : row)
			std::cout << edge.distance;
		std::cout << std::endl;
	}
}

const Graph::Vertex& Graph::searchByName(const std::string name) {
	return *(this->vertices[*(this->search[name])]);
}
