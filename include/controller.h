#include <string>
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Cursor.h"
#include "Document.h"
#include "camera.h"
#include "view.h"
#include "view_controller.h"
#include "utils.h"

// HANDLES DS AND WHAT NOT
class Controller
{

public:
  Controller(Camera &camera, sf::Font &font, int characterSize, sf::Vector2u *WIN_SIZE);
  void renderSideBorder_UI(sf::RenderWindow &win);
  void renderBottomBorder_UI(sf::RenderWindow &win);
  void render(sf::RenderWindow &win);
  void handleInput(sf::Event &ev);
  void enterFunction();
  // void EraseCharacter(bool isBackSpace, int colN, int index);
  void saveFile();

private:
  Camera &camera;
  Renderer renderer;
  Document document;
  View view;
  ViewController mainViewCtrl;

  bool isUndoPressed;
  // to change from here to some other single place
  sf::Font m_font;
  int m_characterSize;
  int m_BOTTOM_BORDER_HEIGHT{30};

  sf::Vector2u *m_WIN_SIZE;

  // ------DATA STRUCTURES------
  enum dir
  {
    LEFT,
    RIGHT
  };

  // holds the filepaths, ill eventually move this to a different location
  // maybe make it a global file or something
  const struct FilePaths
  {
    std::string txt_filepath{"../resources/txt/"};
  } m_filePaths;

  // will hold the deleted chracters
  enum StackType
  {
    NONE,
    LINE_DEL,
    NORMAL_DEL,
    NEW_INPUT,
    ENT_MOVE
  };

  struct DelData
  {
    std::string del_char;
    int index;
    int lineNumber;
    sf::Vector2f currCursorPos;
    StackType s_type{StackType::NONE};
  };

  std::vector<std::string> inputBuffer; // holds the line string
  std::vector<DelData> deleteStack;     // input/action stack
};
