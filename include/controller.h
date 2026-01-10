#include <string>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Cursor.h"
#include "Document.h"
#include "camera.h"
#include "screen.h"
#include "editor_screen.h"
#include "terminal_screen.h"
#include "utils.h"

// HANDLES DS AND WHAT NOT
class Controller
{

public:
  Controller(sf::Font &font, int &characterSize, sf::Vector2u &WIN_SIZE);
  void update();
  void renderFixedUI(sf::RenderWindow &win);
  void renderMoveableUI(sf::RenderWindow &win);
  void handleInput(sf::Event &ev);
  sf::View& getCurrentFixedView();
  sf::View& getCurrentMoveableView();
  // void EraseCharacter(bool isBackSpace, int colN, int index);
  void saveFile();

private:
  Renderer renderer;
  Document document;

  EditorScreen e_screen;
  TerminalScreen t_screen;

  Screen  *currScreen;
  bool    isFunctionPressed;

  // holds the filepaths, ill eventually move this to a different location
  // maybe make it a global file or something
  const struct FilePaths
  {
    std::string txt_filepath{"../resources/txt/"};
  } m_filePaths;

  enum screenstates {
    EDITOR_SCREEN,
    TERMINAL_SCREEN,
  }curr_screen_state{EDITOR_SCREEN};

};
