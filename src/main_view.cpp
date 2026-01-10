#include "main_view.h"
#include "utils.h"

MainView::MainView(sf::Font &font, int &charSize, sf::Vector2u &windowSize) : View{font, charSize, windowSize}
{
  std::cout << "init MainView()" << "\n";
  init();
}

void MainView::init()
{
  // -------------INIT CURSOR----------------
  int CURSOR_YOFFSET = 3;
  int CURSOR_WIDTH = 2;
  int CURSOR_HEIGHT = m_characterSize + CURSOR_YOFFSET;

  m_cursorItem.setFillColor(sf::Color(0, 255, 0));
  m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  // -------------INIT SIDE BORDER----------------

  m_SIDE_BORDER_HEIGHT = m_winSize.y;
  m_SIDE_BORDER_WIDTH = 60;

  m_sideBorder.setFillColor(themes::deepBlue.YELLOW);
  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
  m_sideBorder.setPosition(0, 0);

  // -------------INIT MAIN BORDER----------------
  m_MAIN_BORDER_WIDTH = m_winSize.x - m_SIDE_BORDER_WIDTH;
  m_MAIN_BORDER_HEIGHT = m_winSize.y;

  m_mainScreen.setFillColor(themes::deepBlue.BLUE);
  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(m_SIDE_BORDER_WIDTH, 0);

  // -------------INIT LINE TEXT----------------
  m_lineText.setFont(m_font);
  m_lineText.setFillColor(themes::deepBlue.WHITE);
  m_lineText.setCharacterSize(m_characterSize);

  // -------------INIT LINE NUMBER TEXT----------------
  m_lineNumberText.setFont(m_font);
  m_lineNumberText.setFillColor(themes::deepBlue.GREY);
  m_lineNumberText.setCharacterSize(m_characterSize);
  m_cursor.setScreenPosition(0, 0);
}

void MainView::setBGColor(sf::Color color)
{
  // TODO: maybe have a way to just change the theme being used
  // have a class type called theme where you can set primary, secondary...etc
  // then you can change via a pointer
  m_mainScreen.setFillColor(color);
}

void MainView::drawSideBorder(sf::RenderWindow &window)
{
  m_SIDE_BORDER_HEIGHT = m_winSize.y;

  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
  window.draw(m_sideBorder);
}

void MainView::drawMainScreen(sf::RenderWindow &window)
{
  m_MAIN_BORDER_WIDTH = m_winSize.x - m_SIDE_BORDER_WIDTH;
  m_MAIN_BORDER_HEIGHT = m_winSize.y;

  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  window.draw(m_mainScreen);
}

void MainView::drawCursor(sf::RenderWindow &window)
{
  sf::Vector2i pos = m_cursor.getScreenPosition();

  m_cursorItem.setPosition(pos.x + 60, pos.y);
  window.draw(m_cursorItem);
}

void MainView::drawLineText(sf::RenderWindow &window)
{
  if (!m_buffer.getInputBuffer().empty())
  {
    int rowPos{0};
    for (int i = 0; i < m_buffer.getInputBuffer().size(); i++)
    {
      m_lineText.setPosition(60, rowPos);
      m_lineText.setString(m_buffer.getInputBuffer()[i]);
      window.draw(m_lineText);
      int activeLineNumber = m_cursor.getLineNumber(m_characterSize);
      drawLineNumber(window, i, rowPos, activeLineNumber);
      rowPos += m_characterSize;
    }
  }
}

void MainView::drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber)
{
  if (activeLineNumber == lineNumber)
  {
    m_lineNumberText.setFillColor(sf::Color(0, 255, 0));
    m_lineNumberText.setPosition(10, numPos);
  }
  else
  {
    m_lineNumberText.setFillColor(sf::Color::White);
    m_lineNumberText.setPosition(5, numPos);
  }

  m_lineNumberText.setString(std::to_string(lineNumber + 1));
  window.draw(m_lineNumberText);
}

void MainView::update(){

  if(prev_fontSize != m_characterSize){
    updateCursorScreenPos();
    m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
    m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  }
}

void MainView::renderFixedUI(sf::RenderWindow &window)
{
  drawSideBorder(window);
  drawMainScreen(window);
}

void MainView::renderMoveableUI(sf::RenderWindow &window)
{
  // std::cout << "moveable UI render()\n";
  drawCursor(window);
  drawLineText(window);
}
