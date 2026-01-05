#pragma once
#include "SFML/Graphics.hpp"
#include "Cursor.h"
#include <iostream>
#include <vector>
#include "utils.h"

class Buffer
{

public:
  Buffer();
  void init();
  void addCharacterToBuffer(Cursor &cursor, std::string strChar, int indexOfCursor, int currLineNumber);
  void insertAt(int pos, std::string str);
  int getLineLength(int lineNumber, sf::Font &font, int charSize);
  int getCharPosAt(int cursorcolumnpos, int linenumber, sf::Font &font, int charSize);
  sf::Vector2u undo(int lineN, sf::Font &font);
  void enterFunction(int index, int lineN, int cursorLineN, int colN, int m_characterSize);
  std::vector<std::string> &getInputBuffer(); // holds the line string
  std::vector<m_stdl::DelData> &getDeleteStack() { return deleteStack; }

private:
  std::vector<std::string> inputBuffer; // holds the line string
  std::vector<m_stdl::DelData> deleteStack;
  // input/action stack
};