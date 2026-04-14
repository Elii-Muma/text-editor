#pragma once
#include "screen.h"
#include "terminal_view.h"
#include "terminal_view_controller.h"

// THE TERMINAL IS BASICALLY JUST A BIG ASS BOTTOM VIEW OR I COULD HAVE THEM SWITCH-
// THE BOTTOM VIEW COULD BE A SMALLER TERMINAL, WHICH LOWKEY MAKES SENSE...#
// LET ME DO THAT
class TerminalScreen : public Screen
{
public:
  TerminalScreen(sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE);
  virtual void inputHandler(sf::Event &ev);
  virtual void renderFixedUI(sf::RenderWindow &win);
  virtual void renderMoveableUI(sf::RenderWindow &win);
  virtual void update();
  virtual sf::View& getMoveableView();

private:
  TerminalView t_view;
  TerminalViewController t_viewCtrl;
  unsigned int t_height{30};

  bool isFunctionKeyPressed{false};
};