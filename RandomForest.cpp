#include "RandomForest.h"
#include <random>

// Returns a random direction as an sf::Vector2i: up, down, left, or right
sf::Vector2i getRandomDirection() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 3);

    int dir = dist(gen);
    switch (dir) {
        case 0: return sf::Vector2i(0, -1); // Up
        case 1: return sf::Vector2i(0, 1);  // Down
        case 2: return sf::Vector2i(-1, 0); // Left
        case 3: return sf::Vector2i(1, 0);  // Right
        default: return sf::Vector2i(0, 0);
    }
}
void RandomForest::run(std::vector<Node*> nodes) {

	int iter = 4000;

	for (int i = 0; i < nodes.size(); i++) {

		Node* node = nodes[i];
		node->resizeOccupiedCells();

		sf::Vector2i size = node->getSize();
		sf::Vector2i currPos = node->getCenter();
		currPos.x -= node->getPosition().x;
		currPos.y -= node->getPosition().y;
		//sf::Vector2i currPos(0, 0);

		node->setOccupiedCell(currPos.x, currPos.y, 1);

		if (size.x == 0 || size.y == 0) return;

		for (int l = 0; l < iter; l++) {
			
			sf::Vector2i dir = getRandomDirection();

			if (currPos.x + dir.x < 0 || currPos.x + dir.x >= size.x || currPos.y + dir.y < 0 || currPos.y + dir.y >= size.y) {
				std::cout << "OUT OF BOUND\n";
				continue;
			}

			currPos.x += dir.x;
			currPos.y += dir.y;

			node->setOccupiedCell(currPos.x, currPos.y, 1);

		}
	}



}