#pragma once
#include <SFML/Graphics.hpp>

class Cursor
{
public:
  const float CURSOR_WIDTH = 2.f;
  const float CURSOR_HEIGHT = 30.f;
  void getTest();
private:
  sf::RectangleShape cursor;
};