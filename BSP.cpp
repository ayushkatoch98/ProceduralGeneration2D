#include "BSP.h"
#include "WorldSettings.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cassert>
#include <random>
#include <cmath>



BSP::BSP(sf::Vector2i mapSize, sf::Vector2i minSectionSize, sf::Vector2i maxSectionSize) {
	this->mapSize = mapSize;
	this->minSectionSize = minSectionSize;
	this->maxSectionSize = maxSectionSize;
	this->parentNode = new Node(mapSize, sf::Vector2i(WorldSettings::mapOffsetToScreenBorderX, WorldSettings::mapOffsetToScreenBorderY));

	assert(minSectionSize.x != maxSectionSize.x);
	assert(minSectionSize.y != maxSectionSize.y);
}

int BSP::getRandomIntCell(int min, int max) {
	if (min == max) return min;

	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

SplitDirection BSP::getSplitDirection(Node* node) {
	
    float ratio = (float)node->getSize().x / (float)node->getSize().y;

    if (ratio > 1.25f) {
        return SplitDirection::ROW;
    } else if (ratio < 0.8f) {
        return SplitDirection::COLUMN;
    }
    return (rand() % 2 == 0) ? SplitDirection::ROW : SplitDirection::COLUMN;

}

int BSP::getSplitPoint(Node* node, SplitDirection splitDir) {
	int splitPos = 0;

	if (splitDir == SplitDirection::ROW) {
		int ideal = node->getPosition().x + node->getSize().x / 2; // middle
		int minSplit = node->getPosition().x + minSectionSize.x;
		int maxSplit = node->getPosition().x + node->getSize().x - minSectionSize.x;
		if (maxSplit <= minSplit) return -1;

		int range = (maxSplit - minSplit) / 4;
		splitPos = this->getRandomIntCell(
			std::max(minSplit, ideal - range),
			std::min(maxSplit, ideal + range)
		);
	}
	else if (splitDir == SplitDirection::COLUMN) {
		int ideal = node->getPosition().y + node->getSize().y / 2;
		int minSplit = node->getPosition().y + minSectionSize.y;
		int maxSplit = node->getPosition().y + node->getSize().y - minSectionSize.y;
		if (maxSplit <= minSplit) return -1;

		int range = (maxSplit - minSplit) / 4;
		splitPos = this->getRandomIntCell(
			std::max(minSplit, ideal - range),
			std::min(maxSplit, ideal + range)
		);
	}

	return splitPos;

}

void BSP::findAllLeafNodes(Node* node) {

	if (!node) return;

	// Only draw leaf nodes
	if (node->left == nullptr && node->right == nullptr) {
		this->leafNodes.push_back(node);
	}

	// Recurse
	this->findAllLeafNodes(node->left);
	this->findAllLeafNodes(node->right);

}

std::vector<sf::RectangleShape> BSP::getRenderShapes(bool renderCenter, bool renderBox) {

	const int cellSize = WorldSettings::cellSize;

	std::vector<sf::RectangleShape> output;

	for (int i = 0; i < this->leafNodes.size(); i++) {
		Node* node = leafNodes[i];

		sf::Color blueColor = sf::Color::Blue;
		sf::Color cyanColor = sf::Color::Cyan;
		blueColor.a = 50;
		cyanColor.a = 50;


		sf::RectangleShape rect({ (float)((node->getSize().x) * cellSize), (float)(node->getSize().y * cellSize) });

		rect.setPosition({ (float)(node->getPosition().x * cellSize), (float)(node->getPosition().y * cellSize) });
		rect.setFillColor(blueColor);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);

		sf::RectangleShape centerRect({ (float)(cellSize), (float)(cellSize) });

		sf::Vector2i centerCoords = node->getCenter();
		centerRect.setPosition({ (float)(centerCoords.x * cellSize), (float)(centerCoords.y * cellSize) });
		centerRect.setOutlineColor(sf::Color::Green);
		centerRect.setOutlineThickness(1);
		
		if (renderCenter)
			output.push_back(centerRect);

		if (renderBox)
			output.push_back(rect);

	}


	return output;

}
void BSP::recursion(Node* node) {
	if (node == nullptr) return;

	if (this->minSectionSize.x >= node->getSize().x || this->minSectionSize.y >= node->getSize().y) return;

	const int cellSize = WorldSettings::cellSize;


	SplitDirection splitDir = this->getSplitDirection(node);

	if (splitDir == SplitDirection::ROW) {

		int splitX = this->getSplitPoint(node, splitDir);
		if (splitX == -1) return;


		sf::Vector2i lnSize(splitX - node->getPosition().x, node->getSize().y);
		sf::Vector2i lnPosition(node->getPosition());

		sf::Vector2i rnSize(node->getSize().x - (splitX - node->getPosition().x), node->getSize().y);
		sf::Vector2i rnPosition(splitX, node->getPosition().y);


		if (lnSize.x < minSectionSize.x || rnSize.x < minSectionSize.x) return;


		node->left = new Node(lnSize, lnPosition);
		node->right = new Node(rnSize, rnPosition);

	}

	if (splitDir == SplitDirection::COLUMN) {

		int splitY = this->getSplitPoint(node, splitDir);
		if (splitY == -1) return;


		sf::Vector2i lnSize(node->getSize().x, splitY - node->getPosition().y);
		sf::Vector2i lnPosition(node->getPosition());

		sf::Vector2i rnSize(node->getSize().x, node->getSize().y - (splitY - node->getPosition().y));
		sf::Vector2i rnPosition(node->getPosition().x, splitY);


		if (lnSize.y < minSectionSize.y || rnSize.y < minSectionSize.y) return;


		node->left = new Node(lnSize, lnPosition);
		node->right = new Node(rnSize, rnPosition);

	}


	this->recursion(node->left);
	this->recursion(node->right);
}
void BSP::run() {

	this->recursion(this->parentNode);
	this->findAllLeafNodes(this->parentNode);

	for (int i = 0; i < this->leafNodes.size(); i++) {
		leafNodes[i]->setSize(leafNodes[i]->getSize().x - getRandomIntCell(5 , 10), leafNodes[i]->getSize().y - getRandomIntCell(5, 10));
	}
	
}


void BSP::traverse(Node* node, int depth, int requiredDepth, sf::RenderWindow& window) {

	const int cellSize = WorldSettings::cellSize;

	if (!node) return;

	// Only draw leaf nodes
	if (node->left == nullptr && node->right == nullptr) {
		sf::Color blueColor = sf::Color::Blue;
		sf::Color cyanColor = sf::Color::Cyan;
		blueColor.a = 50;
		cyanColor.a = 50;

		sf::RectangleShape rect({ (float)(node->getSize().x * cellSize), (float)(node->getSize().y * cellSize) });

		rect.setPosition({ (float)(node->getPosition().x * cellSize), (float)(node->getPosition().y * cellSize)});
		rect.setFillColor(blueColor);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(1);

		sf::RectangleShape centerRect({ (float)(cellSize), (float)(cellSize) });

		sf::Vector2i centerCoords = node->getCenter();
		centerRect.setPosition({ (float)(centerCoords.x * cellSize), (float)(centerCoords.y * cellSize) });
		centerRect.setOutlineColor(sf::Color::Green);
		centerRect.setOutlineThickness(1);

		window.draw(rect);
		window.draw(centerRect);

	}

	// Recurse
	traverse(node->left, depth + 1, requiredDepth, window);
	traverse(node->right, depth + 1, requiredDepth, window);
}


