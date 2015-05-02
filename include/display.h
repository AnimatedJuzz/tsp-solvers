#ifndef DISPLAY_H
#define DISPLAY_H

#include "graph.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>
#include <mutex>

typedef std::pair <std::string, sf::Vector2f> CityLocation;

class Display {
public:

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	int maxWidth = 1100;
	int maxHeight = 1100;

	const double OFFSET = 0.08;

	const std::string TITLE = "TSP Solver";

	const int CITY_SIZE = 5;
	const int CITY_OUTLINE_THICKNESS = 2;
	const int EDGE_THICKNESS = 3;
	const int TEXT_SIZE = 20;

	Display(const std::vector< CityLocation >& cities, std::shared_ptr <path> tour);
	void loop();
	void kill();
	void setCurrentTour(std::shared_ptr< path >);

	double temperature, distance;
	std::mutex accessTour;

	virtual ~Display();
private:
	void draw();
	sf::Vector2f const transformCoordinateSystem(const sf::Vector2f& coords);

	bool running;
	sf::RenderWindow * window;
	sf::Font * font;

	std::vector< CityLocation > cities;
	std::map < std::string, CityLocation > cityMap;
	std::shared_ptr< path > currentTour;
};

// Source: https://github.com/LaurentGomila/SFML/wiki/Source:-Line-segment-with-thickness
class sfLine : public sf::Drawable
{
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2, sf::Color color, float thickness):
        color(color), thickness(thickness)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i=0; i<4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices, 4, sf::Quads);
    }


private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};

#endif // DISPLAY_H
