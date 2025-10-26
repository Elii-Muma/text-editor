#pragma once
#include "SFML/Graphics.hpp"
#include "Cursor.h"

class Renderer{

  public:
    Renderer(sf::Font font);
    void init();
    void drawCursor(sf::RenderWindow &window, Cursor& cursor);

    void drawLineText(sf::RenderWindow &window, sf::String lineText, int rowPos);

  private:
    sf::Font m_font;
    sf::RectangleShape m_cursor;
    sf::Text m_lineText;
    int characterSize{22};

};