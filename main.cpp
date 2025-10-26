/**
 * @file main.cpp
 * @author Elijah Muma
 * @brief a simple text editor
 * @version 0.1
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <math.h>
#include "Editor.h"

int main()
{

  sf::RenderWindow win(sf::VideoMode({800, 600}), "Text Editor");
  sf::Event ev;

  sf::Font font;
  int characterSize{22};

  if(!font.loadFromFile("../resources/fonts/FiraCode-Regular.ttf")){
    std::cout<<"failed to load font!\n";
    return -1;
  }

  Editor editor{font, characterSize};

  while (win.isOpen())
  {
    while (win.pollEvent(ev))
    {
      if (ev.type == sf::Event::Closed)
        win.close();
      
        //Handle Input
        editor.handleInput(ev); 
    }

    //Update
    win.clear(sf::Color(0, 0, 255));

    //Render
    editor.render(win);
    win.display();

  }

  system("pause");
  return 0;
}