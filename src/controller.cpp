#include "Controller.h"
#include <iostream>
#include "Cursor.h"
#include "utils.h"

Controller::Controller(sf::Font &font, int& characterSize, sf::Vector2u& WIN_SIZE) : 
renderer{font, WIN_SIZE}, 
e_screen{font, characterSize, WIN_SIZE, themes::deepBlue.BLUE},
t_screen{font, characterSize, WIN_SIZE}
{
  std::printf("init ctrl()\n");
  currScreen = &e_screen;
}

void Controller::update(){
  currScreen->update();
}

void Controller::renderFixedUI(sf::RenderWindow &win){
  // renderer.drawSideBorder(win);
  currScreen->renderFixedUI(win);
}

void Controller::renderMoveableUI(sf::RenderWindow &win){
  currScreen->renderMoveableUI(win);
}

void Controller::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::LControl:
      isFunctionPressed = true;
      break;
    default:
      break;
    }
  }
  
  if (ev.type == sf::Event::KeyPressed && isFunctionPressed)
  {
    if (ev.key.code == sf::Keyboard::F)
    {
      std::cout << "control + I pressed \n";
      curr_screen_state = (curr_screen_state == EDITOR_SCREEN) ? TERMINAL_SCREEN : EDITOR_SCREEN;
      switch (curr_screen_state)
      {
      case EDITOR_SCREEN:
        currScreen= &e_screen;
        break;
      case TERMINAL_SCREEN:
        currScreen= &t_screen;
        break;
      default:
        break;
      }
      // characterSize++;
      isFunctionPressed = false;
    }

  }
  currScreen->inputHandler(ev);
}

// View &Controller::getCurrentFixedView(){
//     // TODO: insert return statement here
// }

sf::View &Controller::getCurrentMoveableView()
{
  // TODO: insert return statement here
    return currScreen->getMoveableView();
}

void Controller::saveFile()
{
  // this is temporarily here, ill find a way to make the path better
  // std::string filename = m_filePaths.txt_filepath + "save_file.txt";
  // std::cout << "filename => " << filename << "\n";
  std::cout << "filename \n";
  // document.saveDocument(filename, mv.getBuffer().getInputBuffer());
}
