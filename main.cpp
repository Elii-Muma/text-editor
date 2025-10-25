/**
 * @file main.cpp
 * @author Elijah Muma
 * @brief a simple text editor
 * @version 0.1
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <math.h>
#include "cursor.h"

/**
 * @todo: ARRANGE THE CODE BETTER
 */

int newPos{-1}; // unimportant: you shoudl delete this

int LINE_HEIGHT = 30;
const int CURSOR_WIDTH = 2;
int CURSOR_HEIGHT = LINE_HEIGHT;

// this should be the size of the character at that pos
float cColInc = 0;
const int cRowInc = LINE_HEIGHT;
const int MAX_LINE = 50;
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int WID_OFFSET = 0;
float cPosCol{0};
float cPosRow{0};
int lineN{0};
int maxDrawable{0};

// ------DATA STRUCTURES------
enum dir
{
  LEFT,
  RIGHT
};
// will hold the deleted chracters
enum StackType
{
  NONE,
  LINE_DEL,
  NORMAL_DEL,
  NEW_INPUT,
  ENT_MOVE
};

/**
 * @param string deleted_character
 * @param int characters index
 * @param int charaters lineNumber
 * @param Vector2f current cursor postion
 * @param StackType type of deletion
 */
struct DelData
{
  std::string del_char;
  int index;
  int lineNumber;
  sf::Vector2f currCursorPos;
  StackType s_type{NONE};
};

std::vector<std::string> inputBuffer;
std::vector<sf::Text> inputRenderBuffer; //
std::vector<DelData> deleteStack;        // holds deleted chars

sf::RectangleShape cursor;
sf::Text tempTxt;
DelData tempData;
bool isUndoPressed{false};
std::ostringstream editorInfo;
sf::Text e_Info;
sf::Font font;
int characterSize{18};
bool isBold{false};

// ------FUNCTIONS------
int getCharPosAt(float cursorX);
void undo_func();
void cursorMoveLeft();
void cursorMoveRight();
std::pair<int, int> getCharGlyphSize(char character);

// ------ENTRY------
/**
 * TODO: fix bug...so many bugs
 */
