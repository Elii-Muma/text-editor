#include "Editor.h"
#include <iostream>
#include "Cursor.h"

Editor::Editor(sf::Font &font, int characterSize) : m_font{font}, m_characterSize{characterSize}, renderer{font}, lineN{0}
{
  std::printf("init editor()\n");
  inputBuffer.push_back("");
  inputLineLen.push_back(0);
}

void Editor::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::TextEntered)
  {
    if (!(ev.text.unicode < 32 || ev.text.unicode > 126))
    {
      sf::String strChar(static_cast<char>(ev.text.unicode));
      int index = getCharPosAt();
      int charWidth = getCharGlyphSize(static_cast<char>(ev.text.unicode)).first;

      inputBuffer[lineN].insert(index, strChar);
      DelData tempDelData = {strChar, index, lineN, sf::Vector2f(cursor.getCursorPosColumnNumber(), cursor.getCursorPosLineNumber()), NEW_INPUT};
      deleteStack.push_back(tempDelData);

      if (inputLineLen.size() <= lineN)
        inputLineLen.resize(lineN + 1, 0);

      int lineWidth = inputLineLen.at(lineN);
      inputLineLen[lineN] = lineWidth + charWidth;
      std::cout << "at line:: " << lineN << " line length :: " << lineWidth + charWidth << "\n";
      cursorMoveRight();
    }
  }

  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    int colN = cursor.getCursorPosColumnNumber();
    switch (ev.key.code)
    {
    case sf::Keyboard::Left:
      cursorMoveLeft();
      break;
    case sf::Keyboard::Right:
      cursorMoveRight();
      break;
    case sf::Keyboard::Up:
      if (lineN - 1 >= 0)
      {
        lineN--;
        cursor.moveCursorUp(22);
      }
      break;
    case sf::Keyboard::Down:
      if (lineN + 1 < inputBuffer.size())
      {
        lineN++;
        cursor.moveCursorDown(22);
      }
      break;
    case sf::Keyboard::Enter:
      if (!inputBuffer[lineN].empty())
      {
        int index = getCharPosAt();
        std::string t{inputBuffer[lineN]};
        std::cout << "before erased result: " << t << "\n";
        t.erase(0, index); // erase from beginning to cursor
        std::cout << "erased result: " << t << "\n";

        DelData tempDelData = {t, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), ENT_MOVE};
        deleteStack.push_back(tempDelData);
        inputBuffer[lineN].erase(index); // erase from cursor to end
        inputBuffer.insert(inputBuffer.begin() + lineN + 1, t);
      }
      else
      {
        inputBuffer.insert(inputBuffer.begin() + lineN + 1, "");
        DelData tempDelData = {"", index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), ENT_MOVE};
        deleteStack.push_back(tempDelData);
      }
      cursor.setPosition(cursor.getCursorPosLineNumber() + 22, 0);
      lineN++;
      break;
    case sf::Keyboard::BackSpace:

      std::cout << "column number :: " << colN << "\n";
      if (colN > 0)
      {
        std::cout << "delete";

        // why do i use -1 here
        // because getCharPosAt() returns the character AFTER the cursor OR
        // the last character
        index = getCharPosAt() - 1;
        if (index >= 0)
        {
          std::cout << "in condition 1\n";
          std::string str{inputBuffer[lineN][index]};

          std::string del_char{inputBuffer[lineN][index]};
          inputBuffer[lineN].erase(index, 1);

          DelData tempDelData = {del_char, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), NORMAL_DEL};
          deleteStack.push_back(tempDelData);
          std::cout << "deleted: " << tempDelData.del_char << "\n";

          cursorMoveLeft();
          break;
        }
      }
      else if (colN <= 0)
      {
        std::cout << "in condition 2\n";
        // delete the line and append it to the line before it
        // unless its the first line
        std::string temp{inputBuffer[lineN]};
        if (lineN != 0)
        {
          inputBuffer.erase(inputBuffer.begin() + lineN);
          DelData tempDelData = {temp, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), NORMAL_DEL};

          lineN--;
          if (lineN < 0)
            lineN = 0;
          if (!temp.empty())
          {
            int size = inputBuffer[lineN].length();
            cursor.setPosition(cursor.getCursorPosLineNumber() - 22, 0);
            inputBuffer[lineN].append(temp);
          }
          else
          {
            cursor.setPosition(cursor.getCursorPosLineNumber() - 22, 0);
          }
          deleteStack.push_back(tempDelData);
        }
      }
      break;
    case sf::Keyboard::LControl:
      isUndoPressed = true; 
      break;
    default:
      break;
    }
  }
  if (ev.type == sf::Event::KeyPressed && isUndoPressed)
  {
    if (ev.key.code == sf::Keyboard::Z)
    {
      std::cout << "control pressed\n";
      undoFunction();
      isUndoPressed = false;
    }
    if (ev.key.code == sf::Keyboard::I)
    {
      std::cout << "control + I pressed \n";
      // characterSize++;
      isUndoPressed = false;
    }
  }
  else
  {
    isUndoPressed = false;
  }
}

