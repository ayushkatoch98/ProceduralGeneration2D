#pragma once

#include <SFML/Graphics.hpp>

class Floor
{
public:
	sf::RectangleShape obj;
	Floor(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color = sf::Color::White);
	void render(sf::RenderWindow& window);

};

