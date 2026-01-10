#include "terminal_screen.h"
#include "terminal_view.h"

TerminalScreen::TerminalScreen(sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE) :
Screen{font, characterSize, WIN_SIZE},t_height{WIN_SIZE.y},
t_view{font, characterSize, WIN_SIZE, t_height, true}, t_viewCtrl{t_view, font, characterSize}
{

}

void TerminalScreen::inputHandler(sf::Event &ev)
{
  t_height = m_winSize.y;
  t_viewCtrl.handleInput(ev);
}

void TerminalScreen::update(){
  t_view.update();
}

void TerminalScreen::renderFixedUI(sf::RenderWindow &win)
{
  t_view.renderFixedUI(win);
}
void TerminalScreen::renderMoveableUI(sf::RenderWindow &win)
{
  t_view.renderMoveableUI(win);
}

sf::View &TerminalScreen::getMoveableView()
{
  // TODO: insert return statement here
  return t_view.getCamera().getMainView();
}