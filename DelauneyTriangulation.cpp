#include "DelauneyTriangulation.h"
#include <cmath>
#include <iostream>
#include "WorldSettings.h"



TriangleC DelauneyTriangulation::createSuperTriangle(std::vector<Node*> nodes) {

	int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;


	for (int i = 0; i < nodes.size(); i++) {
		Node* node = nodes[i];

		sf::Vector2i cent = node->getCenter();

		// Top Left Corner
		if ((cent.x < minX) || (cent.x == minX && cent.y < minY)) {
			minX = cent.x;
			minY = cent.y;
		}

		// Bottom Right Corner
		if ((cent.x > maxX) || (cent.x == maxX && cent.y > maxY)) {
			maxX = cent.x;
			maxY = cent.y;
		}

	}

	int dx = maxX - minX;
	int dy = maxY - minY;
	int d = std::max(dx, dy);

	// calculating Center
	int cx = (int)floor((minX + maxX) / 2);
	int cy = (int)floor((minY + maxY) / 2);

	float k = 2;

	// Creating Triangle
	sf::Vector2i a({ (int)(cx - 2 * k * d), (int)(cy - k * d) });
	sf::Vector2i b({ cx, (int)(cy + 2 * k * d) });
	sf::Vector2i c({ (int)(cx + 2 * k * d), (int)(cy - k * d) });

	TriangleC triangle(a, b, c);

	return triangle;

}

std::vector<TriangleC> DelauneyTriangulation::run(std::vector<Node*> nodes) {

	std::vector<TriangleC> triangles;
	std::vector<TriangleC> output;

	TriangleC superTriangle = this->createSuperTriangle(nodes);
	std::vector<Edge> superTriangleEdges = superTriangle.getEdges();
	triangles.push_back(superTriangle);

	for (int i = 0; i < nodes.size(); i++) {
		Node* node = nodes[i];
		
		std::vector<TriangleC> badTriangles;
		std::unordered_map<Edge, int> edgesCountMapping;

		// loop all triangles and find BAD TRIANGLES
		for (int j = 0; j < triangles.size(); j++){
			if (triangles[j].isPointWithinCircumcircle(node->getCenter())) {

				badTriangles.push_back(triangles[j]);

				std::vector<Edge> triEdges = triangles[j].getEdges();
				// mapping edges of the triangle
				for (int k = 0; k < triEdges.size(); k++)
					edgesCountMapping[triEdges[k]]++;
			}
		}

		// Create new triangles using the EDGES and node.position
		for (const auto& [edge, value] : edgesCountMapping) {
			if (value != 1) continue;        
			
			TriangleC t = TriangleC(edge.a, edge.b, node->getCenter());
			triangles.push_back(t);
		}

		// removing bad triangles
		for (int j = 0; j < badTriangles.size(); j++) {
			triangles.erase(std::remove(triangles.begin(), triangles.end(), badTriangles[j]), triangles.end());
		}

	}

	// removing triangles that are touching edges of super triangle
	for (int i = 0; i < triangles.size(); i++) {

		TriangleC tri = triangles[i];
		
		bool part1 = tri.a == superTriangle.a || tri.a == superTriangle.b || tri.a == superTriangle.c;
		bool part2 = tri.b == superTriangle.a || tri.b == superTriangle.b || tri.b == superTriangle.c;
		bool part3 = tri.c == superTriangle.a || tri.c == superTriangle.b || tri.c == superTriangle.c;

		if (part1 || part2 || part3) {
			triangles[i].color = sf::Color::Green;
			std::cout << "Removing Triangle\n";
		}
		else output.push_back(triangles[i]);
	}

	return output;

}