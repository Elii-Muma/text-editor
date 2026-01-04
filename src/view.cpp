#include "view.h"

View::View(Camera& camera, sf::Font& font) : m_camera{camera}, m_font{font}
{
  std::cout << "init view()" << "\n";
  init();
}

void View::init()
{
  int CURSOR_YOFFSET = 3;
  int CURSOR_WIDTH = 2;
  int CURSOR_HEIGHT = m_characterSize + CURSOR_YOFFSET;
  m_cursorItem.setFillColor(sf::Color(0, 255, 0));
  m_cursorItem.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  // TODO: get this from the actual main window
  winSize.x= 900;
  winSize.y= 600;

  m_SIDE_BORDER_HEIGHT = winSize.y;
  int m_SIDE_BORDER_WIDTH = 60;

  /**
   * Black & Gold Elegance

000000

14213D

FCA311

E5E5E5

FFFFFF
   */

  std::cout<<"side border height:: " << m_SIDE_BORDER_HEIGHT << "\n";
  m_sideBorder.setFillColor(sf::Color(0xFCA311FF));
  m_sideBorder.setSize(sf::Vector2f(m_SIDE_BORDER_WIDTH, m_SIDE_BORDER_HEIGHT));
  m_sideBorder.setPosition(0, 0);

  m_MAIN_BORDER_WIDTH   = winSize.x - m_SIDE_BORDER_WIDTH;
  m_MAIN_BORDER_HEIGHT  = winSize.y;
  m_mainScreen.setFillColor(sf::Color(0x14213DFF));
  m_mainScreen.setSize(sf::Vector2f(m_MAIN_BORDER_WIDTH, m_MAIN_BORDER_HEIGHT));
  m_mainScreen.setPosition(m_SIDE_BORDER_WIDTH, 0);

  // --- text --- 
  m_lineText.setFont(m_font);
  m_lineText.setFillColor(sf::Color::White);
  m_lineText.setCharacterSize(22);

  m_lineNumberText.setFont(m_font);
  m_lineNumberText.setFillColor(sf::Color(0xE5E5E5FF));
  m_lineNumberText.setCharacterSize(m_characterSize);
}

void View::drawSideBorder(sf::RenderWindow &window)
{
  window.draw(m_sideBorder);
}

void View::drawMainScreen(sf::RenderWindow &window)
{
  window.draw(m_mainScreen);
}

void View::drawCursor(sf::RenderWindow &window)
{
  int colN = m_cursor.getCursorPosColumnNumber()+60;
  int lineN = m_cursor.getCursorPosLineNumber();

  m_cursorItem.setPosition(colN, lineN);
  window.draw(m_cursorItem);
}

void View::drawLineText(sf::RenderWindow &window)
{
  if(!m_buffer.getInputBuffer().empty()){
    int rowPos{0};
    for(int i=0; i<m_buffer.getInputBuffer().size(); i++){
      m_lineText.setPosition(60, rowPos);
      m_lineText.setString(m_buffer.getInputBuffer()[i]);
      window.draw(m_lineText);
      int activeLineNumber = m_cursor.getLineNumber(m_characterSize);
      drawLineNumber(window, i, rowPos, activeLineNumber);
      rowPos += 22;
    }
  } else
    std::cout << "buffer empty\n";
}

void View::drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber)
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

void View::render(sf::RenderWindow &window)
{
  // drawSideBorder(window);
  // drawMainScreen(window);
  drawCursor(window);
  drawLineText(window);
}

Buffer& View::getBuffer()
{
  return m_buffer;
}

Cursor& View::getCursor(){
  return m_cursor;
}

Camera &View::getCamera()
{
  // TODO: insert return statement here
  return m_camera;
}

// Buffer &View::getBuffer()
// {
//   // TODO: insert return statement here
//   return m_buffer;
// }
