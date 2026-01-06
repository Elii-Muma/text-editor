#pragma once
#include <SFML/Graphics.hpp>

class Cursor
{
public:

  Cursor() : lineN{0}, colN{0}{};
  Cursor(int lineNumber, int columnNumber);
  void moveCursorUp(int lineNumber);
  void moveCursorDown(int lineNumber);
  void moveCursorLeft(int cursorColumnPosition);
  void moveCursorRight(int cursorColumnPosition);
  void setPosition(int y, int x);
  void setScreenPosition(int x, int y);

  int getCursorPosLineNumber();
  int getCursorPosColumnNumber();
  int getLineNumber(int charSize); // returns the actual lineNumber(not in screen coordinates)
  sf::Vector2i getScreenPosition(); // returns yPos and xPosin screen coordinates
private:
  int lineN, colN;
  int xPos, yPos;
};