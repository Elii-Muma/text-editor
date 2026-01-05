#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Camera{
  public:
    Camera(int characterSize, sf::Vector2u& WIN_SIZE);
    sf::View& getMainView();
    sf::Vector2f getCenter();
    sf::Vector2f getSize();
    int viewTop();
    int viewBottom();
    void cameraScrollDown();
    void cameraScrollUp();
    void cameraJumpUp();
    void cameraJumpDown();
    void moveCameraLeft();
  private:
    sf::View m_view;
    int m_charSize;
    int m_scrollSpeed;
};