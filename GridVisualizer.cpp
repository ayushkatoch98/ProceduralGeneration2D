#include "GridVisualizer.h"
#include <iostream>
#include <stdexcept> // for std::runtime_error


GridVisualizer::GridVisualizer() {

	this->windowSize = WorldSettings::windowSize;
	this->cellSize = WorldSettings::cellSize;

	if (windowSize.x % cellSize != 0 or windowSize.y % cellSize != 0) {
		std::cerr << "Error: Division of windowSize / cellSize must be an integer not float\n";
	}

	this->cellCount = { (int)(windowSize.x / cellSize), (int)(windowSize.y / cellSize) };

}


void GridVisualizer::render(sf::RenderWindow& window) {


	for (int i = 0; i < this->cellCount.x; i++) {
		for (int j = 0; j < this->cellCount.y; j++) {

			sf::RectangleShape cell({ (float)this->cellSize, (float)this->cellSize });
			cell.setPosition({ (float)i * cellSize, (float)j * cellSize });
			cell.setOutlineColor(sf::Color::White);
			cell.setOutlineThickness(1);
			cell.setFillColor(sf::Color::Black);
			window.draw(cell);

		}
	}

}


