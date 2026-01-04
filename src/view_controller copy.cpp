#include "view_controller.h"
#include "utils.h"

ViewController::ViewController(View &view, sf::Font &font, int characterSize) : 
m_view{view}, m_camera{view.getCamera()}, m_buffer{m_buffer}, m_cursor{view.getCursor()}, 
m_characterSize{characterSize}, m_font{font}
{
  std::cout << "init vc()\n";
}

void ViewController::moveCameraUp()
{
  int position = (m_cursor.getCursorPosLineNumber() - (m_characterSize));
  std::cout << "m_cursor to be at: " << position << "\n";
  if (position < m_camera.viewTop())
  {
    m_camera.cameraScrollUp();
  }
}

// depends on: m_cursor position, charaactersize, view_border dimension, m_camera for the view
// what it does, it moves the views m_camera
void ViewController::moveCameraDown(int bottomOffset)
{
  int position = (m_cursor.getCursorPosLineNumber() + (m_characterSize * 2));
  std::cout << "m_cursor to be at: " << position << "\n";
  if (position > m_camera.viewBottom() - bottomOffset)
  {
    m_camera.cameraScrollDown();
  }
}

/// @brief move the m_cursor to the right
void ViewController::cursorMoveRight()
{
  // m_cursor.setPosition(50, 50);
  int colN = m_cursor.getCursorPosColumnNumber();
  int lineN = m_cursor.getLineNumber(m_characterSize); // maybe?? ive forgotten what this represents
  std::cout << "[ colN: " << colN  << " || "
            << "lineN: " << lineN << " ]\n";
  int index = m_buffer.getCharPosAt(colN, lineN, m_font, m_characterSize);
  // if the m_cursor is at the end of the line
  std::cout << "gotten index:: " << index << "\n";
  if (index >= m_buffer.getInputBuffer()[lineN].size())
    return;

  std::cout<<"index:: " << index << "\n";
  std::string cur_char{m_buffer.getInputBuffer()[lineN][index]};
  int charWidth = TextUtils::getCharGlyphSize(*cur_char.c_str(), m_font, m_characterSize).first;
  m_cursor.moveCursorRight(charWidth);
}

/// @brief move the m_cursor to the left
void ViewController::cursorMoveLeft()
{
  int colN = m_cursor.getCursorPosColumnNumber();
  int lineNumber = m_cursor.getLineNumber(m_characterSize); // maybe?? ive forgotten what this represents
  int index = m_buffer.getCharPosAt(colN, lineNumber, m_font, m_characterSize);
  if (index < 0)
    return;
  std::string cur_char{m_buffer.getInputBuffer()[lineNumber][index]};

  int charWidth = TextUtils::getCharGlyphSize(*cur_char.c_str(), m_font, m_characterSize).first;
  if ((colN - charWidth) < 0)
    charWidth = 0;

  m_cursor.moveCursorLeft(charWidth);
}
//cursorMoveDown()
void ViewController::cursorMoveDown()
{
  int lineN = m_cursor.getLineNumber(m_characterSize);
  int size = m_buffer.getInputBuffer().size();
  std::cout << "[lineN: " << lineN << " || "
            << "size: " << size << " ]\n";
  if (lineN + 1 < size)
  {
    moveCameraDown(30);
    m_cursor.setPosition(
        m_cursor.getCursorPosLineNumber() + m_characterSize,
        cursorMoveToCol(1));
  }
}

void ViewController::cursorMoveUp()
{
  int lineN = m_cursor.getLineNumber(m_characterSize);
  if (lineN - 1 >= 0)
  {
    moveCameraUp();
    m_cursor.setPosition(
        m_cursor.getCursorPosLineNumber() - m_characterSize,
        cursorMoveToCol(0));
  }
}

// returns column position the cursor should be moved to
// check the length of the line
// dir: 0 = up, 1 = down
int ViewController::cursorMoveToCol(int dir)
{
  int colN = m_cursor.getCursorPosColumnNumber();
  int lineNumber = m_cursor.getLineNumber(m_characterSize); // maybe?? ive forgotten what this represents
  int index = m_buffer.getCharPosAt(colN, lineNumber, m_font, m_characterSize);
  std::cout << "column index in curr line: " << index << "\n";

  if(dir == 0){
    if(lineNumber - 1 >= 0)
      lineNumber--;
  }else{
    lineNumber++;
  }
  int size = m_buffer.getInputBuffer()[lineNumber].size();
  if(index > size)
    index = size;
  int sumOfCharWidth{0};
  for (int i = 0; i < index; i++)
  {
    std::string cur_char{m_buffer.getInputBuffer()[lineNumber][i]};
    sumOfCharWidth += TextUtils::getCharGlyphSize(*cur_char.c_str(), m_font, m_characterSize).first;
  }
  std::cout << "sum of widths :: " << sumOfCharWidth << "\n";
  return sumOfCharWidth;
}

