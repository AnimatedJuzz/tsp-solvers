/*
 * tour.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: srinivas
 */

#include "tour.h"

Tour::Tour() : Graph() { }

Tour::Tour(int numCities) : Graph(numCities), currentTour(new edges) {
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

edges Tour::solveRandom() {
	std::vector<Graph::Edge> tempSolution;
	double optimalDistance = 1000000000;

	for (int i = 0; i < 1000; i++)
	{
		tempSolution = this->getRandomPath();
		double tourLength = this->getTourLength(tempSolution);
		if (tourLength < optimalDistance)
		{
			optimalDistance = tourLength;
			this->currentTour = std::make_shared< edges > (tempSolution);
		}
	}

	return edges(*(this->currentTour));
}

edges Tour::solveRandomWithSwitches(double maxLength, int maxTries) {
	this->currentTour = std::make_shared< edges > (this->getRandomPath());
	double tourLength = this->getTourLength(*currentTour);

	int tries = 0;

	while (tourLength > maxLength && ++tries <= maxTries)
	{
		std::pair<int, int> randomIndices = this->pickRand(this->currentTour->size() - 1);
		edges newSolution = swap(*(this->currentTour), randomIndices.first, randomIndices.second);
		double newTourLength = this->getTourLength(newSolution);
		if (newTourLength < tourLength)
		{
			tourLength = newTourLength;
			this->currentTour = std::make_shared< edges > (newSolution);
		}
	}

	return edges(*(this->currentTour));
}

edges Tour::solveSimulatedAnnealing(double initialTemp, double finalTemp, double tempLoss) {
	this->currentTour = std::make_shared< edges > (this->getRandomPath());
	double tourLength = this->getTourLength(*(this->currentTour));
	double currentTemperature = initialTemp;

	while (currentTemperature > finalTemp)
	{
		std::pair<int, int> randomIndices = this->pickRand(this->currentTour->size() - 1);
		std::vector< Graph::Edge > newSolution = swap(*(this->currentTour), randomIndices.first, randomIndices.second);
		double newTourLength = this->getTourLength(newSolution);
		if ( ( (double) rand() / (RAND_MAX) ) < (std::exp(tourLength - newTourLength) / currentTemperature) )
		{
			tourLength = newTourLength;
			this->currentTour = std::make_shared< edges > (newSolution);
		}

		currentTemperature -= tempLoss;
	}

	return edges(*(this->currentTour));
}

void Tour::printPath(const std::vector< Graph::Edge > path) {
	for (Graph::Edge edge : path)
			std::cout << edge.startVertex->name << "->" << edge.endVertex->name << " ";
	std::cout << std::endl;
}

std::vector< Graph::Edge > Tour::getRandomPath() {
	srand (time(NULL));

	std::vector< Graph::Edge > randomPath;

	std::vector< int > verticesList(this->vertices.size());
	std::iota(std::begin(verticesList), std::end(verticesList), 0);
	std::random_shuffle(verticesList.begin(), verticesList.end());

	for (auto it = verticesList.begin(); it < verticesList.end() - 1; it++)
		randomPath.push_back(this->graph[*it][*(it+1)]);
	return randomPath;
}

double Tour::getTourLength(std::vector< Graph::Edge >& tour) {
	double totalDist = 0;
	std::for_each(tour.begin(), tour.end(), [&](Graph::Edge edge) { totalDist += edge.distance; });
	return totalDist;
}

std::pair<int, int> Tour::pickRand(int size){
	std::vector< int > elements(size);
	std::iota(std::begin(elements), std::end(elements), 1);
	std::random_shuffle(elements.begin(), elements.end());
	std::pair<int, int> pair;
	if (elements[0] < elements[1])
		pair = std::pair<int, int>(elements[0], elements[1]);
	else
		pair = std::pair<int, int>(elements[1], elements[0]);
	return pair;
}

// Ugly and annoying, but it works. See wikipedia page for details.
std::vector< Graph::Edge > Tour::swap(const std::vector< Graph::Edge >& tour, int firstVertex, int secondVertex) {

	auto getEdge = [&](Vertex* v1, Vertex* v2) { return this->graph[v1->pos][v2->pos]; };

	std::vector< Graph::Vertex* > vertices;
	for (auto it = tour.begin(); it < tour.end(); it++)
		vertices.push_back(it->startVertex);
	vertices.push_back(tour.back().endVertex);

	std::vector< Graph::Edge > route1;
	for (auto it = vertices.begin(); it <= vertices.begin() + firstVertex - 1; it++)
		route1.push_back(getEdge(*it, *(it+1)));

	std::vector< Graph::Edge > route2;
	for (auto it = vertices.begin() + firstVertex; it < vertices.begin() + secondVertex; it++)
		route2.insert(route2.begin(), getEdge(*(it+1), *it));

	route1[route1.size() - 1] = getEdge(vertices[firstVertex - 1], vertices[secondVertex]);

	std::vector< Graph::Edge > route3;
	for (auto it = vertices.begin() + secondVertex + 1; it < vertices.end() - 1; it++)
		route3.push_back(getEdge(*it, *(it+1)));

	route3.insert(route3.begin(), getEdge(vertices[firstVertex], vertices[secondVertex + 1]));

	route1.insert(route1.end(), route2.begin(), route2.end());
	route1.insert(route1.end(), route3.begin(), route3.end());
	return route1;
}

