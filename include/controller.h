#include <string>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Cursor.h"
#include "Document.h"
#include "camera.h"
#include "view.h"
#include "main_view.h"
#include "view_controller.h"
#include "main_view_controller.h"
#include "utils.h"

// HANDLES DS AND WHAT NOT
class Controller
{

public:
  Controller(Camera &camera, sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE);
  void renderFixedUI(sf::RenderWindow &win);
  void renderMoveableUI(sf::RenderWindow &win);
  void handleInput(sf::Event &ev);
  // void EraseCharacter(bool isBackSpace, int colN, int index);
  void saveFile();

private:
  Renderer renderer;
  Document document;

  MainView mv;
  MainViewController mv_ctrl;

  MainView other_mv;
  MainViewController other_mv_ctrl;

  ViewController *currViewCtrl;

  bool isUndoPressed;
  m_stdl::states currState{m_stdl::MAIN_SCREEN_STATE};
  int m_BOTTOM_BORDER_HEIGHT{30};

  // holds the filepaths, ill eventually move this to a different location
  // maybe make it a global file or something
  const struct FilePaths
  {
    std::string txt_filepath{"../resources/txt/"};
  } m_filePaths;
};
