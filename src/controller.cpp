#include "Controller.h"
#include <iostream>
#include "Cursor.h"

Controller::Controller(Camera &camera, sf::Font &font, int characterSize, sf::Vector2u *WIN_SIZE) : m_font{font}, m_characterSize{characterSize}, renderer{font, WIN_SIZE}, m_WIN_SIZE{WIN_SIZE},
                                                                                                    camera{camera}, view{camera, font}, mainViewCtrl{view, font, characterSize}
{
  std::printf("init ctrl()\n");
}

void Controller::renderSideBorder_UI(sf::RenderWindow &win)
{
  // renderer.drawSideBorder(win);
  view.drawSideBorder(win);
  view.drawMainScreen(win);
}

void Controller::renderBottomBorder_UI(sf::RenderWindow &win)
{
  // renderer.drawBottomBorder(win);
}

void Controller::render(sf::RenderWindow &win)
{
  view.render(win);
}

void Controller::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::TextEntered)
  {
    if (!(ev.text.unicode < 32 || ev.text.unicode > 126))
    {
      // this could be a method called add character
      sf::String strChar(static_cast<char>(ev.text.unicode));
      int colN = view.getCursor().getCursorPosColumnNumber();
      int lineN = view.getCursor().getLineNumber(m_characterSize);
      int index = view.getBuffer().getCharPosAt(colN, lineN, m_font, m_characterSize);

      view.getBuffer().addCharacterToBuffer(view.getCursor(), strChar, index, lineN);
      std::cout << "[colN: " << colN << " || "
                << "lineN: " << lineN << " || "
                << "index: " << index << " || "
                << "added Character: " << std::string(strChar) << " || ";
      mainViewCtrl.cursorMoveRight();
    }
  }

  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::Left:
      mainViewCtrl.cursorMoveLeft();
      break;
    case sf::Keyboard::Tab:
    {

      sf::String strChar("----|");
      int colN = view.getCursor().getCursorPosColumnNumber();
      int lineN = view.getCursor().getLineNumber(m_characterSize);
      int index = view.getBuffer().getCharPosAt(colN, lineN, m_font, m_characterSize);

      view.getBuffer().addCharacterToBuffer(view.getCursor(), strChar, index, lineN);
      mainViewCtrl.cursorMoveRight();
      mainViewCtrl.cursorMoveRight();
      mainViewCtrl.cursorMoveRight();
      mainViewCtrl.cursorMoveRight();
      mainViewCtrl.cursorMoveRight();

      // change this to maybe move to the right by the new entry size, cos look at this shit
    }
    break;
    case sf::Keyboard::Right:
      mainViewCtrl.cursorMoveRight();
      break;
    case sf::Keyboard::Up:
      mainViewCtrl.cursorMoveUp();
      break;
    case sf::Keyboard::Down:
      mainViewCtrl.cursorMoveDown();
      break;
    case sf::Keyboard::Home:
      mainViewCtrl.cursorMoveToHome();
      break;
    case sf::Keyboard::End:
      mainViewCtrl.cursorMoveToEnd();
      break;
    case sf::Keyboard::Delete:
    {
      int isNotBackSpace = 0;
      mainViewCtrl.RemoveCharacter(isNotBackSpace);
    }
    break;
    case sf::Keyboard::BackSpace:
    {
      int isBackSpace = 1;
      mainViewCtrl.RemoveCharacter(isBackSpace);
    }
    break;
    case sf::Keyboard::Enter:
      mainViewCtrl.moveCameraDown(30);
      {
        int lineN = view.getCursor().getLineNumber(m_characterSize);
        int colN = view.getCursor().getCursorPosColumnNumber();
        index = view.getBuffer().getCharPosAt(colN, lineN, m_font, m_characterSize);
        view.getBuffer().enterFunction(index, lineN, view.getCursor().getCursorPosLineNumber(), colN, m_characterSize);
      }
      view.getCursor().moveCursorDown(m_characterSize);
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
      int lineN = view.getCursor().getLineNumber(m_characterSize);
      sf::Vector2i cursor_pos = view.getBuffer().undo(lineN, m_font);
      view.getCursor().setPosition(cursor_pos.y, cursor_pos.x);
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

void Controller::saveFile()
{
  // this is temporarily here, ill find a way to make the path better
  std::string filename = m_filePaths.txt_filepath + "save_file.txt";
  std::cout << "filename => " << filename << "\n";
  document.saveDocument(filename, view.getBuffer().getInputBuffer());
}
