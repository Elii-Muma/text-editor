#include "Renderer.h"
#include <iostream>

Renderer::Renderer(sf::Font font, sf::Vector2u *winSize) : m_font{font}, m_WIN_SIZE(winSize)
{
  init();
}

void Renderer::init()
{
  int CURSOR_YOFFSET = 3;
  int CURSOR_WIDTH = 2;
  int CURSOR_HEIGHT = characterSize + CURSOR_YOFFSET;
  m_cursor.setFillColor(sf::Color(0, 255, 0));
  m_cursor.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  m_lineText.setFont(m_font);
  m_lineText.setFillColor(sf::Color::Yellow);
  m_lineText.setCharacterSize(characterSize);

  m_lineNumberText.setFont(m_font);
  m_lineNumberText.setFillColor(sf::Color::White);
  m_lineNumberText.setCharacterSize(characterSize);

  m_sideBorder.setFillColor(sf::Color(130, 130, 130));
  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_WIN_SIZE->y));
  m_sideBorder.setPosition(0, 0);
}

void Renderer::drawCursor(sf::RenderWindow &window, Cursor &cursor)
{
  m_cursor.setPosition(cursor.getCursorPosColumnNumber() + m_SIDE_BORDER_WIDTH, cursor.getCursorPosLineNumber());
  window.draw(m_cursor);
}

void Renderer::drawLineText(sf::RenderWindow &window, sf::String lineText, int lineNumber, int rowPos)
{
  m_lineText.setPosition(m_SIDE_BORDER_WIDTH, rowPos);
  m_lineText.setString(lineText);
  window.draw(m_lineText);
}

void Renderer::drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber)
{

  if (activeLineNumber == lineNumber)
  {
    m_lineNumberText.setFillColor(sf::Color(0, 255, 0));
    m_lineNumberText.setPosition(10, numPos);
  }
  else{
    m_lineNumberText.setFillColor(sf::Color::White);
    m_lineNumberText.setPosition(5, numPos);
  }

  m_lineNumberText.setString(std::to_string(lineNumber + 1));

  window.draw(m_lineNumberText);
}

void Renderer::drawSideBorder(sf::RenderWindow &window)
{
  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_WIN_SIZE->y));
  window.draw(m_sideBorder);
}