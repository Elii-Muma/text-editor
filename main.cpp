/**
 * @file main.cpp
 * @author Elijah Muma
 * @brief a simple text ctrl
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
#include <thread>
#include "Controller.h"
#include "camera.h"

int main()
{
  sf::Vector2u winSize{900, 600};
  sf::RenderWindow win(sf::VideoMode({winSize.x, winSize.y}), "Text Controller");
  win.setVerticalSyncEnabled(true);
  sf::Event ev;

  sf::RectangleShape testBox({100.f, 100.f});
  testBox.setPosition(300, 300);


  sf::Font font;
  if (!font.loadFromFile("../resources/fonts/FiraCode-Regular.ttf"))
  {
    std::cout << "failed to load font!\n";
    system("pause");
    return -1;
  }

  int characterSize{22};
  Camera camera{characterSize, winSize};
  sf::View &view = camera.getMainView();
  view = win.getDefaultView();
  sf::View uiView = win.getDefaultView();
  Controller ctrl{camera, font, characterSize, winSize};

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
        uiView.setSize({static_cast<float>(ev.size.width),
                        static_cast<float>(ev.size.height)});
        uiView.setCenter(static_cast<float>(ev.size.width) / 2.f,
                         static_cast<float>(ev.size.height) / 2.f);
        win.setView(view);
        winSize.x = view.getSize().x;
        winSize.y = view.getSize().y;
      }
      // Handle Input
      ctrl.handleInput(ev);
    }

    // Update
    win.clear(sf::Color(0, 0, 255));

    // Render AND SET VIEW
    win.setView(uiView);
    ctrl.renderFixedUI(win);

    win.setView(camera.getMainView());
    ctrl.renderMoveableUI(win);

    // win.setView(uiView);
    // ctrl.renderBottomBorder_UI(win);

    win.display();
  }

  system("pause");
  return 0;
}