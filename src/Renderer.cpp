#include "Renderer.h"
#include <iostream>

Renderer::Renderer(sf::Font font) : m_font{font}{
  init();
}

void Renderer::init(){
  int CURSOR_WIDTH = 2;
  int CURSOR_HEIGHT = 22;
  m_cursor.setFillColor(sf::Color::Yellow);
  m_cursor.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  m_lineText.setFont(m_font);
  m_lineText.setFillColor(sf::Color::Yellow);
  m_lineText.setCharacterSize(characterSize);
}

void Renderer::drawCursor(sf::RenderWindow &window, Cursor& cursor)
{
  m_cursor.setPosition(cursor.getCursorPosColumnNumber(), cursor.getCursorPosLineNumber());
  window.draw(m_cursor);
}

void Renderer::drawLineText(sf::RenderWindow& window, sf::String lineText, int rowPos){
  m_lineText.setPosition(0, rowPos);
  m_lineText.setString(lineText);
  window.draw(m_lineText);
}
