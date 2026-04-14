#include "main_view.h"
#include "utils.h"

MainView::MainView(sf::Font &font, int &charSize, sf::Vector2u &windowSize) : View{font, charSize, windowSize}
{
  std::cout << "init MainView()" << "\n";
  prev_fontSize = m_characterSize;
  init();
}

void MainView::init()
{
  // -------------INIT CURSOR----------------
  CURSOR_YOFFSET  = 0;
  CURSOR_WIDTH    = 3;
  CURSOR_HEIGHT   = m_characterSize - CURSOR_YOFFSET;
  //the offset, to center the cursor, will always be the same because the block size is always the same length
  //charactersize in this case is the size of each line/lineheight ?? idk
  CURSOR_PY_OFF   = (m_characterSize - CURSOR_HEIGHT); //UNUSED

  m_cursorItem.setFillColor(themes::oceanBreeze.ACCENT);
  m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  // -------------INIT SIDE BORDER----------------

  m_SIDE_BORDER_HEIGHT  = m_winSize.y;
  m_SIDE_BORDER_WIDTH   = 60;

  m_sideBorder.setFillColor(themes::oceanBreeze.SECONDARY);
  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
  m_sideBorder.setPosition(0, 0);

  // -------------INIT MAIN BORDER----------------
  m_MAIN_BORDER_WIDTH   = m_winSize.x - m_SIDE_BORDER_WIDTH;
  m_MAIN_BORDER_HEIGHT  = m_winSize.y;

  m_mainScreen.setFillColor(themes::oceanBreeze.BACKGROUND);
  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(m_SIDE_BORDER_WIDTH, 0);

  // -------------INIT LINE TEXT----------------
  m_lineText.setFont(m_font);
  m_lineText.setFillColor(themes::oceanBreeze.TEXT_COLOR);
  m_lineText.setCharacterSize(m_characterSize);

  // -------------INIT LINE NUMBER TEXT----------------
  m_lineNumberText.setFont(m_font);
  m_lineNumberText.setFillColor(themes::oceanBreeze.ACCENT);
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
  window.draw(m_mainScreen);
}

void MainView::drawCursor(sf::RenderWindow &window)
{
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
    m_lineNumberText.setFillColor(themes::oceanBreeze.ACCENT);
    m_lineNumberText.setPosition(10, numPos);
  }
  else
  {
    m_lineNumberText.setFillColor(themes::oceanBreeze.TEXT_COLOR);
    m_lineNumberText.setPosition(5, numPos);
  }

  m_lineNumberText.setString(std::to_string(lineNumber + 1));
  window.draw(m_lineNumberText);
}

void MainView::update()
{
  //CALC AND SET SCREEN SIZE
  m_MAIN_BORDER_WIDTH   = m_winSize.x - m_SIDE_BORDER_WIDTH;
  m_MAIN_BORDER_HEIGHT  = m_winSize.y;

  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));

  // SET CURSOR POS
  sf::Vector2i pos = m_cursor.getScreenPosition();
  m_cursorItem.setPosition(pos.x + 60, pos.y);

  //ONLY DO THIS IF THE FONT INCREASES OR DECREASES
  if (prev_fontSize != m_characterSize)
  {
    updateCursorScreenPos();

    CURSOR_HEIGHT = m_characterSize - CURSOR_YOFFSET;
    m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

    m_lineText.setCharacterSize(m_characterSize);
    m_lineNumberText.setCharacterSize(m_characterSize);
    // m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
    // m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
    prev_fontSize = m_characterSize;
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
