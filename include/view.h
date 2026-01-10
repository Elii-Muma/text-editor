#pragma once
//a buffer here
#include "text_buffer.h"
#include "camera.h"
#include "Cursor.h"
#include <utility>



class View{

  public:
    View(sf::Font &font, int &charSize, sf::Vector2u &windowSize);
    // these dont get overridden tho, ever, so should i even make them virtual?
    virtual void renderFixedUI(sf::RenderWindow &window);
    virtual void renderMoveableUI(sf::RenderWindow &window);
    virtual void update() {};
    void updateCursorScreenPos();
    Buffer &getBuffer();
    Cursor &getCursor();
    Camera &getCamera();

  protected:
    Camera m_camera; // what if i made this a copy instead of a reference?
    Cursor m_cursor;
    Buffer m_buffer;
    sf::Vector2u& m_winSize;
    sf::Font& m_font;
    int& m_characterSize;
    int c_posY;
    int prev_fontSize;
};