#pragma once
#include <SFML/Graphics.hpp>
#include "WorldSettings.h"

class GridVisualizer
{
private:
	sf::Vector2i windowSize;
	sf::Vector2i cellCount;
	int cellSize;

public:
	GridVisualizer();
	void render(sf::RenderWindow& window);
};

