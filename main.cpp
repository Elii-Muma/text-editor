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
  sf::Vector2u *winSize = new sf::Vector2u(600, 800);
  sf::RenderWindow win(sf::VideoMode({winSize->y, winSize->x}), "Text Editor");
  sf::Event ev;
  sf::View view = win.getDefaultView();

  sf::Font font;
  int characterSize{22};

  if (!font.loadFromFile("../resources/fonts/FiraCode-Regular.ttf"))
  {
    std::cout << "failed to load font!\n";
    return -1;
  }

  Editor editor{font, characterSize, winSize};

  while (win.isOpen())
  {
    while (win.pollEvent(ev))
    {
      if (ev.type == sf::Event::Closed)
        win.close();

      if (ev.type == sf::Event::Resized)
      {
        view.setSize({static_cast<float>(ev.size.width),
                      static_cast<float>(ev.size.height)});
        view.setCenter(static_cast<float>(ev.size.width) / 2.f,
                       static_cast<float>(ev.size.height) / 2.f);
        win.setView(view);
        winSize->x = view.getSize().x;
        winSize->y = view.getSize().y;
      }
      // Handle Input
      editor.handleInput(ev);
    }

    // Update
    win.clear(sf::Color(0, 0, 255));

    // Render
    editor.render(win);
    win.display();
  }

  system("pause");
  return 0;
}