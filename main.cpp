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

void resizeView(sf::View &view, unsigned int width, unsigned int height)
{
  view.setSize({static_cast<float>(width),
                static_cast<float>(height)});
  view.setCenter(static_cast<float>(width) / 2.f,
                 static_cast<float>(height) / 2.f);
}

int main()
{
  sf::Vector2u winSize{900, 600};
  sf::RenderWindow win(sf::VideoMode({winSize.x, winSize.y}), "Text Controller");
  win.setVerticalSyncEnabled(true);
  sf::Event ev;

  sf::RectangleShape testBox({100.f, 100.f});
  testBox.setPosition(300, 300);

  sf::Font font;
  //TODO: it actually does not work well with variable font sizes
  if (!font.loadFromFile("../resources/fonts/FiraCode-Regular.ttf"))
  // if (!font.loadFromFile("../resources/fonts/ShadowsIntoLight-Regular.ttf"))
  // if (!font.loadFromFile("../resources/fonts/Quicksand-Regular.ttf"))
  {
    std::cout << "failed to load font!\n";
    system("pause");
    return -1;
  }

  int characterSize{32};
  bool isCtrlPressed{false};
  sf::View uiView = win.getDefaultView();
  Controller ctrl{font, characterSize, winSize};

  while (win.isOpen())
  {
    while (win.pollEvent(ev))
    {
      if (ev.type == sf::Event::Closed)
        win.close();

      if (ev.type == sf::Event::Resized)
      {
        sf::View &view = ctrl.getCurrentMoveableView();
        unsigned int width = ev.size.width;
        unsigned int height = ev.size.height;
        resizeView(view, width, height);
        resizeView(uiView, width, height);

        winSize.x = view.getSize().x;
        winSize.y = view.getSize().y;
      }
      if (ev.type == sf::Event::KeyPressed ){
        if (ev.key.code == sf::Keyboard::RControl) isCtrlPressed = true; 

        if (isCtrlPressed){
          if ((ev.key.code == sf::Keyboard::Up) ){
            characterSize++;
            std::cout << "fontSize:: " << characterSize << "\n";
            isCtrlPressed = false;
          }
          if ((ev.key.code == sf::Keyboard::Down) && isCtrlPressed){
            characterSize--;
            std::cout << "fontSize:: " << characterSize << "\n";
            isCtrlPressed = false;
          }
        }
      }
        
      ctrl.handleInput(ev);
    }

    // Update
    if(winSize.x != ctrl.getCurrentMoveableView().getSize().x || winSize.y != ctrl.getCurrentMoveableView().getSize().y){
      std::cout << "|---->resize!!!\n";
      resizeView(ctrl.getCurrentMoveableView(), winSize.x, winSize.y);
    }
    ctrl.update();
    win.clear(sf::Color(0, 0, 255));

    // Render AND SET VIEW
    win.setView(uiView);
    ctrl.renderFixedUI(win);

    win.setView(ctrl.getCurrentMoveableView());
    ctrl.renderMoveableUI(win);

    // win.setView(uiView);
    // ctrl.renderBottomBorder_UI(win);

    win.display();
  }

  system("pause");
  return 0;
}