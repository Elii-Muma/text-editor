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
    ViewController(View& view, sf::Font& font, int characterSize);
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

  private:
    Buffer& m_buffer;
    View&   m_view;
    Cursor& m_cursor;
    Camera& m_camera;
    int m_characterSize;
    sf::Font m_font;

};