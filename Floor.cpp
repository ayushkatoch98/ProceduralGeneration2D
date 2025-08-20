#include "Floor.h"


Floor::Floor(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color)
{
	obj.setPosition(position);
	obj.setSize(size);
	obj.setFillColor(color);
}

void Floor::render(sf::RenderWindow& window)
{
	window.draw(obj);
}