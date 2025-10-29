#include "Editor.h"
#include <iostream>
#include "Cursor.h"

Editor::Editor(sf::Font &font, int characterSize, sf::Vector2u *WIN_SIZE) : m_font{font}, m_characterSize{characterSize}, renderer{font, WIN_SIZE}, lineN{0}, m_WIN_SIZE{WIN_SIZE}
{
  std::printf("init editor()\n");
  inputBuffer.push_back("");
}

void Editor::render(sf::RenderWindow &win)
{
  renderer.drawCursor(win, cursor);
  renderer.drawSideBorder(win);
  renderer.drawBottomBorder(win);

  if (!inputBuffer.empty())
  {
    int rowPosInc{0};
    for (int i = 0; i < inputBuffer.size(); i++)
    {
      renderer.drawLineText(win, inputBuffer[i], i, rowPosInc);
      renderer.drawLineNumber(win, i, rowPosInc, cursor.getCursorPosLineNumber() / m_characterSize);
      // renderer.drawLineNumber(win, i);
      rowPosInc += m_characterSize;
    }
  }
}

void Editor::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::TextEntered)
  {
    if (!(ev.text.unicode < 32 || ev.text.unicode > 126))
    {
      sf::String strChar(static_cast<char>(ev.text.unicode));
      int index = getCharPosAt();

      inputBuffer[lineN].insert(index, strChar);
      DelData tempDelData = {strChar, index, lineN, sf::Vector2f(cursor.getCursorPosColumnNumber(), cursor.getCursorPosLineNumber()), NEW_INPUT};
      deleteStack.push_back(tempDelData);

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
        cursor.moveCursorUp(m_characterSize);
      }
      break;
    case sf::Keyboard::Down:
      if (lineN + 1 < inputBuffer.size())
      {
        lineN++;
        cursor.moveCursorDown(m_characterSize);
      }
      break;
    case sf::Keyboard::Home:
      cursor.setPosition(cursor.getCursorPosLineNumber(), 0);
      break;
    case sf::Keyboard::End:
      cursorMoveToEnd();
      break;
    case sf::Keyboard::Delete:
      index = getCharPosAt();
      {
        bool isBackSpace = false;
        EraseCharacter(isBackSpace, colN, index);
      }
      break;
    case sf::Keyboard::BackSpace:
      index = getCharPosAt() - 1;
      {
        bool isBackSpace = true;
        EraseCharacter(isBackSpace, colN, index);
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

        inputBuffer[lineN].erase(index); // erase from cursor to end
        // insert to the the line we're currently on + 1 meaning the bottom line
        inputBuffer.insert(inputBuffer.begin() + lineN + 1, t);

        DelData tempDelData = {t, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), ENT_MOVE};
        deleteStack.push_back(tempDelData);
      }
      else if (inputBuffer[lineN].empty())
      {
        inputBuffer.insert(inputBuffer.begin() + lineN + 1, "");
        DelData tempDelData = {"", index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), ENT_MOVE};
        deleteStack.push_back(tempDelData);
      }
      // cursor.setPosition(cursor.getCursorPosLineNumber() + 22, 0);
      cursor.moveCursorDown(m_characterSize);
      lineN++;
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

    if (ev.key.code == sf::Keyboard::S)
    {
      std::cout << "savefile pressed\n";
      saveFile();
      isUndoPressed = false;
    }
  }
  else
  {
    isUndoPressed = false;
  }
}

void Editor::EraseCharacter(bool isBackSpace, int colN, int index)
{
  std::cout << "column number :: " << colN << "\n";
  if (colN > 0 || !isBackSpace)
  {
    std::cout << "delete";

    // why do i use -1 here
    // because getCharPosAt() returns the character AFTER the cursor OR
    // the last character
    if (index >= 0)
    {
      std::cout << "in condition 1\n";
      std::string str{inputBuffer[lineN][index]};

      std::string del_char{inputBuffer[lineN][index]};
      inputBuffer[lineN].erase(index, 1);

      DelData tempDelData = {del_char, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), NORMAL_DEL};
      deleteStack.push_back(tempDelData);
      std::cout << "deleted: " << tempDelData.del_char << "\n";

      if (isBackSpace)
      {
        cursorMoveLeft();
      }
      else
        std::cout << "ERASE_CHARACTER::is not backspace so we dont move\n";
    }
  }
  else if (colN <= 0)
  {
    std::cout << "in condition 2\n";
    // delete the line and append it to the line before it
    // unless its the first line
    std::string temp{inputBuffer[lineN]};
    if (lineN != 0 && isBackSpace)
    {
      if (index < 0)
        index = 0;
      inputBuffer.erase(inputBuffer.begin() + lineN);
      std::cout << "deleted index:: " << index << "\n";
      DelData tempDelData = {temp, index, lineN, sf::Vector2f(colN, cursor.getCursorPosLineNumber()), LINE_DEL};

      lineN--;
      if (lineN < 0)
        lineN = 0;
      if (!temp.empty())
      {
        int size = inputBuffer[lineN].length();
        int newLineColPos = getLineLength(lineN);
        cursor.setPosition(cursor.getCursorPosLineNumber() - m_characterSize, newLineColPos);
        inputBuffer[lineN].append(temp);
      }
      else
      {
        int newLineColPos = getLineLength(lineN);
        cursor.setPosition(cursor.getCursorPosLineNumber() - m_characterSize, newLineColPos);
      }

      deleteStack.push_back(tempDelData);
    }
  }
}

