#pragma once
//a buffer here
#include "text_buffer.h"
#include "camera.h"
#include "Cursor.h"
#include <utility>



class View{

  public:
    View(Camera &camera, sf::Font &font);
    void init();
    void render(sf::RenderWindow &window);
    void drawLineText(sf::RenderWindow &windowint);
    void drawLineNumber(sf::RenderWindow &window, int lineNumber, int numPos, int activeLineNumber);
    void drawSideBorder(sf::RenderWindow &window);
    void drawMainScreen(sf::RenderWindow &window);
    void drawCursor(sf::RenderWindow &window);
    Buffer &getBuffer();
    Cursor &getCursor();
    Camera &getCamera();


  private:

    Camera& m_camera;
    Cursor m_cursor;
    sf::Vector2i winSize;
    Buffer m_buffer;
    sf::Text m_lineText;
    sf::Text m_lineNumberText;
    sf::RectangleShape m_cursorItem;

    int lineN;

    int m_SIDE_BORDER_WIDTH;
    int m_SIDE_BORDER_HEIGHT;
    int m_MAIN_BORDER_WIDTH;
    int m_MAIN_BORDER_HEIGHT;

    sf::RectangleShape m_sideBorder;
    sf::RectangleShape m_mainScreen;

    sf::Font m_font;
    int m_characterSize{22};
    sf::Vector2u *m_WIN_SIZE;

    //holds the filepaths, ill eventually move this to a different location
    //maybe make it a global file or something
    const struct FilePaths{
      std::string txt_filepath{"../resources/txt/"};
    } m_filePaths;
};