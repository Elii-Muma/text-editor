#include "view.h"

View::View(Camera &camera, sf::Font &font, int &charSize, sf::Vector2u &windowSize) : m_camera{camera}, m_font{font}, m_characterSize{charSize}, m_winSize{windowSize}
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
