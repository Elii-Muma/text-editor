#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "camera.h"

class Screen{
  public:
    Screen(sf::Font &font, int& characterSize, sf::Vector2u& WIN_SIZE)
    :m_font{font}, m_charSize{characterSize}, m_winSize{WIN_SIZE}, m_camera{characterSize, WIN_SIZE}
    {std::cout << "init screen();";}

    virtual void      inputHandler(sf::Event& ev) { }
    virtual void      renderFixedUI(sf::RenderWindow& win) { }
    virtual void      renderMoveableUI(sf::RenderWindow& win) { }
    virtual void      update(){ }
    virtual sf::View& getFixedView(){ return m_camera.getMainView(); }
    virtual sf::View& getMoveableView(){ return m_camera.getMainView(); }
    
  protected:
    Camera        m_camera;
    int&          m_charSize;
    sf::Vector2u& m_winSize;
    sf::Font&     m_font; 
    // View* currView; //allow for switching views to focus on? maybe i dont need this because they'll all be rendered either way

};