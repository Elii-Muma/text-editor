#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "view.h"
#include "main_view.h"
#include "view_controller.h"
#include "Cursor.h"
#include "text_buffer.h"
#include "camera.h"

class MainViewController : public ViewController
{

public:
  MainViewController(MainView &view, sf::Font &font, int &characterSize);
  virtual void handleInput(sf::Event &ev);
  MainView &getView() override;

protected:
  MainView &view;
};