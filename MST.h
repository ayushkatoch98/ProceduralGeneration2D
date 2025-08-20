#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "TriangleC.h"


struct DSU {

	std::unordered_map<Node*, Node*> set;

	DSU(std::vector<Node*> nodes) {
		for (int i = 0; i < nodes.size(); i++) {
			set[nodes[i]] = nodes[i];
		}
	}

	void unite(Node* a, Node* b) {
		Node* rootA = find(a);
		Node* rootB = find(b);
		if (rootA != rootB) {
			set[rootB] = rootA;  // merge sets
		}
	}

	Node* find(Node* x) {

		if (x == set[x]) return x;

		return find(set[x]);

	}
};

class MST{
	
public:
	std::vector<Edge> run(std::vector<Node*> nodes, std::vector<Edge> edges);
};