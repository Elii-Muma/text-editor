#include "editor_screen.h"

EditorScreen::EditorScreen(sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE, sf::Color color) : 
Screen{font, characterSize, WIN_SIZE},
mainView{font, characterSize, WIN_SIZE}, mainViewCtrl{mainView, font, characterSize},
t_view{font, characterSize, WIN_SIZE, bv_height, false}, bttmViewCtrl{t_view, font, characterSize}
{
  std::cout << "editor screen\n";
  // currView = &mainView;
  mainView.setBGColor(color);
  currViewCtrl = &mainViewCtrl;
}

void EditorScreen::update(){
  t_view.update();
}

void EditorScreen::renderFixedUI(sf::RenderWindow& win) {
  mainView.renderFixedUI(win);
  t_view.renderFixedUI(win);
}

void EditorScreen::renderMoveableUI(sf::RenderWindow& win) {
  mainView.renderMoveableUI(win);
  t_view.renderMoveableUI(win);
}

// sf::View &EditorScreen::getFixedView()
// {
//   // TODO: insert return statement here
// }

sf::View &EditorScreen::getMoveableView()
{
  // TODO: insert return statement here
  return currViewCtrl->getView().getCamera().getMainView();
}

void EditorScreen::inputHandler(sf::Event &ev)
{

  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::LControl:
      isFunctionKeyPressed = true;
      break;
    default:
      break;
    }
  }

  if (ev.type == sf::Event::KeyPressed && isFunctionKeyPressed)
  {
    if (ev.key.code == sf::Keyboard::O)
    {
      std::cout << "switching focus\n";
      curr_state = (curr_state == MAIN_VIEW_STATE) ? BOTTOM_VIEW_STATE: MAIN_VIEW_STATE;
      switch (curr_state)
      {
      case MAIN_VIEW_STATE:
        currViewCtrl = &mainViewCtrl;
        break;
      case BOTTOM_VIEW_STATE:
        currViewCtrl = &bttmViewCtrl;
        break;
      default:
        break;
      }
      isFunctionKeyPressed = false;
    }

    if (ev.key.code == sf::Keyboard::S)
    {
      std::cout << "save file\n";
      // saveFile();
      isFunctionKeyPressed = false;
    }
  }
  currViewCtrl->handleInput(ev);
}
