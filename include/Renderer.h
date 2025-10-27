#pragma once
#include "SFML/Graphics.hpp"
#include "Cursor.h"

class Renderer
{

public:
  Renderer(sf::Font font, sf::Vector2u *winSize);

  void drawCursor(sf::RenderWindow &window, Cursor &cursor);
  void drawLineText(sf::RenderWindow &window, sf::String lineText, int lineNumber, int rowPos);
  void drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber);
  void drawSideBorder(sf::RenderWindow &window);

private:
  sf::Font m_font;
  sf::RectangleShape m_cursor;
  sf::RectangleShape m_sideBorder;
  sf::Text m_lineText;
  sf::Text m_lineNumberText;
  int characterSize{22};

  int m_SIDE_BORDER_WIDTH{40};
  int m_SIDE_BORDER_HEIGHT{600};
  sf::Vector2u *m_WIN_SIZE;

  void init();
};