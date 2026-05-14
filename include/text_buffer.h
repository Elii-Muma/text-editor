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
  Buffer(bool isTerminal) : m_isTerminal{isTerminal} {}

  void  init();
  void  addCharacterToBuffer(Cursor &cursor, std::string strChar, int indexOfCursor, int currLineNumber);
  // void  insertAt(int pos, std::string str);
  void insertAt(int pos, std::string str, m_stdl::LineType lineType);
  int   getLineLength(int lineNumber, sf::Font &font, int charSize);
  int   getCharPosAt(int cursorcolumnpos, int linenumber, sf::Font &font, int charSize);
  sf::Vector2u undo(int lineN, sf::Font &font);
  void enterFunction(int index, int lineN, int cursorLineN, int colN, int m_characterSize, bool isTerminal);
  // std::vector<std::string> &getInputBuffer(); // holds the line string
  std::vector<std::pair<std::string, m_stdl::LineType>> &getInputBuffer();
  std::vector<m_stdl::DelData> &getDeleteStack() { return deleteStack; }

private:
  int   current_editable_line{0};
  bool  m_isTerminal{false};

  std::vector<std::pair<std::string, m_stdl::LineType>> inputBuffer; // holds the terminal line string AND flags
  std::vector<std::string>      t_inputBuffer; // holds the line string
  std::vector<m_stdl::DelData>  deleteStack;
};