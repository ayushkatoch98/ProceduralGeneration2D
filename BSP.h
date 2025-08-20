#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"

//#include <cstdlib>
//#include <ctime>

enum SplitDirection {
	ROW,
	COLUMN
};


class BSP
{
private:
	sf::Vector2i mapSize;
	sf::Vector2i minSectionSize;
	sf::Vector2i maxSectionSize;
	sf::Vector2i roomBorderOffset;
	Node* parentNode;
	std::vector<Node*> leafNodes;
	sf::Clock clock;
	int edgesToShow = 0;

	void recursion(Node* node);

public:
	int counter = 0;
	BSP(sf::Vector2i mapSize, sf::Vector2i minSectionSize, sf::Vector2i maxRoomsize);
	int getRandomIntCell(int min, int max);
	SplitDirection getSplitDirection(Node* node);
	int getSplitPoint(Node* node, SplitDirection splitDir);
	std::vector<Node*> getLeafNodes() { return this->leafNodes; }
	void findAllLeafNodes(Node* node);
	std::vector<sf::RectangleShape> getRenderShapes(bool renderCenter, bool renderBox);
	void run();
	void traverse(Node* node, int depth, int requiredDepth, sf::RenderWindow& window);
	Node* getParentNode() { return this->parentNode; }
};

