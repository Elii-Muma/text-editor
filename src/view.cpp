#include "view.h"

View::View(sf::Font &font, int &charSize, sf::Vector2u &windowSize) :m_font{font}, m_characterSize{charSize}, m_winSize{windowSize}, m_camera{charSize, windowSize}
{
  std::cout << "init view()" << "\n";
}

void View::renderFixedUI(sf::RenderWindow &window)
{
  std::cout << "not overriden fixedUI()\n";
}

void View::renderMoveableUI(sf::RenderWindow &window)
{
  std::cout << "not overriden moveableUI()\n";
}

// this is the moveTo method, but i cant access it from here because its in the ctrl-view
void View::updateCursorScreenPos()
{
  std::cout << "\n--updating_cursor_pos--\n";
  int colN  = m_cursor.getCursorPosColumnNumber();
  int lineN = m_cursor.getLineNumber(prev_fontSize);
  int index = m_buffer.getCharPosAt(colN, lineN, m_font, prev_fontSize);

  int size  = m_buffer.getInputBuffer()[lineN].size();
  if (index > size){
    index = size;
    std::cout << "_update_c_cond::index > size\n";
  }
  int newX{0};
  for (int i = 0; i < index; i++)
  {
    std::string cur_char{m_buffer.getInputBuffer()[lineN][i]};
    newX += TextUtils::getCharGlyphSize(*cur_char.c_str(), m_font, m_characterSize).first;
  }

  std::cout << "yPos [ "<< m_cursor.getScreenPosition().y << " ]\n";
  m_cursor.setScreenPosition(newX, m_cursor.getScreenPosition().y);
  m_cursor.setPosition(lineN, newX);
}

Buffer &View::getBuffer()
{
  return m_buffer;
}

Cursor &View::getCursor()
{
  return m_cursor;
}

Camera &View::getCamera()
{
  // TODO: insert return statement here
  return m_camera;
}