void ViewController::cursorMoveToHome(){
  m_cursor.setPosition(m_cursor.getCursorPosLineNumber(), 0);
}

// move the m_cursor to the end of the line
void ViewController::cursorMoveToEnd()
// i wanted to have a seperate data structure to hold the sum of character wifths
// of each line but for my small application i think this for loop implementation should be fine
// its basically a move to right but we keep summing until we reach the end
{
  int colN = m_cursor.getCursorPosColumnNumber();
  int lineNumber = m_cursor.getLineNumber(m_characterSize); // maybe?? ive forgotten what this represents
  int index = m_buffer.getCharPosAt(colN, lineNumber, m_font, m_characterSize);

  // if the m_cursor is at the end of the line
  if (index >= m_buffer.getInputBuffer()[lineNumber].size())
    return;

  int sumOfCharWidth{0};
  for (int i = 0; i < (int)m_buffer.getInputBuffer()[lineNumber].length(); i++)
  {
    std::string cur_char{m_buffer.getInputBuffer()[lineNumber][i]};
    sumOfCharWidth += TextUtils::getCharGlyphSize(*cur_char.c_str(), m_font, m_characterSize).first;
  }
  std::cout << "sum of widths :: " << sumOfCharWidth << "\n";
  m_cursor.setPosition(m_cursor.getCursorPosLineNumber(), sumOfCharWidth);
}

// -- i dont know where to put the functions below so ill just keep them here -- 
// THEY SEEM LIKE TEXT FUNCTIONS, SO MAYBE ILL THROW THEM IN THE UTILS? BUT HERE THEY DONT NEED ARGS

// dir = 0: is not  backspace, dir = 1, is backspace
void ViewController::RemoveCharacter(int dir){
  int colN = m_cursor.getCursorPosColumnNumber();
  int lineN = m_cursor.getLineNumber(m_characterSize);
  int offset = dir; //offset is 1 if its backspace...
  int index = m_buffer.getCharPosAt(colN, lineN, m_font, m_characterSize) - offset;

  bool isBackSpace = dir;
  EraseCharacter(isBackSpace, colN, index);
}

// WHAT DOES THIS METHOD DEPEND ON
void ViewController::EraseCharacter(bool isBackSpace, int colN, int index)
{
  std::cout << "column number :: " << colN << "\n";
  std::vector<std::string> &inputBuffer{m_buffer.getInputBuffer()};
  std::vector<m_stdl::DelData> &deleteStack = m_buffer.getDeleteStack();
  int lineN = m_cursor.getLineNumber(m_characterSize);
  if (colN > 0 || !isBackSpace)
  {
    std::cout << "delete";

    if (index >= 0)
    {
      std::cout << "in condition 1\n";
      std::string str{inputBuffer[lineN][index]};

      std::string del_char{inputBuffer[lineN][index]};
      inputBuffer[lineN].erase(index, 1);

      m_stdl::DelData tempDelData = {del_char, index, lineN, sf::Vector2f(colN, m_cursor.getCursorPosLineNumber()), m_stdl::NORMAL_DEL};
      deleteStack.push_back(tempDelData);
      std::cout << "deleted: " << tempDelData.del_char << "\n";

      if (isBackSpace)
      {
        cursorMoveLeft();
      }
      else
        std::cout << "ERASE_CHARACTER::but i dont know where to move :|\n";
    }
  }
  else if (colN <= 0)
  {
    std::cout << "in condition 2\n";
    // delete the line and append it to the line before it
    // unless its the first line
    std::string temp{inputBuffer[lineN]};
    if (lineN != 0 && isBackSpace)
    // if youre not on the first line and youve pressed backspace
    {

      // if colN is 0, doesnt that mean you'll also be at zero?
      if (index < 0)
        index = 0;

      //erase/delete the line(since its youre at the start of it)
      inputBuffer.erase(inputBuffer.begin() + lineN);
      std::cout << "deleted index:: " << index << "\n";
      m_stdl::DelData tempDelData = {temp, index, lineN, sf::Vector2f(colN, m_cursor.getCursorPosLineNumber()), m_stdl::LINE_DEL};

      moveCameraUp(); // just call it, just in case you need to pan up
      lineN--;
      if (lineN < 0)
        lineN = 0;

      int newLineColPos = m_buffer.getLineLength(lineN, m_font, m_characterSize);
      int size = inputBuffer[lineN].length();
      // get the lenght of the line at the top with reference to character widths, 
      // idk what coordinate system that can be called
      //move the cursor to that position(end of the top line)
      m_cursor.setPosition(m_cursor.getCursorPosLineNumber() - m_characterSize, newLineColPos);

      if (!temp.empty()){
        // append the string we got from the cut out line to the end of it
        inputBuffer[lineN].append(temp);
      }

      deleteStack.push_back(tempDelData);
    }
  }
}
