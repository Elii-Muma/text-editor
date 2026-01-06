#include "Cursor.h"
#include <iostream>

Cursor::Cursor(int lineNumber, int columnNumber) : 
lineN{lineNumber}, colN{columnNumber}
{
  std::cout<<"init Cursor()\n";
}

void Cursor::moveCursorRight(int cursorColumnPosition)
{
  this->colN += cursorColumnPosition; 
  this->xPos += cursorColumnPosition; 
  std::cout << "bottom cursor [lineN: " << colN << ", "
            << "screen_lineN: " << xPos << "]\n";
}

void Cursor::setPosition(int y, int x)
{
  this->lineN = y;
  this->colN = x;
  
  // WATCH THIS FOR BUGS BUNNY
  this->xPos = x;
  this->yPos = y;
}

void Cursor::setScreenPosition(int x, int y)
{
  this->xPos = x;
  this->yPos = y;
}

void Cursor::moveCursorLeft(int cursorColumnPosition)
{
  this->colN -= cursorColumnPosition; 
  this->xPos -= cursorColumnPosition; 
}

void Cursor::moveCursorDown(int lineNumber)
{
  this->colN = 0;
  this->lineN += lineNumber; 
  this->xPos = 0;
  this->yPos += lineNumber;
}

void Cursor::moveCursorUp(int lineNumber)
{
  this->colN = 0;
  this->lineN -= lineNumber; 
  this->yPos -= lineNumber;
}

int Cursor::getCursorPosLineNumber()
{
  return lineN;
}

int Cursor::getCursorPosColumnNumber()
{
  return colN;
}

int Cursor::getLineNumber(int charSize){
  if(lineN < 0)
    return 0;
  return lineN/charSize;

}

// (x, y)

sf::Vector2i Cursor::getScreenPosition()
{
  return sf::Vector2i(xPos, yPos);
}
