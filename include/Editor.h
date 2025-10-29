#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Cursor.h"
#include "Document.h"

// HANDLES DS AND WHAT NOT
class Editor
{

public:
  Editor(sf::Font &font, int characterSize, sf::Vector2u *WIN_SIZE);
  void render(sf::RenderWindow &window);
  void update();
  void handleInput(sf::Event &ev);
  void EraseCharacter(bool isBackSpace, int colN, int index);
  void saveFile();
  int getCharPosAt();
  int getLineLength(int lineNumber);
  void cursorMoveLeft();
  void cursorMoveToEnd();
  std::pair<int, int> getCharGlyphSize(char character);
  void cursorMoveRight();
  void undoFunction();


private:
  Cursor cursor;
  Renderer renderer;
  Document document;

  int lineN;
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

  //holds the filepaths, ill eventually move this to a different location
  //maybe make it a global file or something
  const struct FilePaths{
    std::string txt_filepath{"../resources/txt/"};
  } m_filePaths;

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

  std::vector<std::string> inputBuffer;    // holds the line string 
  std::vector<DelData> deleteStack;        // input/action stack
};
