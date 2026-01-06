#include "bottom_view.h"
#include "utils.h"
#define CURSOR_YOFFSET  3
#define CURSOR_WIDTH    2

BottomView::BottomView(Camera &camera, sf::Font &font, int &charSize, sf::Vector2u &windowSize, int& height) : 
View{camera, font, charSize, windowSize}, m_height{height}
{
  std::cout << "init BottomView()" << "\n";
  //std::cout << "offset in cons("<< xOffset << ")" << "\n";
  init();
}

void BottomView::init()
{
  // -------------INIT CURSOR----------------

  CURSOR_HEIGHT   = m_characterSize;

  m_cursorItem.setFillColor(themes::deepBlue.BLUE);
  m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));


  // -------------INIT MAIN BORDER----------------
  m_MAIN_BORDER_WIDTH = m_winSize.x;
  m_MAIN_BORDER_HEIGHT = 35;
  m_MAIN_BORDER_YPOS = m_winSize.y-m_MAIN_BORDER_HEIGHT;

  m_mainScreen.setFillColor(themes::deepBlue.GREY);
  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(0, m_MAIN_BORDER_YPOS);

  // -------------INIT LINE TEXT----------------
  m_lineText.setFont(m_font);
  m_lineText.setFillColor(themes::deepBlue.BLUE);
  m_lineText.setCharacterSize(m_characterSize);

  m_arrowText.setFont(m_font);
  m_arrowText.setFillColor(themes::deepBlue.BLUE);
  m_arrowText.setCharacterSize(m_characterSize);
  m_arrowText.setString(">>");
  xOffset = TextUtils::getCharGlyphSize('>', m_font, m_characterSize).first*2;
  // m_cursor.setPosition(0, xOffset + 2); // 2 is the padding
  int half = (m_MAIN_BORDER_HEIGHT/2) - (CURSOR_HEIGHT/2);
  c_posY =  m_winSize.y - (m_MAIN_BORDER_HEIGHT - half); 
  std::cout << "half: " << half << "\n"; 
  std::cout << "main_winPos: " << m_MAIN_BORDER_YPOS << "\n"
            << "cursor_Pos: " << m_winSize.y - (m_MAIN_BORDER_HEIGHT - half) << "\n";
  m_cursor.setScreenPosition(0, ( c_posY )); // 2 is the padding

  std::cout << "bottom cursor [lineN: " << m_cursor.getCursorPosColumnNumber() << ", "
            << "screen_lineN: " << m_cursor.getScreenPosition().x << "]\n";

}

void BottomView::setBGColor(sf::Color color)
{
  // TODO: maybe have a way to just change the theme being used
  // have a class type called theme where you can set primary, secondary...etc
  // then you can change via a pointer
  m_mainScreen.setFillColor(color);
}

void BottomView::drawMainScreen(sf::RenderWindow &window)
{
  m_MAIN_BORDER_WIDTH = m_winSize.x;
  m_MAIN_BORDER_YPOS = m_winSize.y-m_MAIN_BORDER_HEIGHT;

  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(0, m_MAIN_BORDER_YPOS);
  window.draw(m_mainScreen);
}

void BottomView::drawCursor(sf::RenderWindow &window)
{
  int half = (m_MAIN_BORDER_HEIGHT/2) - (CURSOR_HEIGHT/2);
  c_posY =  m_winSize.y - (m_MAIN_BORDER_HEIGHT - half); 
  sf::Vector2i pos = m_cursor.getScreenPosition();

  m_cursorItem.setPosition(pos.x + (xOffset + 2), c_posY);
  window.draw(m_cursorItem);
}

void BottomView::drawLineText(sf::RenderWindow &window)
{
  int yPos = m_cursor.getScreenPosition().y;
  m_arrowText.setPosition(0, c_posY);
  window.draw(m_arrowText);
  if (!m_buffer.getInputBuffer().empty())
  {
    int rowPos{c_posY};
    for (int i = 0; i < m_buffer.getInputBuffer().size(); i++)
    {
      m_lineText.setPosition(xOffset + 2, rowPos);
      m_lineText.setString(m_buffer.getInputBuffer()[i]);
      window.draw(m_lineText);
      int activeLineNumber = m_cursor.getLineNumber(m_characterSize);
      rowPos += m_characterSize;
    }
  }
}

void BottomView::renderFixedUI(sf::RenderWindow &window)
{
  drawMainScreen(window);
  drawCursor(window);
  drawLineText(window);
}

void BottomView::renderMoveableUI(sf::RenderWindow &window)
{
}
