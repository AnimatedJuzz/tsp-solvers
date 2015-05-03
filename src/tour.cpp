/*
 * tour.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: srinivas
 */

#include "tour.h"

Tour::Tour() : Graph() { }

Tour::Tour(int numCities) : Graph(numCities), currentTour(new path) {
	std::vector< CityLocation > cities(0);

	// Create random cities
	srand (SEED);
	double multiplier = MAX_DIST_CITY - MIN_DIST_CITY;
	for (int index = 0; index < numCities; index++)
	{
		CityLocation location = std::make_pair(this->vertices[index]->name, sf::Vector2f(
				MIN_DIST_CITY + ((double) rand() / RAND_MAX) * multiplier,
				MIN_DIST_CITY + ((double) rand() / RAND_MAX) * multiplier));
		cities.push_back(location);
	}

	this->populateGraph(cities);
}

Tour::Tour(std::string inputFileName) : Graph(), currentTour(new path) {
	std::string file;
	std::ifstream inputFile(inputFileName);

	if (!inputFile.is_open())
		throw std::invalid_argument("File not found");

	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	file = buffer.str();

	std::size_t dimension = file.find(Graph::DIMENSION);
	std::size_t edgeWeightType = file.find(EDGE_WEIGHT_TYPE);
	std::size_t nodeCoordSection = file.find(NODE_COORD_SECTION);

	if (dimension == std::string::npos || edgeWeightType == std::string::npos || nodeCoordSection == std::string::npos)
	{
		std::string invalid = "Invalid input file";
		std::cerr << invalid << std::endl;
		throw std::invalid_argument(invalid);
	}
	else
	{
		std::size_t pos = dimension;
		auto skipUntilNewline = [&] () { do pos++; while(file[pos] != '\n'); };
		auto skipWhitespace = [&] () { while (file[pos] == ' ' || file[pos] == '\n') pos++; };
		auto getToken = [&] () { std::string token; while (file[pos] != ' ' && file[pos] != '\n') token += file[pos++]; return token; };
		int sizeOfGraph;

		getToken();
		skipWhitespace();
		std::string dimensionString = getToken();

		if (dimensionString == ":")
		{
			skipWhitespace();
			dimensionString = getToken();
		}

		sizeOfGraph = std::stoi(dimensionString);
		pos = nodeCoordSection;

		std::vector< std::unique_ptr< Vertex > > vertexList;
		std::vector< CityLocation > cities(sizeOfGraph);

		double xMax, yMax = 0;

		skipUntilNewline();
		while (pos < file.length())
		{
			skipWhitespace();
			std::string token = getToken();

			if (token == Graph::EOF_LABEL)
				break;

			int nodeNumber = std::stoi(token) - 1;
			skipWhitespace();
			double x1 = std::stod(getToken());
			skipWhitespace();
			double y1 = std::stod(getToken());

			if (x1 > xMax)
				xMax = x1;
			if (y1 > yMax)
				yMax = y1;

			vertexList.push_back(std::unique_ptr<Graph::Vertex>(new Graph::Vertex(nodeNumber)));
			this->addVertex(*vertexList[nodeNumber]);
			cities[nodeNumber] = std::make_pair(vertexList[nodeNumber]->name, sf::Vector2f(x1, y1));
		}
		this->populateGraph(cities, xMax, yMax);
	}
}

path Tour::solveRandom() {
	path tempSolution;
	double optimalDistance = 1000000000;

	for (int i = 0; i < 1000; i++)
	{
		tempSolution = this->getRandomPath();
		double tourLength = this->getTourLength(tempSolution);
		if (tourLength < optimalDistance)
		{
			optimalDistance = tourLength;
			this->currentTour = std::make_shared< path > (tempSolution);
			updateDisplay(-1.0, tourLength);
		}
	}

	return path(*(this->currentTour));
}

path Tour::solveRandomWithSwitches(double maxLength, int maxTries) {
	this->currentTour = std::make_shared< path > (this->getRandomPath());
	double tourLength = this->getTourLength(*currentTour);

	int tries = 0;

	while (tourLength > maxLength && ++tries <= maxTries)
	{
		std::pair<int, int> randomIndices = this->pickRand(this->currentTour->size() - 1);
		path newSolution = swap(*(this->currentTour), randomIndices.first, randomIndices.second);
		double newTourLength = this->getTourLength(newSolution);
		if (newTourLength < tourLength)
		{
			tourLength = newTourLength;
			this->currentTour = std::make_shared< path > (newSolution);
			updateDisplay(-1.0, newTourLength);
		}
	}

	return path(*(this->currentTour));
}

