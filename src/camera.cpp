#include "camera.h"

Camera::Camera(int characterSize, sf::Vector2u &WIN_SIZE) : 
m_view({WIN_SIZE.x/2, WIN_SIZE.y/2},{WIN_SIZE.x, WIN_SIZE.y}), 
m_charSize(characterSize*2), m_scrollSpeed(characterSize), m_WIN_SIZE(WIN_SIZE)
{
  std::cout << "init camera()";
}
sf::View &Camera::getMainView()
{
  return m_view;
}

sf::Vector2f Camera::getCenter(){
  return m_view.getCenter();  
}
sf::Vector2f Camera::getSize(){
  return m_view.getSize();
}

int Camera::viewTop(){
  return m_view.getCenter().y - (getSize().y / 2);
}

int Camera::viewBottom(){
  return m_view.getCenter().y + (getSize().y / 2);
}

void Camera::cameraScrollDown()
{
  m_view.move({0, m_scrollSpeed});
}

void Camera::cameraScrollDown(int scrollSpeed)
{
  m_view.move({0, scrollSpeed});
}

void Camera::cameraJumpDown()
{
  m_view.move({0, m_charSize*2});
}

void Camera::cameraScrollUp()
{
  m_view.move({0, -m_scrollSpeed});
}

void Camera::resetCamera(){
  m_view.setCenter(static_cast<float>(m_WIN_SIZE.x) / 2.f,
                 static_cast<float>(m_WIN_SIZE.y) / 2.f);
}