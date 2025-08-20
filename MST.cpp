#include "MST.h"
#include <iostream>


std::vector<Edge> MST::run(std::vector<Node*> nodes, std::vector<Edge> edges) {
    
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
        return e1.dist < e2.dist;
    });

    std::vector<Edge> mstEdges;

    DSU dsu(nodes);

    for (int i = 0; i < edges.size(); i++) {


        Node* nodeA = nullptr;
        Node* nodeB = nullptr;

        for (int j= 0; j< nodes.size(); j++){
            sf::Vector2i cent = nodes[j]->getCenter();
            if (cent == edges[i].a) nodeA = nodes[j];
            if (cent == edges[i].b) nodeB = nodes[j];
        }

        if (!nodeA || !nodeB) continue;

        edges[i].setNodes(nodeA, nodeB);

        if (dsu.find(nodeA) != dsu.find(nodeB)) {
            dsu.unite(nodeA, nodeB);
            std::cout << "Adding EDGE MST" << std::endl;
            mstEdges.push_back(edges[i]);
        }
    }


    return mstEdges;

}