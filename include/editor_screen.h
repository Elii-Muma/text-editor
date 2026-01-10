#pragma once
#include "screen.h"
#include "main_view.h"
#include "main_view_controller.h"
#include "terminal_view.h"
#include "terminal_view_controller.h"

class EditorScreen : public Screen
{
public:
  EditorScreen(sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE, sf::Color color);
  virtual void update();
  virtual void inputHandler(sf::Event &ev);
  virtual void renderFixedUI(sf::RenderWindow &win);
  virtual void renderMoveableUI(sf::RenderWindow &win);
  // virtual sf::View& getFixedView();
  virtual sf::View& getMoveableView();

private:
  MainView mainView;
  MainViewController mainViewCtrl;
  TerminalView t_view;
  TerminalViewController bttmViewCtrl;
  unsigned int bv_height{38};

  ViewController *currViewCtrl; // allow for switching inputHandlers

  bool isFunctionKeyPressed{false};
  enum EditorScreenStates
  {
    MAIN_VIEW_STATE,
    BOTTOM_VIEW_STATE
  } curr_state{MAIN_VIEW_STATE};
};