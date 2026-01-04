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
  void setPosition(int lineN, int colN);

  int getCursorPosLineNumber();
  int getCursorPosColumnNumber();
  int getLineNumber(int charSize); // returns the actual lineNumber(not in screen coordinates)
  std::pair<int, int> getPosition(); // returns lineNumber and colNumber in screen coordinates
private:
  int lineN, colN;
};