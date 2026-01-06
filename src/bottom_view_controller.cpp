#include "bottom_view_controller.h"

BottomViewController::BottomViewController(BottomView &view, sf::Font &font, int &characterSize)
    : ViewController{view, font, characterSize}, view{view}
{
}

void BottomViewController::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::TextEntered)
  {
    if (!(ev.text.unicode < 32 || ev.text.unicode > 126))
    {
      // this could be a method called add character
      sf::String strChar(static_cast<char>(ev.text.unicode));
      int colN = m_cursor.getCursorPosColumnNumber();
      int lineN = m_cursor.getLineNumber(m_characterSize);
      int index = m_buffer.getCharPosAt(colN, lineN, m_font, m_characterSize);

      m_buffer.addCharacterToBuffer(m_cursor, strChar, index, lineN);
      std::cout << "[colN: " << colN << " || "
                << "lineN: " << lineN << " || "
                << "index: " << index << " || "
                << "added Character: " << std::string(strChar) << "\n";
      cursorMoveRight();
    }
  }

  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::Left:
      cursorMoveLeft();
      break;
    case sf::Keyboard::Tab:
    break;
    case sf::Keyboard::Right:
      cursorMoveRight();
      break;
    case sf::Keyboard::Up:
      cursorMoveUp();
      break;
    case sf::Keyboard::Down:
      cursorMoveDown();
      break;
    case sf::Keyboard::Home:
      cursorMoveToHome();
      break;
    case sf::Keyboard::End:
      cursorMoveToEnd();
      break;
    case sf::Keyboard::Delete:
    {
      int isNotBackSpace = 0;
      RemoveCharacter(isNotBackSpace);
    }
    break;
    case sf::Keyboard::BackSpace:
    {
      int isBackSpace = 1;
      RemoveCharacter(isBackSpace);
    }
    break;
    case sf::Keyboard::Enter:
      {
        int lineN = m_cursor.getCursorPosLineNumber();
        std::cout<< "run: " << m_buffer.getInputBuffer()[lineN] << "\n";
      }
      break;
    case sf::Keyboard::LControl:
      // isUndoPressed = true;
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
      int lineN = m_cursor.getLineNumber(m_characterSize);
      sf::Vector2u cursor_pos = m_buffer.undo(lineN, m_font);
      m_cursor.setPosition(cursor_pos.y, cursor_pos.x);
      isUndoPressed = false;
    }
    if (ev.key.code == sf::Keyboard::I)
    {
      std::cout << "control + I pressed \n";
      // characterSize++;
      isUndoPressed = false;
    }

    // if (ev.key.code == sf::Keyboard::S)
    // {
    //   std::cout << "savefile pressed\n";
    //   saveFile();
    //   isUndoPressed = false;
    // }
  }
  else
  {
    isUndoPressed = false;
  }
}

BottomView &BottomViewController::getView()
{
  return view;
}
