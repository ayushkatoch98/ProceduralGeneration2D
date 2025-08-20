#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "WorldSettings.h"



struct Edge {

public:
	sf::Vector2i a;
	sf::Vector2i b;
	float dist = 0;
	Node* nodeA;
	Node* nodeB;
	sf::Color color = sf::Color::Yellow;


	Edge(const sf::Vector2i& a, const sf::Vector2i& b) {
		if (a.x < b.x || (a.x == b.x && a.y < b.y)) {
			this->a = a;
			this->b = b;
		}
		else {
			this->a = b;
			this->b = a;
		}

		this->dist = calculateDistance();
	}

	// These nodes get SET by MST Algorithm run() method
	void setNodes(Node* nodeA, Node* nodeB) {
		this->nodeA = nodeA;
		this->nodeB = nodeB;
	}


	float calculateDistance() const {
		int dx = b.x - a.x;
		int dy = b.y - a.y;
		return std::sqrt(static_cast<float>(dx * dx + dy * dy));
	}

	void render(sf::RenderWindow& window) {

		int cellSize = WorldSettings::cellSize;

		sf::VertexArray lines(sf::PrimitiveType::Lines, 2);

		lines[1].position = sf::Vector2f({ (float)(a.x * cellSize), (float)(a.y * cellSize) });
		lines[0].position = sf::Vector2f({ (float)(b.x * cellSize), (float)(b.y * cellSize) });
		lines[1].color = this->color;
		lines[0].color = this->color;

		sf::RectangleShape rect({ (float)cellSize, (float)cellSize });
		rect.setPosition(sf::Vector2f({ (float)(a.x * cellSize), (float)(a.y * cellSize) }));
		rect.setFillColor(sf::Color::Green);

		sf::RectangleShape rect2({ (float)cellSize, (float)cellSize });
		rect2.setPosition(sf::Vector2f({ (float)(b.x * cellSize), (float)(b.y * cellSize) }));
		rect2.setFillColor(sf::Color::Green);

		window.draw(lines);
		window.draw(rect);
		window.draw(rect2);


	}

	
	bool isEqual(Edge const& e) const {
		return a == e.a && b == e.b;
	}

	bool operator==(Edge const& other) const {
		return isEqual(other);
	}
};

namespace std {
	template <>
	struct hash<Edge> {
		std::size_t operator()(const Edge& k) const {
			// Combine hashes of member variables
			std::size_t hashA = std::hash<int>()(k.a.x) ^ (std::hash<int>()(k.a.y) << 1);
			std::size_t hashB = std::hash<int>()(k.b.x) ^ (std::hash<int>()(k.b.y) << 1);
			return ((hashA ^ (hashB << 1)) >> 1);
		}
	};
};

class TriangleC {
public:

	struct Circumcircle {
		sf::Vector2f center;
		float radius = 0.f;
		bool valid = false;
	};

	sf::Color color = sf::Color::Red;


private:
    std::vector<Edge> edges;
    Circumcircle circumcircle;

public:

	sf::Vector2i a;
	sf::Vector2i b;
	sf::Vector2i c;

    TriangleC(sf::Vector2i a, sf::Vector2i b, sf::Vector2i c);
    void setEdges();
    bool isPointWithinCircumcircle(sf::Vector2i point);
	void render(sf::RenderWindow& window, bool renderCircumcircle);
	std::vector<Edge> getEdges() { return this->edges; }
	Circumcircle computeCircumcircle();

	// ordering of the triangle (orientation) matters
	// ABC = ABC but ABC != ACB
	bool operator==(TriangleC const& other) const {
		return a == other.a && b == other.b && c == other.c;
	}
};