#include "terminal_view.h"
#include "utils.h"
#define CURSOR_YOFFSET 2

//TODO: maybe the cursor width could dynamically switch to be the size of the current char its on

TerminalView::TerminalView(sf::Font &font, int &charSize, sf::Vector2u &windowSize, unsigned int &height, bool isScreen) : 
m_isScreen{isScreen}, 
View{font, charSize, windowSize}, m_MAIN_BORDER_HEIGHT{height}
{
  std::cout << "init TerminalView()" << "\n";
  // std::cout << "offset in cons("<< xOffset << ")" << "\n";
  prev_fontSize = m_characterSize;
  init();
}

void TerminalView::init()
{
  // -------------INIT CURSOR----------------

  CURSOR_HEIGHT = 4;
  CURSOR_WIDTH = TextUtils::getCharGlyphSize('A', m_font, m_characterSize).first;

  m_cursorItem.setFillColor(themes::deepBlue.BLUE);
  m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  // -------------INIT MAIN BORDER----------------
  m_MAIN_BORDER_WIDTH = m_winSize.x;
  // m_MAIN_BORDER_HEIGHT = m_height;
  m_MAIN_BORDER_YPOS = m_winSize.y - m_MAIN_BORDER_HEIGHT;

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
  xOffset = TextUtils::getCharGlyphSize('>', m_font, m_characterSize).first * 2;
  // m_cursor.setPosition(0, xOffset + 2); // 2 is the padding
  if (!m_isScreen)
  {
    int half = (m_MAIN_BORDER_HEIGHT / 2) - ((m_characterSize+CURSOR_YOFFSET)/ 2);
    c_posY = m_winSize.y - (m_MAIN_BORDER_HEIGHT - half);
    std::cout << "half: " << half << "\n";
    std::cout << "main_winPos: " << m_MAIN_BORDER_YPOS << "\n"
              << "cursor_Pos: " << m_winSize.y - (m_MAIN_BORDER_HEIGHT - half) << "\n";
  }
  else
    c_posY = 0;
  m_cursor.setScreenPosition(0, (c_posY)); // 2 is the padding

  std::cout << "bottom cursor [lineN: " << m_cursor.getCursorPosColumnNumber() << ", "
            << "screen_lineN: " << m_cursor.getScreenPosition().y << "]\n";
}

bool TerminalView::getIsScreen(){
  return m_isScreen;
}
void TerminalView::update(){

  sf::Vector2i pos = m_cursor.getScreenPosition();
  if (!m_isScreen){
    int half = (m_MAIN_BORDER_HEIGHT / 2) - ((m_characterSize+CURSOR_YOFFSET)/ 2);
    c_posY = m_winSize.y - (m_MAIN_BORDER_HEIGHT - half);
  }else
    c_posY = pos.y;

  m_cursorItem.setPosition(pos.x + (xOffset + 2), c_posY + m_characterSize + CURSOR_YOFFSET);

  //ONLY UPDATE WHEN CHARACTER SIZE IS INCREASED
  if(prev_fontSize != m_characterSize){
    std::cout << "update()\n";

    updateCursorScreenPos();

    CURSOR_WIDTH = TextUtils::getCharGlyphSize('A', m_font, m_characterSize).first;
    std::cout << "width from tv(): " << CURSOR_WIDTH << "\n"
    << "font size from tv(): " << m_characterSize << "\n";
    m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

    //m_cursor.moveCursorRight(m_characterSize-prev_fontSize); //this iz zupposed to fix a bug? doesnt work tho
    xOffset = TextUtils::getCharGlyphSize('>', m_font, m_characterSize).first * 2;

    m_arrowText.setCharacterSize(m_characterSize);
    m_lineText.setCharacterSize(m_characterSize);

    //finally
    prev_fontSize = m_characterSize;
  }
}

void TerminalView::setBGColor(sf::Color color)
{
  // TODO: maybe have a way to just change the theme being used
  // have a class type called theme where you can set primary, secondary...etc
  // then you can change via a pointer
  m_mainScreen.setFillColor(color);
}

void TerminalView::drawMainScreen(sf::RenderWindow &window)
{
  m_MAIN_BORDER_WIDTH = m_winSize.x;
  m_MAIN_BORDER_YPOS = m_winSize.y - m_MAIN_BORDER_HEIGHT;

  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(0, m_MAIN_BORDER_YPOS);
  window.draw(m_mainScreen);
}

void TerminalView::drawCursor(sf::RenderWindow &window)
{
  window.draw(m_cursorItem);
}

void TerminalView::drawLineText(sf::RenderWindow &window)
{
  if (!m_buffer.getInputBuffer().empty())
  {
    int rowPos{0}; //c_posy gets computed from the draw cursor function
    for (int i = 0; i < m_buffer.getInputBuffer().size(); i++)
    {
      int yPos = (m_isScreen) ? rowPos : c_posY;

      m_arrowText.setPosition(0, yPos);

      m_lineText.setPosition(xOffset + 2, yPos);
      m_lineText.setString(m_buffer.getInputBuffer()[i]);

      window.draw(m_arrowText);
      window.draw(m_lineText);
      int activeLineNumber = m_cursor.getLineNumber(m_characterSize);
      rowPos += m_characterSize;
    }
  }
}

void TerminalView::renderFixedUI(sf::RenderWindow &window)
{
  drawMainScreen(window);
  if(!m_isScreen){
    drawCursor(window);
    drawLineText(window);
  }
}

void TerminalView::renderMoveableUI(sf::RenderWindow &window)
{
  if(m_isScreen){
    drawCursor(window);
    drawLineText(window);
  }
}
