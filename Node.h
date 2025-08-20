#pragma once

#include <SFML/Graphics.hpp>

class Node {
private:
    sf::Vector2i size;
    sf::Vector2i position;
    sf::Vector2i centerCoords;
    std::vector<std::vector<int>> occupiedCells;


    void setCenter();

public:
    sf::Vector2i roomSize;

    Node* left;
    Node* right;

    bool entryConnected;
    bool exitConnected;

    Node* entryConnectedNode;
    Node* exitConnectedNode;

    // Constructor
    Node(sf::Vector2i size, sf::Vector2i position);

    // Setters / Getters
    Node setLeft(Node leftNode);
    Node setRight(Node rightNode);
    void setSize(int x, int y);
    void setPosition(int x, int y);

    sf::Vector2i getSize();
    sf::Vector2i getPosition();
    sf::Vector2i getCenter();

    void resizeOccupiedCells();
    void setOccupiedCell(int x, int y, int val);
    int getOccupiedCell(int x, int y);

    void renderRandomForest(sf::RenderWindow& window);

    bool operator==(Node const& other) const {
        return size == other.size && position == other.position && centerCoords == other.centerCoords;
    }


    // Destructor
    ~Node();
};



namespace std {
    template <>
    struct hash<sf::Vector2i> {
        size_t operator()(const sf::Vector2i& v) const noexcept {
            return hash<int>()(v.x) ^ (hash<int>()(v.y) << 1);
        }
    };

    template <>
    struct hash<Node> {
        size_t operator()(Node& n) const noexcept {
            size_t h1 = hash<sf::Vector2i>()(n.getSize());
            size_t h2 = hash<sf::Vector2i>()(n.getPosition());
            size_t h3 = hash<sf::Vector2i>()(n.getCenter());
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}