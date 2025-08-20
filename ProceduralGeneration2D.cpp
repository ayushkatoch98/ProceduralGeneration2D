#pragma once
#include <SFML/Graphics.hpp>
#include "WorldSettings.h"
#include "GridVisualizer.h"
#include "BSP.h"
#include "Floor.h"
#include "DelauneyTriangulation.h"
#include "TriangleC.h"
#include "MST.h"
#include "RandomForest.h"
#include <iostream>



void printTree(Node* node, int depth = 0, std::string prefix = "") {
    if (!node) return;

    // Indentation based on depth
    for (int i = 0; i < depth; i++) std::cout << "   ";

    // Print node info
    std::cout << prefix
        << "[pos=(" << node->getPosition().x << "," << node->getPosition().y
        << ") size=(" << node->getSize().x << "x" << node->getSize().y << ")]\n";

    // Traverse children
    if (node->left || node->right) {
        printTree(node->left, depth + 1, "L-> ");
        printTree(node->right, depth + 1, "R-> ");
    }
}


std::vector<Edge> extractEdgesFromTrianges(std::vector<TriangleC> triangles) {

    std::vector<Edge> allEdges;
    for (int i = 0; i < triangles.size(); i++) {
        std::vector<Edge> temp = triangles[i].getEdges();
        allEdges.insert(allEdges.end(), temp.begin(), temp.end());
    }

    return allEdges;

}


int main() {


    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(WorldSettings::windowSize.x), (unsigned int)WorldSettings::windowSize.y }), "SFML Test");

    DelauneyTriangulation dt;
    MST mst;
    GridVisualizer gv;
    RandomForest rf;
    std::vector<Edge> allEdges;


    BSP* bsp = new BSP(WorldSettings::mapSize / WorldSettings::cellSize, WorldSettings::minSectionSize, WorldSettings::maxSectionSize);
    bsp->run();
    std::vector<TriangleC> triangles = dt.run(bsp->getLeafNodes());
    allEdges = mst.run(bsp->getLeafNodes(), extractEdgesFromTrianges(triangles));

    rf.run(bsp->getLeafNodes());
   

    sf::Clock clock1;
    sf::Clock clock2;
    sf::Clock clock3;

    clock1.start();
    clock2.start();
    clock3.start();

    float roomDelay = 0.0f;      // seconds between reveals
    float triangleDelay = 0.0f;      // seconds between reveals
    float edgesDelay = 0.0;      // seconds between reveals

    int shownRooms = 0;         
    int shownTriangles = 0;
    int shownEdges = 0;

    bool enabledRooms = true;
    bool enableTriangles = false;
    bool enabledEdges = false;

    while (window.isOpen()) {


        while (const std::optional event = window.pollEvent()){
            // Close window: exit

            const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();

            if (event->is<sf::Event::Closed>())
                window.close();
            else if (keyPressed)
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape || keyPressed->scancode == sf::Keyboard::Scancode::Q)
                    window.close();
                
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    delete bsp;
                    shownRooms = 0;
                    shownTriangles = 0;
                    shownEdges = 0;
                    bsp = new BSP(WorldSettings::mapSize / WorldSettings::cellSize, WorldSettings::minSectionSize, WorldSettings::maxSectionSize);
                    bsp->run();
                    std::vector<TriangleC> triangles = dt.run(bsp->getLeafNodes());
                    allEdges = extractEdgesFromTrianges(triangles);
                    mst.run(bsp->getLeafNodes(), allEdges);
                }
            }

        }

        std::vector<sf::RectangleShape> nodeShape = bsp->getRenderShapes(true, true);

        if (shownRooms >= nodeShape.size()) {
            enableTriangles = true;
        }

        if (shownTriangles >= triangles.size()) {
            enabledEdges = true;
        }

        if (shownEdges >= allEdges.size()) {
            enableTriangles = false;
        }


        // After every 2 seconds, reveal the next room
        if (clock1.getElapsedTime().asSeconds() > roomDelay && shownRooms < nodeShape.size()) {
            shownRooms += 2;
            clock1.restart();
        }


        if (clock3.getElapsedTime().asSeconds() > triangleDelay && shownTriangles < triangles.size() && enableTriangles) {
            shownTriangles++;
            clock3.restart();
        }

        if (clock2.getElapsedTime().asSeconds() > edgesDelay && shownEdges < allEdges.size() && shownTriangles >= triangles.size() && enabledEdges) {
            shownEdges++;
            clock2.restart();
        }

     

        window.clear();
        //gv.render(window);

        for (int i = 0; i < bsp->getLeafNodes().size(); i++) {
            bsp->getLeafNodes()[i]->renderRandomForest(window);
        }


        /*for (int i = 0; i < shownRooms; i++) {
            window.draw(nodeShape[i]);
        }*/

        for (int i = 0; i < shownTriangles && enableTriangles; i++) {
            triangles[i].render(window, false);
        }
        for (int i = 0; i < shownEdges && enabledEdges; i++) {
            allEdges[i].render(window);
        }

        window.display();
    }
      

    return 0;
}
