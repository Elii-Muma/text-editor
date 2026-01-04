#pragma once
#include <utility>
#include "SFML/Graphics.hpp"
#include "camera.h"
#include "Cursor.h"
#include "text_buffer.h"

namespace TextUtils
{
  // returns a pair of ints which are the dimensions of the character
  inline std::pair<int, int> getCharGlyphSize(char character, sf::Font &font, int characterSize)
  {
    sf::Glyph glyph = font.getGlyph(character, characterSize, false);
    // float charWidth = glyph.advance;  //this one includes spacing to next thingy
    int charWidth = static_cast<int>(glyph.advance);
    int charHeight = static_cast<int>(glyph.bounds.height);

    std::pair<int, int> charDimensions = {charWidth, charHeight};
    return charDimensions;
  }
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
}