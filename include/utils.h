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
    // float charWidth = glyph.advance;  //this one includes spacing to next thingy
    int charWidth = static_cast<int>(glyph.advance);
    int charHeight = static_cast<int>(glyph.bounds.height);
    std::cout << "|--> char_info( " << character << ", " << charWidth << ")\n";

    std::pair<int, int> charDimensions = {charWidth, charHeight};
    return charDimensions;
  }

  inline int moveToCol(int colN, int lineN, int index, sf::Font font, int font_size , std::vector<std::vector<std::string>> buffer)
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

namespace themes{
  struct deep_blue_theme{
    sf::Color BLUE    {0x14213DFF};
    sf::Color YELLOW  {0xFCA311FF};
    sf::Color BLACK   {0x000000FF};
    sf::Color WHITE   {0xFFFFFFFF};
    sf::Color GREY    {0xE5E5E5FF};
  }inline deepBlue; 
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

  enum states{
    MAIN_SCREEN_STATE,
    TERMINAL_SCREEN_STATE,
    BOTTOM_SCREEN_STATE,
  };
}