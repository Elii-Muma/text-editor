#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "view.h"
#include "terminal_view.h"
#include "view_controller.h"
#include "Cursor.h"
#include "text_buffer.h"
#include "camera.h"

class TerminalViewController : public ViewController
{

public:
  TerminalViewController(TerminalView &view, sf::Font &font, int &characterSize);
  virtual void handleInput(sf::Event &ev);
  TerminalView &getView() override;

protected:
  TerminalView &view;
  bool isScreen;
};