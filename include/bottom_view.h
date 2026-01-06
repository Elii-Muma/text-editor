#pragma once
//a buffer here
#include "text_buffer.h"
#include "camera.h"
#include "view.h"
#include "Cursor.h"
#include <utility>

class BottomView : public View{

  public:
    BottomView(Camera &camera, sf::Font &font, int &charSize, sf::Vector2u &windowSize, int &height);
    void init();
    void setBGColor(sf::Color color);
    void drawMainScreen(sf::RenderWindow &window);
    void drawLineText(sf::RenderWindow &windowint);
    void drawCursor(sf::RenderWindow &window);

    virtual void renderFixedUI(sf::RenderWindow &window);
    virtual void renderMoveableUI(sf::RenderWindow &window);

  private:
    int&                m_height; //i dont remeber why i put this here(oh i remember)
    int                 xOffset;
    sf::Text            m_lineText;
    sf::Text            m_arrowText;
    sf::RectangleShape  m_cursorItem;
    sf::RectangleShape  m_mainScreen;

    int CURSOR_HEIGHT;
    int m_MAIN_BORDER_WIDTH;
    int m_MAIN_BORDER_HEIGHT;
    int m_MAIN_BORDER_YPOS;


    //holds the filepaths, ill eventually move this to a different location
    //maybe make it a global file or something
    const struct FilePaths{
      std::string txt_filepath{"../resources/txt/"};
    } m_filePaths;
};