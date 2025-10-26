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
}

void Cursor::setPosition(int lineN, int colN)
{
  this->lineN = lineN;
  this->colN = colN;
}

void Cursor::moveCursorLeft(int cursorColumnPosition)
{
  this->colN -= cursorColumnPosition; 
}

void Cursor::moveCursorDown(int lineNumber)
{
  this->colN = 0;
  this->lineN += lineNumber; 
}

void Cursor::moveCursorUp(int lineNumber)
{
  this->colN = 0;
  this->lineN -= lineNumber; 
}

int Cursor::getCursorPosLineNumber()
{
  return lineN;
}

int Cursor::getCursorPosColumnNumber()
{
  return colN;
}



