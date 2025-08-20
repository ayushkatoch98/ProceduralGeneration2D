#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
#include <vector>
#include "TriangleC.h"


class DelauneyTriangulation{
public:
	std::vector<TriangleC> run(std::vector<Node*> nodes);
	TriangleC createSuperTriangle(std::vector<Node*> nodes);
};