void Editor::render(sf::RenderWindow &win)
{
  renderer.drawCursor(win, cursor);
  renderer.drawSideBorder(win);

  if (!inputBuffer.empty())
  {
    int rowPosInc{0};
    for (int i = 0; i < inputBuffer.size(); i++)
    {
      renderer.drawLineText(win, inputBuffer[i], i, rowPosInc);
      renderer.drawLineNumber(win, i, rowPosInc, cursor.getCursorPosLineNumber()/m_characterSize);
      //renderer.drawLineNumber(win, i);
      rowPosInc += m_characterSize;
    }
  }
}

// returns character right after cursor or last character if cursor is at the end
int Editor::getCharPosAt()
{
  int colN = cursor.getCursorPosColumnNumber();

  float total = 0.f;
  if (inputBuffer.empty())
  {
    std::cout << "empty buffer\n";
    return 0;
  }

  for (int i = 0; i < inputBuffer[lineN].size(); ++i)
  {
    std::string cur_char{inputBuffer[lineN][i]};
    float charWidth = getCharGlyphSize(*cur_char.c_str()).first;
    // if cursorXPos
    if (colN <= total)
    {
      return i;
    }
    total += charWidth;
  }

  return inputBuffer[lineN].size();
}

/// @brief move the cursor to the right
void Editor::cursorMoveRight()
{

  int colN = cursor.getCursorPosColumnNumber();

  int index = getCharPosAt();
  if (index >= inputBuffer[lineN].size())
    return;
  std::string cur_char{inputBuffer[lineN][index]};

  int charWidth = getCharGlyphSize(*cur_char.c_str()).first;
  cursor.moveCursorRight(charWidth);
}

/// @brief move the cursor to the left
// you want to move back the size o
void Editor::cursorMoveLeft()
{
  int colN = cursor.getCursorPosColumnNumber();

  int index = getCharPosAt();
  if (index < 0)
    return;
  std::string cur_char{inputBuffer[lineN][index]};
  int charWidth = getCharGlyphSize(*cur_char.c_str()).first;
  if ((colN - charWidth) < 0)
    charWidth = 0;

  cursor.moveCursorLeft(charWidth);
}

std::pair<int, int> Editor::getCharGlyphSize(char character)
{
  sf::Glyph glyph = m_font.getGlyph(character, m_characterSize, false);
  // float charWidth = glyph.advance;  //this one includes spacing to next thingy
  int charWidth = static_cast<int>(glyph.advance);
  int charHeight = static_cast<int>(glyph.bounds.height);

  std::pair<int, int> charDimensions = {charWidth, charHeight};

  return charDimensions;
}

//function that handles the undo function
void Editor::undoFunction()
{
  //must've written this on coke cos ts works perfectly
  if (!deleteStack.empty())
  {
    int cPosCol{0};
    int cPosRow{0};
    // get the data at the end(being the top)
    DelData tempData = deleteStack.back();
    // remove it
    deleteStack.pop_back();

    std::cout << "about to undo\n";
    // check if that line still exists
    std::cout << "line number: " << tempData.lineNumber
              << "buffer size" << inputBuffer.size() << "\n";

    for (auto c : deleteStack)
    {
      std::cout << "chars in stack: " << c.del_char << "\n";
    }

    switch (tempData.s_type)
    {
    case NORMAL_DEL:
      std::cout << "undoing character\n";
      // insert the deleted character at its prev pos
      inputBuffer[tempData.lineNumber].insert(tempData.index, tempData.del_char);
      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      cursor.setPosition(cPosRow, cPosCol);
      break;
    case NEW_INPUT:
      inputBuffer[tempData.lineNumber].erase(tempData.index, 1);
      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      cursor.setPosition(cPosRow, cPosCol);
      break;
    case ENT_MOVE:
      std::cout << "you want to undo enter\n"
                << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber;

      inputBuffer[tempData.lineNumber].append(tempData.del_char);
      // delete current line
      inputBuffer.erase(inputBuffer.begin() + lineN);
      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      cursor.setPosition(cPosRow, cPosCol);

      break;
    case LINE_DEL:
      std::cout << "undoing line\n";
      // get the text the previous line was at before it got appended
      {
        std::string appendedLine{inputBuffer[tempData.lineNumber - 1]};
        size_t pos = appendedLine.find(tempData.del_char);

        if (tempData.lineNumber - 1 < 0)
          inputBuffer[0].erase(pos);
        else
          inputBuffer[tempData.lineNumber - 1].erase(pos);
      }

      inputBuffer.insert(inputBuffer.begin() + (tempData.lineNumber), tempData.del_char);
      {
        sf::Text tempTxt;
        tempTxt.setFont(m_font);
        tempTxt.setString(tempData.del_char);
      }

      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      cursor.setPosition(cPosRow, cPosCol);
      break;
    default:
      break;
    }
  }
  else
  {
    std::cout << "delete stack is empty!\n";
  }
}