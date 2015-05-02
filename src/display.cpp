#include "display.h"

Display::Display(const std::vector< CityLocation >& cities, std::shared_ptr <path> tour) :
window(new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), TITLE)),
font(new sf::Font),
cities(cities),
running(true),
currentTour(tour),
cityMap(),
temperature(-1),
distance(-1),
accessTour() {
	if (!this->font->loadFromFile("assets/fonts/Inconsolata.otf")) {
		std::cerr << "Failed to load font" << std::endl;
	}

	for (CityLocation city : cities)
		this->cityMap[city.first] = city;
}

void Display::loop() {
	while (this->window->isOpen() && running)
	{
		sf::Event event;
		while (this->window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->window->close();
		}

		window->clear(sf::Color::White);

		this->draw();

		window->display();
	}
}

void Display::draw() {
	for (CityLocation city : this->cities)
	{
		sf::CircleShape city_image(CITY_SIZE);
		city_image.setFillColor(sf::Color::Transparent);

		city_image.setOutlineThickness(CITY_OUTLINE_THICKNESS);
		city_image.setOutlineColor(sf::Color::Black);

		sf::Vector2f cityPos = this->transformCoordinateSystem(city.second);
		city_image.setPosition(cityPos.x - CITY_SIZE/2, cityPos.y - CITY_SIZE/2);

		this->window->draw(city_image);
	}

	this->accessTour.lock();
	path tour = *(this->currentTour);
	this->accessTour.unlock();

	for (Graph::Edge e : tour)
	{
		if (e.startVertex == NULL || e.endVertex == NULL || e.startVertex->name.empty() || e.endVertex->name.empty())
			continue;

		sf::Vector2f edge[] =
		{
			this->transformCoordinateSystem(this->cityMap[e.startVertex->name].second),
			this->transformCoordinateSystem(this->cityMap[e.endVertex->name].second)
		};
		sfLine line(edge[0], edge[1], sf::Color::Black, EDGE_THICKNESS);
		this->window->draw(line);
	}

	sf::Text temp("Temperature " + std::to_string(this->temperature), *(this->font));
	temp.setColor(sf::Color::Black);
	temp.setPosition(10, 10);

	sf::Text distanceText("Distance " + std::to_string(this->distance), *(this->font));
	distanceText.setColor(sf::Color::Black);
	distanceText.setPosition(10, HEIGHT - 35);

	this->window->draw(temp);
	this->window->draw(distanceText);
}

void Display::kill() {
	this->running = false;
}

void Display::setCurrentTour(std::shared_ptr< path > currentTour) {
	this->currentTour = currentTour;
}

sf::Vector2f const Display::transformCoordinateSystem(const sf::Vector2f& coords) {
	return sf::Vector2f ((coords.x / ((float) maxWidth) ) * WIDTH,
			(1.0 - (coords.y / ((float) maxHeight)) ) * HEIGHT);
}

Display::~Display() {
	this->running = false;
	this->window->close();
	delete this->window;
	delete this->font;
}
