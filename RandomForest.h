#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Node.h"

class RandomForest
{

public:
	void run(std::vector<Node*> nodes);
};

