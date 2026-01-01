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
  void drawBottomBorder(sf::RenderWindow & window);

private:
  sf::Font m_font;
  sf::RectangleShape m_cursor;
  sf::RectangleShape m_sideBorder;
  sf::RectangleShape m_bottomBorder;
  sf::Text m_lineText;
  sf::Text m_lineNumberText;
  int characterSize{22};

  int m_SIDE_BORDER_WIDTH{60};
  int m_SIDE_BORDER_HEIGHT;
  int m_BOTTOM_BORDER_WIDTH;
  int m_BOTTOM_BORDER_HEIGHT{30};
  int Y_OFFSET{5};
  sf::Vector2u *m_WIN_SIZE;

  void init();
};