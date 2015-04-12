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
	srand (time(NULL));
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