int main()
{

  sf::RenderWindow win(sf::VideoMode({WIN_WIDTH, WIN_HEIGHT}), "Text Editor");
  sf::Event ev;
  cursor.setFillColor(sf::Color(125, 15, 15));
  cursor.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

  font.loadFromFile("resources/fonts/FiraCode-Regular.ttf");

  tempTxt.setFont(font);
  tempTxt.setString("B");
  char tempChar = 'B';
  LINE_HEIGHT = getCharGlyphSize(tempChar).second;
  CURSOR_HEIGHT = LINE_HEIGHT;

  e_Info.setFont(font);
  e_Info.setString("ln: 0");
  e_Info.setPosition(sf::Vector2f(0, 770));
  e_Info.setFillColor(sf::Color(125, 25, 25));

  /**
   * TODO: you can init struct variables like this
   * why am i doing this again?
   */
  // tempData = {};
  // for(int i=0; i<2; i++){
  //   sf::Text text;
  //   text.setFont(font);
  //   std::string str{""};

  //   inputRenderBuffer.push_back(text);
  //   inputBuffer.push_back(str);
  // }

  // init with at least one item
  sf::Text text;
  text.setFont(font);
  std::string str{""};

  inputRenderBuffer.push_back(text);
  inputBuffer.push_back(str);

  while (win.isOpen())
  {
    while (win.pollEvent(ev))
    {
      if (ev.type == sf::Event::Closed)
        win.close();

      if (ev.type == sf::Event::TextEntered)
      {
        if (ev.text.unicode < 32 || ev.text.unicode > 126)
          continue; // it'll ignore non printable text

        // access the current string at that line and append to it
        sf::Uint32 in_char = ev.text.unicode;
        std::string str{static_cast<char>(in_char)};
        float charWidth = getCharGlyphSize(in_char).first;
        int index = getCharPosAt(cPosCol);

        inputBuffer[lineN].insert(index, str);
        // why am i pushing to the delete stack if im not deleting?
        tempData = {str, index, lineN, cursor.getPosition(), NEW_INPUT};
        deleteStack.push_back(tempData);
        tempTxt.setString(str);
        // this is how im incrementing the cursors position everytime
        // i type which is flawed
        std::cout << "char width :: " << charWidth << "\n";
        cPosCol += charWidth;
        std::cout << "with + widOffset, new cPosCol:: " << cPosCol << "\n";

        if (maxDrawable < lineN)
          maxDrawable = lineN;
      }

      if (ev.type == sf::Event::KeyPressed)
      {
        float cursorX = cursor.getPosition().x;
        int len = (inputBuffer[lineN].size());
        int index{0};

        switch (ev.key.code)
        {
        case sf::Keyboard::Enter:
          /**TODO: insert new line after the current line number */
          if (!inputBuffer[lineN].empty())
          {
            int index = getCharPosAt(cursorX);
            std::string t{inputBuffer[lineN]};
            std::cout << "before erased result: " << t << "\n";
            t.erase(0, index);
            std::cout << "erased result: " << t << "\n";
            tempData = {t, index, lineN, cursor.getPosition(), ENT_MOVE};
            deleteStack.push_back(tempData);
            inputBuffer[lineN].erase(index);
            inputBuffer.insert(inputBuffer.begin() + lineN + 1, t);
          }
          else
          {
            inputBuffer.insert(inputBuffer.begin() + lineN + 1, "");
            tempData = {"", index, lineN, cursor.getPosition(), ENT_MOVE};
            deleteStack.push_back(tempData);
          }

          inputRenderBuffer.insert(inputRenderBuffer.begin() + lineN + 1, tempTxt);
          cPosRow += cRowInc; // drop the cursor to next pos
          cPosCol = 0;        // take it to the beginning
          lineN++;            // inc line number
          maxDrawable++;      // inc number of things we can draw
          break;
        case sf::Keyboard::Left:
          if (cursor.getPosition().x > 0)
            cursorMoveLeft();
          break;
        case sf::Keyboard::Right:
          if ((cursorX < WIN_WIDTH) && (getCharPosAt(cursorX) < len))
          {
            std::cout << "move right\n";
            cursorMoveRight();
          }
          break;
        case sf::Keyboard::Up:
          if (cPosRow > 0)
          {
            cPosRow -= cRowInc;
            lineN--;
            if (lineN < 0)
              lineN = 0;
            cPosCol = 0;
          }
          break;
        case sf::Keyboard::Down:
          if (cPosRow + cRowInc < WIN_HEIGHT && lineN + 1 < inputBuffer.size())
          {
            cPosRow += cRowInc;
            lineN++;
            cPosCol = 0;
          }
          break;
        case sf::Keyboard::BackSpace:

          if (cursor.getPosition() != sf::Vector2f(0, 0))
          {
            std::cout << "delete";
            index = getCharPosAt(cursorX) - 1;
            if (index >= 0)

            {
              std::cout << "in condition 1\n";
              std::string str{inputBuffer[lineN][index]};
              tempTxt.setString(str);

              std::string del_char{inputBuffer[lineN][index]};
              inputBuffer[lineN].erase(index, 1);

              tempData = {del_char, index, lineN, cursor.getPosition(), NORMAL_DEL};
              deleteStack.push_back(tempData);
              std::cout << "deleted: " << tempData.del_char << "\n";

              cPosCol -= getCharGlyphSize(*str.c_str()).first;
              break;
            }
            else if (cursorX <= 0)
            {
              std::cout << "in condition 2\n";
              // delete the line and append it to the line before it
              // unless its the first line
              std::string temp{inputBuffer[lineN]};
              if (lineN != 0)
              {
                inputBuffer.erase(inputBuffer.begin() + lineN);
                inputRenderBuffer.erase(inputRenderBuffer.begin() + lineN);
                tempData = {temp, index, lineN, cursor.getPosition(), LINE_DEL};
                lineN--;
                if (lineN < 0)
                  lineN = 0;
                if (!temp.empty())
                {
                  inputBuffer[lineN].append(temp);
                }
                deleteStack.push_back(tempData);
                cPosRow -= cRowInc;
                cPosCol = 0;
              }
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
          undo_func();
          isUndoPressed = false;
        }
      }
      else
      {
        isUndoPressed = false;
      }
    }

    if (cPosCol < 0)
      cPosCol = 0;

    if (newPos != cPosCol)
    {
      std::cout << "cursor drawn position :: " << cPosCol << "\n";
      newPos = cPosCol;
    }

    cursor.setPosition(cPosCol + WID_OFFSET, cPosRow);
    win.clear(sf::Color(0, 0, 255));

    int drawColPos{0};
    if (!inputBuffer.empty())
      for (int i = 0; i <= maxDrawable; i++)
      {
        inputRenderBuffer[i].setString(inputBuffer[i]);
        inputRenderBuffer[i].setCharacterSize(characterSize);
        inputRenderBuffer[i].setPosition(0, drawColPos);
        inputRenderBuffer[i].setFillColor(sf::Color(255, 255, 0));
        win.draw(inputRenderBuffer[i]);
        // increase by line division(30) or sumn...
        drawColPos += cRowInc;
      }

    win.draw(cursor);
    editorInfo.str("");
    editorInfo << "ln: " << lineN + 1;
    e_Info.setString(editorInfo.str());
    win.draw(e_Info);
    win.display();
  }

  return 0;
}

// ####################################################
//                  HELPER FUNCTIONS
// ####################################################

// get index of character curssor is at
int getCharPosAt(float cursorXPos)
{
  float total = 0.f;
  for (int i = 0; i < inputBuffer[lineN].size(); ++i)
  {
    std::string cur_char{inputBuffer[lineN][i]};
    tempTxt.setString(cur_char);
    float charWidth = getCharGlyphSize(*cur_char.c_str()).first;
    // if cursorXPos
    if (cursorXPos <= total)
      return i;
    total += charWidth;
  }

  return inputBuffer[lineN].size();
}

/// @brief move the cursor to the right
void cursorMoveRight()
{
  int index = getCharPosAt(cPosCol);
  if (index >= inputBuffer[lineN].size())
    return;
  std::string cur_char{inputBuffer[lineN][index]};
  tempTxt.setString(cur_char);
  cPosCol += getCharGlyphSize(*cur_char.c_str()).first;
}

/// @brief move the cursor to the left
// you want to move back the size o
void cursorMoveLeft()
{
  int index = getCharPosAt(cPosCol);
  if (index < 0)
    return;
  std::string cur_char{inputBuffer[lineN][index]};
  tempTxt.setString(cur_char);
  cPosCol -= getCharGlyphSize(*cur_char.c_str()).first;
  if (cPosCol < 0)
    cPosCol = 0;
}

/// @brief undo...? yeah
void undo_func()
{
  if (!deleteStack.empty())
  {
    // get the data at the end(being the top)
    tempData = deleteStack.back();
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
      break;
    case NEW_INPUT:
      inputBuffer[tempData.lineNumber].erase(tempData.index, 1);
      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      break;
    case ENT_MOVE:
      std::cout << "you want to undo enter\n"
                << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber;

      inputBuffer[tempData.lineNumber].append(tempData.del_char);
      // delete current line
      inputBuffer.erase(inputBuffer.begin() + lineN);
      inputRenderBuffer.erase(inputRenderBuffer.begin() + lineN);

      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;

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
      tempTxt.setString(tempData.del_char);
      inputRenderBuffer.insert(inputRenderBuffer.begin() + tempData.lineNumber, tempTxt);

      lineN = tempData.lineNumber;
      cPosCol = tempData.currCursorPos.x;
      cPosRow = tempData.currCursorPos.y;
      maxDrawable++;
      break;
    default:
      break;
    }
  }
}

std::pair<int, int> getCharGlyphSize(char character)
{
  sf::Glyph glyph = font.getGlyph(character, characterSize, isBold);
  // float charWidth = glyph.advance;  //this one includes spacing to next thingy
  int charWidth = static_cast<int>(glyph.advance);
  int charHeight = static_cast<int>(glyph.bounds.height);

  std::pair<int, int> charDimensions = {charWidth, charHeight};

  return charDimensions;
}