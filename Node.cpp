#include "Node.h"
#include <cmath>
#include <iostream>
#include "WorldSettings.h"


Node::Node(sf::Vector2i size, sf::Vector2i position)
    : size(size), position(position), left(nullptr), right(nullptr),
    entryConnected(false), exitConnected(false),
    entryConnectedNode(nullptr), exitConnectedNode(nullptr)
{
    roomSize = { size.x - 30, size.y - 30 };
    setCenter();
}

Node Node::setLeft(Node leftNode) {
    left = &leftNode; 
    return leftNode;
}

Node Node::setRight(Node rightNode) {
    right = &rightNode; 
    return rightNode;
}

sf::Vector2i Node::getSize() {
    return size;
}

void Node::setSize(int x, int y) {
    size = sf::Vector2i(x, y);
    setCenter();
}

void Node::setPosition(int x, int y) {
    position = sf::Vector2i(x, y);
    setCenter();
}

sf::Vector2i Node::getPosition() {
    return position;
}

void Node::setCenter() {
    int x = static_cast<int>(std::floor(size.x / 2.0f));
    int y = static_cast<int>(std::floor(size.y / 2.0f));
    centerCoords = sf::Vector2i(position.x + x, position.y + y);
}

sf::Vector2i Node::getCenter() {
    return centerCoords;
}

void Node::resizeOccupiedCells() {
    occupiedCells.resize(size.x);

    for (int i = 0; i < occupiedCells.size(); i++) {
        occupiedCells[i].resize(size.y);
    }
}

void Node::setOccupiedCell(int x, int y, int val) {

 /*   std::cout << "x: " << x << ", y: " << y << std::endl;
    std::cout << "Node size: (" << size.x << ", " << size.y << ")" << std::endl;
    std::cout << "occupiedCells.size(): " << occupiedCells.size() << std::endl;
    if (!occupiedCells.empty()) {
        std::cout << "occupiedCells[0].size(): " << occupiedCells[0].size() << std::endl;
    }
    else {
        std::cout << "occupiedCells[0].size(): [empty]" << std::endl;
    }*/


    assert(x >= 0 && x < occupiedCells.size());
    assert(y >= 0 && y < occupiedCells[0].size());

    occupiedCells[x][y] = val;
}

int Node::getOccupiedCell(int x, int y) {
    std::cout << "X & Y (" << x << ", " << y << ")\n";
    assert(x >= 0 && x < occupiedCells.size());
    assert(y >= 0 && y < occupiedCells[0].size());

    return occupiedCells[x][y];
}

void Node::renderRandomForest(sf::RenderWindow &window) {

    int cellSize = WorldSettings::cellSize;

    for (int i = 0; i < occupiedCells.size(); i++) {
        for (int j = 0; j < occupiedCells[0].size(); j++) {
            if (occupiedCells[i][j] != 1) continue;

            sf::RectangleShape r({ (float)cellSize, (float)cellSize });
            r.setPosition({ (float)((i + position.x) * cellSize) , (float)((j + position.y)*cellSize) });
            r.setFillColor(sf::Color::Red);
            /*r.setOutlineColor(sf::Color::Red);
            r.setOutlineThickness(1);*/
            
            window.draw(r);

        }
    }

}


Node::~Node() {
    delete left;
    delete right;
}
