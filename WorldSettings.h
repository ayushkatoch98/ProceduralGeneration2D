#pragma once  
#include <SFML/Graphics.hpp>  

class WorldSettings {  
public:  
  // Screen
  static const int screenSizeX = 1200;
  static const int screenSizeY = 1000;

  // Cell
  static const int cellSize = 5;

  // Map
  static const int mapSizeX = 1000;
  static const int mapSizeY = 1000;

  // BSP Sections (Cell Count)
  static const int minSectionX = 40;
  static const int minSectionY = 40;

  static const int maxSectionX = 50;
  static const int maxSectionY = 50;

  static const int mapOffsetToScreenBorderX = (int)((screenSizeX / (cellSize * 2)) - (mapSizeX / (cellSize * 2)));
  static const int mapOffsetToScreenBorderY = (int)((screenSizeY / (cellSize * 2)) - (mapSizeY / (cellSize * 2)));

  static const sf::Vector2i windowSize;
  static const sf::Vector2i mapSize;
  static const sf::Vector2i minSectionSize;
  static const sf::Vector2i maxSectionSize;

  virtual void virtualMethod() = 0;  
};  

