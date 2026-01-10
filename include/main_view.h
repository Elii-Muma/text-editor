#pragma once
//a buffer here
#include "text_buffer.h"
#include "camera.h"
#include "view.h"
#include "Cursor.h"
#include <utility>



class MainView : public View{

  public:
    MainView(sf::Font &font, int &charSize, sf::Vector2u &windowSize);
    void init();
    void setBGColor(sf::Color color);
    void drawMainScreen(sf::RenderWindow &window);
    void drawLineText(sf::RenderWindow &windowint);
    void drawCursor(sf::RenderWindow &window);
    void drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber);
    void drawSideBorder(sf::RenderWindow &window);

    virtual void renderFixedUI(sf::RenderWindow &window);
    virtual void renderMoveableUI(sf::RenderWindow &window);
    virtual void update();

  private:
    sf::Text m_lineText;
    sf::Text m_lineNumberText;
    sf::RectangleShape m_cursorItem;

    int m_SIDE_BORDER_WIDTH;
    int m_SIDE_BORDER_HEIGHT;
    int m_MAIN_BORDER_WIDTH;
    int m_MAIN_BORDER_HEIGHT;

    sf::RectangleShape m_sideBorder;
    sf::RectangleShape m_mainScreen;

    //holds the filepaths, ill eventually move this to a different location
    //maybe make it a global file or something
    const struct FilePaths{
      std::string txt_filepath{"../resources/txt/"};
    } m_filePaths;
};