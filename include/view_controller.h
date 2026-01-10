#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "view.h"
#include "Cursor.h"
#include "text_buffer.h"
#include "camera.h"

/**@brief 
 * THIS CLASS IS INTENDED TO BE THE CONTROLLER FOR THE VIEW
 * IT CONTROLS THE CURSOR MOVEMENTS, THE VIEWS CAMERA MOVEMENTS ETC
 * BASICALLY IT CONTROLS THE MOVEMENTS OF THE ENTITIES ON THE VIEW
 */

class ViewController{

  public:
    ViewController(View& view, sf::Font& font, int& characterSize);
    virtual void handleInput(sf::Event &ev){std::cout << "handleInput() not overridden\n";}
    void moveCameraUp();
    void moveCameraDown(int bottomOffset);
    void cursorMoveRight();
    void cursorMoveLeft();
    void cursorMoveDown();
    void cursorMoveUp();
    int cursorMoveToCol(int dir);
    void cursorMoveToHome();
    void cursorMoveToEnd();
    void RemoveCharacter(int dir);
    void EraseCharacter(bool isBackSpace, int colN, int index);
    virtual View& getView();

  protected:
    Buffer& m_buffer;
    Cursor& m_cursor;
    Camera& m_camera;
    bool isUndoPressed{false};
    int& m_characterSize;
    sf::Font& m_font;
    View&   m_view;

};