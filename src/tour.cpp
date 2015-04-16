/*
 * tour.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: srinivas
 */

#include "tour.h"

Tour::Tour() : Graph() { }

Tour::Tour(int numCities) : Graph(numCities) {
	std::vector< std::pair<double, double> > cities(0);

	// Create random cities
	srand (SEED);
	double multiplier = MAX_DIST_CITY - MIN_DIST_CITY;
	for (int index = 0; index < numCities; index++)
		cities.push_back(std::make_pair(MIN_DIST_CITY + ((double) rand() / RAND_MAX) * multiplier,
				MIN_DIST_CITY + ((double) rand() / RAND_MAX) * multiplier));

	// Populate graph with their distances
	for (auto startCity = cities.begin(); startCity < cities.end(); startCity++)
	{
		for (auto endCity = startCity + 1; endCity < cities.end(); endCity++)
		{
			double distance = std::sqrt(std::pow(endCity->first - startCity->first, 2) +
					std::pow(endCity->second - startCity->second, 2));
			this->changeEdgeWeight(distance, this->getVertex(startCity - cities.begin()),
					this->getVertex(endCity - cities.begin()));
		}
	}
}

std::vector< Graph::Edge > Tour::solveRandom() {
	std::vector<Graph::Edge> tempSolution;
	std::vector<Graph::Edge> optimalSolution;
	double optimalDistance = 1000000000;

	for (int i = 0; i < 10000; i++)
	{
		tempSolution = this->getRandomPath();
		double tourLength = this->getTourLength(tempSolution);
		if (tourLength < optimalDistance)
		{
			optimalDistance = tourLength;
			optimalSolution = tempSolution;
		}
	}

	return optimalSolution;
}

std::vector< Graph::Edge > Tour::getRandomPath() {
	srand (time(NULL));
	std::vector< Graph::Vertex > remainingVertices;
	std::for_each(this->vertices.begin(), this->vertices.end(), [&](std::unique_ptr< Graph::Vertex >& vertex)
			{ remainingVertices.push_back(*vertex); });

	Graph::Vertex currentVertex = remainingVertices[0];
	std::vector< Graph::Edge > randomPath;

	for (int i = 0; i < this->numVertices; i++)
	{
		Graph::Vertex nextVertex;
		int nextVertexIndex;

		do {
			nextVertexIndex = rand() % remainingVertices.size();
			nextVertex = remainingVertices[nextVertexIndex];
		} while (nextVertex.name == currentVertex.name);

		randomPath.push_back(this->graph[currentVertex.pos][nextVertex.pos]);
		currentVertex = nextVertex;
		remainingVertices.erase(remainingVertices.begin() + nextVertexIndex);
	}
	return randomPath;
}

double Tour::getTourLength(std::vector< Graph::Edge >& tour) {
	double totalDist = 0;
	std::for_each(tour.begin(), tour.end(), [&](Graph::Edge edge) { totalDist += edge.distance; });
	return totalDist;
}