path Tour::solveSimulatedAnnealing(double initialTemp, double finalTemp, double tempLoss) {
	this->currentTour = std::make_shared< path > (this->getRandomPath());
	double tourLength = this->getTourLength(*(this->currentTour));
	double currentTemperature = initialTemp;

	while (currentTemperature > finalTemp)
	{
		std::pair<int, int> randomIndices = this->pickRand(this->currentTour->size() - 1);
		path newSolution = swap(*(this->currentTour), randomIndices.first, randomIndices.second);
		double newTourLength = this->getTourLength(newSolution);
		if ( ( (double) rand() / (RAND_MAX) ) < (std::exp(tourLength - newTourLength) / currentTemperature) )
		{
			tourLength = newTourLength;
			this->currentTour = std::make_shared< path > (newSolution);
		}

		updateDisplay(currentTemperature, tourLength);
		currentTemperature -= tempLoss;
	}

	return path(*(this->currentTour));
}

std::shared_ptr< path >& Tour::getCurrentTour() {
	return this->currentTour;
}

const std::unique_ptr<std::thread>& Tour::getDisplayThread() const {
	return displayThread;
}

void Tour::printPath(const path path) {
	for (Graph::Edge edge : path)
			std::cout << edge.startVertex->name << "->" << edge.endVertex->name << " ";
	std::cout << std::endl;
}

Tour::~Tour() {
	if (this->display)
		this->display->kill();
}

void Tour::populateGraph(std::vector< CityLocation > cities, double xMax, double yMax) {
	// Populate graph with their distances
	for (auto startCity = cities.begin(); startCity < cities.end(); startCity++)
	{
		for (auto endCity = startCity + 1; endCity < cities.end(); endCity++)
		{
			double distance = std::sqrt(std::pow(endCity->second.x - startCity->second.x, 2) +
					std::pow(endCity->second.y - startCity->second.y, 2));
			this->changeEdgeWeight(distance, this->getVertex(startCity - cities.begin()),
					this->getVertex(endCity - cities.begin()));
		}
	}

	if (!disableGUI)
	{
		this->display = std::unique_ptr< Display > (new Display(cities, this->currentTour));

		if (xMax == -1 || yMax == -1)
		{
			this->display->maxWidth = Display::DEFAULT_SCALED_WIDTH; this->display->maxHeight = Display::DEFAULT_SCALED_HEIGHT;
		}
		else
		{
			this->display->maxWidth = xMax; this->display->maxHeight = yMax;
		}
		this->displayThread = std::unique_ptr< std::thread > (new std::thread(&Display::loop, std::ref( *(this->display) )));
	}
}

path Tour::getRandomPath() {
	srand (time(NULL));

	path randomPath;

	std::vector< int > verticesList(this->vertices.size());
	std::iota(std::begin(verticesList), std::end(verticesList), 0);
	std::random_shuffle(verticesList.begin() + 1, verticesList.end() - 1);

	for (auto it = verticesList.begin(); it < verticesList.end() - 1; it++)
		randomPath.push_back(this->graph[*it][*(it+1)]);
	randomPath.push_back(this->graph[*(verticesList.end() - 1)][*(verticesList.begin())]);
	return randomPath;
}

double Tour::getTourLength(path& tour) {
	double totalDist = 0;
	std::for_each(tour.begin(), tour.end(), [&](Graph::Edge edge) { totalDist += edge.distance; });
	return totalDist;
}

std::pair<int, int> Tour::pickRand(int size) {
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
path Tour::swap(const path& tour, int firstVertex, int secondVertex) {

	auto getEdge = [&](Vertex* v1, Vertex* v2) { return this->graph[v1->pos][v2->pos]; };

	std::vector< Graph::Vertex* > vertices;
	for (auto it = tour.begin(); it < tour.end(); it++)
		vertices.push_back(it->startVertex);
	vertices.push_back(tour.back().endVertex);

	path route1;
	for (auto it = vertices.begin(); it <= vertices.begin() + firstVertex - 1; it++)
		route1.push_back(getEdge(*it, *(it+1)));

	path route2;
	for (auto it = vertices.begin() + firstVertex; it < vertices.begin() + secondVertex; it++)
		route2.insert(route2.begin(), getEdge(*(it+1), *it));

	route1[route1.size() - 1] = getEdge(vertices[firstVertex - 1], vertices[secondVertex]);

	path route3;
	for (auto it = vertices.begin() + secondVertex + 1; it < vertices.end() - 1; it++)
		route3.push_back(getEdge(*it, *(it+1)));

	route3.insert(route3.begin(), getEdge(vertices[firstVertex], vertices[secondVertex + 1]));

	route1.reserve(route1.size() + route2.size() + route3.size());
	route1.insert(route1.end(), route2.begin(), route2.end());
	route1.insert(route1.end(), route3.begin(), route3.end());
	return route1;
}

void Tour::updateDisplay(double temperature, double distance) {

	if (!this->display)
		return;

	this->display->accessTour.lock();
	this->display->setCurrentTour(this->getCurrentTour());
	this->display->temperature = temperature;
	this->display->distance = distance;
	this->display->accessTour.unlock();
}
