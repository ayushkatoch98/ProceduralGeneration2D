#include "TriangleC.h"
#include <vector>
#include <cmath>
#include <iostream>



TriangleC::TriangleC(sf::Vector2i a, sf::Vector2i b, sf::Vector2i c) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->setEdges();
    circumcircle = this->computeCircumcircle();
}

void TriangleC::setEdges() {
    this->edges.push_back(Edge(a, b));
    this->edges.push_back(Edge(a, c));
    this->edges.push_back(Edge(b, c));
}

bool TriangleC::isPointWithinCircumcircle(sf::Vector2i point) {
    
    if (!this->circumcircle.valid) return false;
    
    double dx = point.x - this->circumcircle.center.x;
    double dy = point.y - this->circumcircle.center.y;
    
    double distSquared = dx * dx + dy * dy;

    return distSquared < (this->circumcircle.radius * this->circumcircle.radius);
}


void TriangleC::render(sf::RenderWindow& window, bool renderCircumcircle) {

    int cellSize = WorldSettings::cellSize;

    sf::VertexArray lines(sf::PrimitiveType::Lines, 2);
    sf::VertexArray lines2(sf::PrimitiveType::Lines, 2);
    sf::VertexArray lines3(sf::PrimitiveType::Lines, 2);
    
    Edge e = edges[0];

    lines[1].position = sf::Vector2f({ (float) (e.a.x * cellSize), (float) (e.a.y * cellSize) });
    lines[0].position = sf::Vector2f({ (float) (e.b.x * cellSize), (float) (e.b.y * cellSize) });
    lines[1].color = this->color;
    lines[0].color = this->color;

    e = edges[1];

    lines2[1].position = sf::Vector2f({ (float)(e.a.x * cellSize), (float)(e.a.y * cellSize) });
    lines2[0].position = sf::Vector2f({ (float)(e.b.x * cellSize), (float)(e.b.y * cellSize) });
    lines2[1].color = this->color;
    lines2[0].color = this->color;

    e = edges[2];

    lines3[1].position = sf::Vector2f({ (float)(e.a.x * cellSize), (float)(e.a.y * cellSize) });
    lines3[0].position = sf::Vector2f({ (float)(e.b.x * cellSize), (float)(e.b.y * cellSize) });
    lines3[1].color = this->color;
    lines3[0].color = this->color;

    sf::CircleShape circle;
    circle.setRadius((float)this->circumcircle.radius * cellSize);
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(this->circumcircle.center * (float)cellSize);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Magenta);

    sf::RectangleShape rect;
    rect.setPosition(this->circumcircle.center * (float)cellSize);
    rect.setSize({ (float)cellSize, (float)cellSize });
    rect.setFillColor(sf::Color::Yellow);

    window.draw(lines);
    window.draw(lines2);
    window.draw(lines3);

    if (renderCircumcircle) {
        window.draw(rect);
        window.draw(circle);
    }

}

TriangleC::Circumcircle TriangleC::computeCircumcircle() {
    
    double x1 = a.x;
    double y1 = a.y;
    
    double x2 = b.x;
    double y2 = b.y;
    
    double x3 = c.x;
    double y3 = c.y;
    
    double D = 2 * ((x1 * (y2 - y3)) + (x2 * (y3 - y1)) + (x3 * (y1 - y2)));
    
    if (std::abs(D) < 1e-9) {
        std::cout << "Found invalid Triangle\n";
        return { {NAN, NAN}, NAN, false };
    }
    double part1, part2, part3;
    
    double x1Square = x1 * x1;
    double x2Square = x2 * x2;
    double x3Square = x3 * x3;
    
    double y1Square = y1 * y1;
    double y2Square = y2 * y2;
    double y3Square = y3 * y3;
    
    part1 = ((x1Square + y1Square) * (y2 - y3));
    part2 = ((x2Square + y2Square) * (y3 - y1));
    part3 = ((x3Square + y3Square) * (y1 - y2));
    
    double centerX = (part1 + part2 + part3) / D;
    
    part1 = ((x1Square + y1Square) * (x3 - x2));
    part2 = ((x2Square + y2Square) * (x1 - x3));
    part3 = ((x3Square + y3Square) * (x2 - x1));
    
    double centerY = (part1 + part2 + part3) / D;
    
    double aa = x1 - centerX;
    double bb = y1 - centerY;
    
    double radius = std::sqrt((aa * aa) + (bb * bb));

    return { {(float)centerX, (float)centerY}, (float)radius, true };
}
