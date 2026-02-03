#pragma once
#include <utility>
#include "SFML/Graphics.hpp"
#include "camera.h"
#include "Cursor.h"
#include "text_buffer.h"

namespace TextUtils
{
  // returns a pair of ints which are the dimensions of the character
  // TODO: have it check for more than one character...maybe a string and iterate over it
  inline std::pair<int, int> getCharGlyphSize(char character, sf::Font &font, int characterSize)
  {
    sf::Glyph glyph = font.getGlyph(character, characterSize, false);
    int charWidth   = static_cast<int>(glyph.advance);
    int charHeight  = static_cast<int>(glyph.bounds.height);

    std::pair<int, int> charDimensions = {charWidth, charHeight};
    return charDimensions;
  }

  inline int moveToCol(int colN, int lineN, int index, sf::Font font, int font_size, std::vector<std::vector<std::string>> buffer)
  {
    // this is the moveTo method, but i cant access it from here because its in the ctrl-view
    int size = buffer[lineN].size();
    if (index > size)
      index = size;
    int newX{0};
    for (int i = 0; i < index; i++)
    {
      std::string cur_char{buffer[lineN][i]};
      newX += getCharGlyphSize(*cur_char.c_str(), font, font_size).first;
    }
    return newX;
  }
}

namespace themes
{
  struct theme
  {
    sf::Color BACKGROUND;
    sf::Color SECONDARY;
    sf::Color ACCENT;
    sf::Color HIGHLIGHT;
    sf::Color TEXT_COLOR;
  };
  inline theme deepBlue{
      .BACKGROUND = sf::Color(0x14213DFF),
      .SECONDARY = sf::Color(0x000000FF),
      .ACCENT = sf::Color(0xFCA311FF),
      .HIGHLIGHT = sf::Color(0xFFFFFFFF),
      .TEXT_COLOR = sf::Color(0xE5E5E5FF)};
  /**
  punch-red: #e63946ff;
  honeydew: #f1faeeff;
  frosted-blue: #a8dadcff;
  cerulean: #457b9dff;
  oxford-navy: #1d3557ff;
   k*/
  inline theme oceanBreeze{
      .BACKGROUND = sf::Color(0x1d3557ff),
      .SECONDARY = sf::Color(0x457b9dff),
      .ACCENT = sf::Color(0xe63946ff),
      .HIGHLIGHT = sf::Color(0xa8dadcff),
      .TEXT_COLOR = sf::Color(0xf1faeeff)};
  /**
   Muted Earthy Tones
      FFCDB2

      FFB4A2

      E5989B

      B5838D

      6D6875
   */
  inline theme mutedEarth{
      .BACKGROUND = sf::Color(0x6D6875FF),
      .SECONDARY = sf::Color(0xB5838DFF),
      .ACCENT = sf::Color(0xE5989BFF),
      .HIGHLIGHT = sf::Color(0xFFB4A2FF),
      .TEXT_COLOR = sf::Color(0xFFCDB2FF)};

  inline theme dreamland{
      .BACKGROUND = sf::Color(0xcdb4dbff),
      .SECONDARY = sf::Color(0xbde0feff),
      .ACCENT = sf::Color(0xffafccff),
      .HIGHLIGHT = sf::Color(0xa2d2ffff),
      .TEXT_COLOR = sf::Color(0x1d1d1dff)};
}

// my std library
namespace m_stdl
{

  enum StackType
  {
    NONE,
    LINE_DEL,
    NORMAL_DEL,
    NEW_INPUT,
    ENT_MOVE
  };

  // structure that the deleted data has
  struct DelData
  {
    std::string del_char;              // deleted character
    int index;                         // index of cursor in context of the text in line
    int lineNumber;                    // current line number
    sf::Vector2f currCursorPos;        // the cursors position coordinates
    StackType s_type{StackType::NONE}; //"delete type" idk why i called it stacktType
  };

  enum states
  {
    MAIN_SCREEN_STATE,
    TERMINAL_SCREEN_STATE,
    BOTTOM_SCREEN_STATE,
  };
}