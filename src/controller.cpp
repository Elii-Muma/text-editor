#include "Controller.h"
#include <iostream>
#include "Cursor.h"

Controller::Controller(Camera &camera, sf::Font &font, int& characterSize, sf::Vector2u& WIN_SIZE) : 
renderer{font, WIN_SIZE}, mv{camera, font, characterSize, WIN_SIZE}, mv_ctrl{mv, font, characterSize}, 
other_mv{camera, font, characterSize, WIN_SIZE}, other_mv_ctrl{other_mv, font, characterSize},
bv{camera, font, characterSize, WIN_SIZE, m_height}, bv_ctrl{bv, font, characterSize}
{
  std::printf("init ctrl()\n");
  currViewCtrl = &mv_ctrl;
  inputHandlerCtrl = &mv_ctrl;
}

void Controller::renderFixedUI(sf::RenderWindow &win)
{
  // renderer.drawSideBorder(win);
  currViewCtrl->getView().renderFixedUI(win);
  bv_ctrl.getView().renderFixedUI(win);
}

void Controller::renderMoveableUI(sf::RenderWindow &win)
{
  currViewCtrl->getView().renderMoveableUI(win);
  bv_ctrl.getView().renderMoveableUI(win);
}

void Controller::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::LControl:
      isUndoPressed = true;
      break;
    default:
      break;
    }
  }
  
  if (ev.type == sf::Event::KeyPressed && isUndoPressed)
  {
    if (ev.key.code == sf::Keyboard::I)
    {
      std::cout << "control + I pressed \n";
      currState = (currState == m_stdl::MAIN_SCREEN_STATE) ? m_stdl::TERMINAL_SCREEN_STATE : m_stdl::MAIN_SCREEN_STATE;
      switch (currState)
      {
      case m_stdl::MAIN_SCREEN_STATE:
        currViewCtrl = &mv_ctrl;
        inputHandlerCtrl = &mv_ctrl;
        break;
      case m_stdl::TERMINAL_SCREEN_STATE:
        other_mv_ctrl.getView().setBGColor(sf::Color(0xEF233CFF));
        currViewCtrl = &other_mv_ctrl;
        inputHandlerCtrl = &other_mv_ctrl;
        break;
      default:
        break;
      }
      // characterSize++;
      isUndoPressed = false;
    }

    if (ev.key.code == sf::Keyboard::O)
    {
      std::cout << "CHANGED FOCUS\n";
      inputHandlerCtrl = (inputHandlerCtrl == &bv_ctrl) ? currViewCtrl : &bv_ctrl;
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
  inputHandlerCtrl->handleInput(ev);
}

void Controller::saveFile()
{
  // this is temporarily here, ill find a way to make the path better
  std::string filename = m_filePaths.txt_filepath + "save_file.txt";
  std::cout << "filename => " << filename << "\n";
  document.saveDocument(filename, mv.getBuffer().getInputBuffer());
}
