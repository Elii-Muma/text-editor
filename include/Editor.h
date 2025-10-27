#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Cursor.h"

// HANDLES DS AND WHAT NOT
class Editor
{

public:
  Editor(sf::Font &font, int characterSize, sf::Vector2u *WIN_SIZE);
  void handleInput(sf::Event &ev);
  void update();
  void render(sf::RenderWindow &window);
  void EraseCharacter(bool isBackSpace, int colN, int index);
  int getCharPosAt();
  void setWinSize(sf::Vector2i WIN_SIZE);
  sf::Vector2i getWinSize();
  void cursorMoveUp();
  void cursorMoveDown();
  void cursorMoveLeft();
  std::pair<int, int> getCharGlyphSize(char character);
  void cursorMoveRight();
  void undoFunction();


private:
  int lineN;
  Cursor cursor;
  Renderer renderer;
  std::string string_buffer;
  bool isUndoPressed;
  // to change from here to some other single place
  sf::Font m_font;
  int m_characterSize;

  sf::Vector2u *m_WIN_SIZE;

  // ------DATA STRUCTURES------
  enum dir
  {
    LEFT,
    RIGHT
  };

  // will hold the deleted chracters
  enum StackType
  {
    NONE,
    LINE_DEL,
    NORMAL_DEL,
    NEW_INPUT,
    ENT_MOVE
  };


  struct DelData
  {
    std::string del_char;
    int index;
    int lineNumber;
    sf::Vector2f currCursorPos;
    StackType s_type{StackType::NONE};
  };

  std::vector<std::string> inputBuffer;    // holds the line string and the length of the line
  std::vector<int> inputLineLen;           // holds the line string and the length of the line
  // std::vector<sf::Text> inputRenderBuffer; //
  std::vector<DelData> deleteStack;        // holds deleted chars
};