void Editor::saveFile()
{
  // this is temporarily here, ill find a way to make the path better
  std::string filename = m_filePaths.txt_filepath + "save_file.txt";
  std::cout << "filename => " << filename << "\n";
  document.saveDocument(filename, inputBuffer);
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
  // if the cursor is at the end of the line
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

// move the cursor to the end of the line
void Editor::cursorMoveToEnd()
// i wanted to have a seperate data structure to hold the sum of character wifths
// of each line but for my small application i think this for loop implementation should be fine
// its basically a move to right but we keep summing until we reach the end
{

  int colN = cursor.getCursorPosColumnNumber();

  int index = getCharPosAt();

  // if the cursor is at the end of the line
  if (index >= inputBuffer[lineN].size())
    return;

  int sumOfCharWidth{0};
  for (int i = index; i < (int)inputBuffer[lineN].length(); i++)
  {
    std::string cur_char{inputBuffer[lineN][i]};
    sumOfCharWidth += getCharGlyphSize(*cur_char.c_str()).first;
  }
  std::cout << "sum of widths :: " << sumOfCharWidth << "\n";
  cursor.setPosition(cursor.getCursorPosLineNumber(), sumOfCharWidth);
}

int Editor::getLineLength(int lineNumber)
{

  int sumOfCharWidth{0};
  for (int i = 0; i < (int)inputBuffer[lineNumber].length(); i++)
  {
    std::string cur_char{inputBuffer[lineNumber][i]};
    sumOfCharWidth += getCharGlyphSize(*cur_char.c_str()).first;
  }
  std::cout << "sum of widths :: " << sumOfCharWidth << "\n";
  return sumOfCharWidth;
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

// function that handles the undo function
void Editor::undoFunction()
{
  // must've written this on coke cos ts works perfectly
  if (!deleteStack.empty())
  {
    int cPosCol{0};
    int cPosRow{0};
    // get the data at the end(being the top)
    DelData tempData = deleteStack.back();
    
    // ######################
    //      DEBUG PRINT
    // ######################
    // int ind = 0;
    // int len = (int)deleteStack.size();
    // std::cout << "########## ITEMS IN DELETE STACK ##########\n";
    // for (int i = len - 1; i >= 0; i--)
    // {
    //   if (i == (len - 1))
    //     std::cout << "to del->item[" << ind << "] :: " << deleteStack[i].del_char << "\n";
    //   else
    //     std::cout << "item[" << ind << "] :: " << deleteStack[i].del_char << "\n";
    //   ind++;
    // }
    // std::cout << "########## DONE ##########\n ";

    try
    {
      switch (tempData.s_type)
      {
      case NORMAL_DEL:
        std::cout << "NORMAL_DEL_UNDO::undoing character\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber;
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
        std::cout << "ENT_MOVE_UNDO::undoing enter\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber;

        // append the string that was split and moved to the new line with top line that was split
        inputBuffer[tempData.lineNumber].append(tempData.del_char);
        // after appending, delete/erase the new line string that was created
        inputBuffer.erase(inputBuffer.begin() + tempData.lineNumber + 1);

        // update the cursor data with what it was previously
        lineN = tempData.lineNumber;
        cPosCol = tempData.currCursorPos.x;
        cPosRow = tempData.currCursorPos.y;
        cursor.setPosition(cPosRow, cPosCol);

        break;
      case LINE_DEL:
        std::cout << "LINE_DEL_UNDO::undoing line\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber;
        // get the text the previous line was at before it got appended
        if (!tempData.del_char.empty())
        {
          {
            std::string appendedLine{inputBuffer[tempData.lineNumber - 1]};
            size_t pos = appendedLine.find(tempData.del_char);

            if (tempData.lineNumber - 1 < 0)
              inputBuffer[0].erase(pos);
            else
              inputBuffer[tempData.lineNumber - 1].erase(pos);
          }
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
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }

    // remove it on success
    deleteStack.pop_back();
  }
  else
  {
    std::cout << "delete stack is empty!\n";
  }
}