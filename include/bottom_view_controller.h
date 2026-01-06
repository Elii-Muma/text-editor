#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "view.h"
#include "bottom_view.h"
#include "view_controller.h"
#include "Cursor.h"
#include "text_buffer.h"
#include "camera.h"

class BottomViewController : public ViewController
{

public:
  BottomViewController(BottomView &view, sf::Font &font, int &characterSize);
  virtual void handleInput(sf::Event &ev);
  BottomView &getView() override;

protected:
  BottomView &view;
};