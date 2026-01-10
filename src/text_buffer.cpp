#include "text_buffer.h"

Buffer::Buffer()
{
  init();
}

void Buffer::init()
{
  inputBuffer.push_back(" ");
}

void Buffer::addCharacterToBuffer(Cursor &cursor, std::string strChar, int indexOfCursor, int currLineNumber)
{
  // int index = getCharPosAt();
  inputBuffer[currLineNumber].insert(indexOfCursor, strChar);
  m_stdl::DelData tempDelData = {strChar, indexOfCursor, currLineNumber, sf::Vector2f(cursor.getCursorPosColumnNumber(), cursor.getCursorPosLineNumber()), m_stdl::NEW_INPUT};
  deleteStack.push_back(tempDelData);
}

void Buffer::insertAt(int pos, std::string str)
{
  inputBuffer.insert(inputBuffer.begin() + pos, str);
}

int Buffer::getLineLength(int lineNumber, sf::Font &font, int charSize)
{
  int sumOfCharWidth{0};
  for (int i = 0; i < (int)inputBuffer[lineNumber].length(); i++)
  {
    std::string cur_char{inputBuffer[lineNumber][i]};
    sumOfCharWidth += TextUtils::getCharGlyphSize(*cur_char.c_str(), font, charSize).first;
  }
  std::cout << "sum of widths :: " << sumOfCharWidth << "\n";
  return sumOfCharWidth;
}

int Buffer::getCharPosAt(int cursorColumnPos, int lineNumber, sf::Font &font, int charSize)
{
  float total = 0.f;
  if (inputBuffer.empty())
  {
    std::cout << "empty buffer\n";
    return 0;
  }

  if (lineNumber > inputBuffer.size())
  {
    std::cout << "lineNumber out of bounds\n";
    return 0;
  }
  for (int i = 0; i < inputBuffer[lineNumber].size(); ++i)
  {
    std::string cur_char{inputBuffer[lineNumber][i]};
    float charWidth = TextUtils::getCharGlyphSize(*cur_char.c_str(), font, charSize).first;
    // if total(the sum of charwidths) is less that the cursors column pos.
    // we return the index of that char (the one after the cursor)
    if (cursorColumnPos <= total)
    {
      return i;
    }
    total += charWidth;
  }

  // else if there was no character after the cursor return the last characters index
  return inputBuffer[lineNumber].size();
}

// ############## UNDO FUNCTON  ################
// function that handles the undo function
// returns the new cursor position
sf::Vector2u Buffer::undo(int lineN, sf::Font &font)
{
  // must've written this on coke cos ts works perfectly
  if (!deleteStack.empty())
  {
    int cPosCol{0};
    int cPosRow{0};
    // get the data at the end(being the top)
    m_stdl::DelData tempData = deleteStack.back();

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
      case m_stdl::NORMAL_DEL:
        std::cout << "NORMAL_DEL_UNDO::undoing character\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber << "\n";
        // insert the deleted character at its prev pos
        inputBuffer[tempData.lineNumber].insert(tempData.index, tempData.del_char);

        break;
      case m_stdl::NEW_INPUT:
        std::cout << "NEW_INPUT_UNDO::undoing input\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber << "\n";
        inputBuffer[tempData.lineNumber].erase(tempData.index, 1);

        break;
      case m_stdl::ENT_MOVE:
        std::cout << "ENT_MOVE_UNDO::undoing enter\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber << "\n";

        // append the string that was split and moved to the new line with top line that was split
        inputBuffer[tempData.lineNumber].append(tempData.del_char);
        // after appending, delete/erase the new line string that was created
        inputBuffer.erase(inputBuffer.begin() + tempData.lineNumber + 1);

        // update the cursor data with what it was previously

        break;
      case m_stdl::LINE_DEL:
        std::cout << "LINE_DEL_UNDO::undoing line\n\t"
                  << "temp data: " << tempData.del_char << " line number to go to: " << tempData.lineNumber << "\n";
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
          tempTxt.setFont(font);
          tempTxt.setString(tempData.del_char);
        }

        break;
      default:
        break;
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }

    lineN = tempData.lineNumber;
    cPosCol = tempData.currCursorPos.x;
    cPosRow = tempData.currCursorPos.y;

    deleteStack.pop_back();
    return sf::Vector2u(cPosCol, cPosRow);
  }
  else
  {
    std::cout << "delete stack is empty!\n";
    return sf::Vector2u(0, 0);
  }
}

// ############## ENTER FUNCTON  ################
// idk if this belongs here but it seems right
// int index,
// int lineN: normalized line number??,
// int cursorLineN: the coordinate version
// int colN, int m_characterSize
void Buffer::enterFunction(int index, int lineN, int cursorLineN, int colN, int m_characterSize, bool isTerminal)
{
  if (!inputBuffer[lineN].empty() && !isTerminal)
  {
    //SPLIT LINE WHEN CURSOR IS IN THE MIDDLE?
    std::string t{inputBuffer[lineN]};
    std::cout << "before erased result: " << t << "\n";
    t.erase(0, index); // erase from beginning to cursor
    std::cout << "erased result: " << t << "\n";

    inputBuffer[lineN].erase(index); // erase from cursor to end
    // insert to the the line we're currently on + 1 meaning the bottom line
    std::cout << "buffer size before enter:: " << inputBuffer.size() << "\n";
    inputBuffer.insert(inputBuffer.begin() + lineN + 1, t);
    std::cout << "buffer size after enter:: " << inputBuffer.size() << "\n";

    m_stdl::DelData tempDelData = {t, index, lineN, sf::Vector2f(colN, cursorLineN), m_stdl::ENT_MOVE};
    deleteStack.push_back(tempDelData);
  }
  else if (inputBuffer[lineN].empty() || isTerminal)
  {
    inputBuffer.insert(inputBuffer.begin() + lineN + 1, "");
    m_stdl::DelData tempDelData = {"", index, lineN, sf::Vector2f(colN, cursorLineN), m_stdl::ENT_MOVE};
    deleteStack.push_back(tempDelData);
  }
}

std::vector<std::string> &Buffer::getInputBuffer()
{
  return inputBuffer;
}
