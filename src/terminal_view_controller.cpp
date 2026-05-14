#include "terminal_view_controller.h"

TerminalViewController::TerminalViewController(TerminalView &view, sf::Font &font, int &characterSize)
    : ViewController{view, font, characterSize}, view{view}
{
  isScreen = view.getIsScreen();
  if(isScreen){
    m_buffer.insertAt(0, "ULTRA TERMINAL V2.0.1", m_stdl::LineType::OUTPUT_LINE);
    // m_buffer.insertAt(1, " ", m_stdl::LineType::OUTPUT_LINE);
    m_cursor.moveCursorDown(m_characterSize); // here is where we can do some math!
  }
}

// process the command input by the user and return the size of the input
std::vector<std::string> TerminalViewController::processCommand(std::string command)
// note: order of pushing command results needs to be reversed
{
  std::vector<std::string> result;
  std::vector<std::string> command_pattern;
  {
    int start_index{0};
    int c_size = command.size();
    for (int i = 0; i < c_size; i++){
      if (command[i] == ' '){
        int end = i - start_index;
        std::string curr_command = command.substr(start_index, end);
        command_pattern.push_back(curr_command);
        start_index = i + 1;
      }
    }

    std::string curr_command = command.substr(start_index);
    if (!curr_command.empty())
      command_pattern.push_back(curr_command);
  }

  int cp_size = command_pattern.size();
  if (command_pattern[0] == "help" || command_pattern[0] == "??"){
    if (cp_size > 1){
      result.push_back("help doesnt currently support " + command_pattern[1]);
      return result;
    }

    std::cout << "running HELP\n";
    result.push_back("RCTRL + F to switch between editor and terminal");
    result.push_back("bugz    : unfixed bugs the editor has");
    result.push_back("clear   : lets you clear the buffer/screen");
    result.push_back("screen  : allows you to move to certain screen");
    result.push_back("stats   : returns your stats");
    result.push_back("menu    : returns the menu");
    result.push_back("For more information on a specific command, type help command-name");
    return result;
  }

  if (command_pattern[0] == "menu"){
    if (cp_size > 1){
      if (command_pattern[1] != "goto"){
        result.push_back("menu doesnt currently support " + command_pattern[1]);
        return result;
      }else{
        if (command_pattern[2] == "settings"){
          result.push_back("execute settings");
          return result;
        }
        result.push_back("execute something from menu");
        return result;
      }

    }
    result.push_back("> games");
    result.push_back("> settings");
    result.push_back("> editor");
    result.push_back("you can type 'menu goto menu-item' to navigate");
    return result;
  }

  if (command_pattern[0] == "clear"){
    m_buffer.getInputBuffer().clear();
    m_buffer.insertAt(0, "ULTRA TERMINAL V2.0.1", m_stdl::LineType::OUTPUT_LINE);
    m_buffer.insertAt(1, " ", m_stdl::LineType::OUTPUT_LINE);
    m_camera.resetCamera();
    m_cursor.resetCursor();
    return result;
  }

  if (command_pattern[0] == "bugz"){
    result.push_back("maybe add a way to add bugs. this part could read from file.");
    result.push_back("2. can only use mono space fonts");
    result.push_back("1. increasing font size messes with the char size");
    return result;
  }

  result.push_back("try using 'help' to see what you can do.");
  result.push_back("the command [" + command_pattern[0] + "] is unknown.");
  return result;
}

void TerminalViewController::handleInput(sf::Event &ev)
{
  if (ev.type == sf::Event::TextEntered && !isProcessingCommand)
  {
    if (!(ev.text.unicode < 32 || ev.text.unicode > 126))
    {
      // this could be a method called add character
      sf::String strChar(static_cast<char>(ev.text.unicode));
      int colN = m_cursor.getCursorPosColumnNumber();
      int lineN = m_cursor.getLineNumber(m_characterSize);
      int index = m_buffer.getCharPosAt(colN, lineN, m_font, m_characterSize);

      m_buffer.addCharacterToBuffer(m_cursor, strChar, index, lineN);
      std::cout << "[colN: " << colN << " || "
                << "lineN: " << lineN << " || "
                << "index: " << index << " || "
                << "added Character: " << std::string(strChar) << "\n";
      cursorMoveRight();
    }
  }

  if (ev.type == sf::Event::KeyPressed)
  {
    int index = 0;
    switch (ev.key.code)
    {
    case sf::Keyboard::Left:
      cursorMoveLeft();
      break;
    case sf::Keyboard::Tab:
      break;
    case sf::Keyboard::Right:
      cursorMoveRight();
      break;
    case sf::Keyboard::Up:
      // cursorMoveUp();
      break;
    case sf::Keyboard::Down:
      // cursorMoveDown();
      break;
    case sf::Keyboard::Home:
      cursorMoveToHome();
      break;
    case sf::Keyboard::End:
      cursorMoveToEnd();
      break;
    case sf::Keyboard::Delete:
    {
      int isNotBackSpace = 0;
      RemoveCharacter(isNotBackSpace);
    }
    break;
    case sf::Keyboard::BackSpace:
    {
      int isBackSpace = 1;
      RemoveCharacter(isBackSpace);
    }
    break;
    case sf::Keyboard::Enter:
      isProcessingCommand = true;
      {
        int lineN = m_cursor.getLineNumber(m_characterSize);
        // why dont i just use the size of the buffer?
        int moveMultiplier{0};
        // if you're using the big screen terminal and not the bottom one
        if (isScreen) {
          // PROCESS THE COMMAND
          {
            std::string command = m_buffer.getInputBuffer()[lineN].first;
            std::cout << "run: [" << command << "]\n";

            std::vector<std::string> t_result = processCommand(command);

            if(!t_result.empty()){

              for (std::string res : t_result)
              {
                std::cout << "result: " << res << "\n";
                m_buffer.insertAt(lineN + moveMultiplier + 1, res, m_stdl::LineType::OUTPUT_LINE);
              }
              moveMultiplier += t_result.size();

              m_buffer.insertAt(lineN + moveMultiplier + 1, " ", m_stdl::LineType::EMPTY_LINE);
              moveMultiplier++;
            }
          }

          {
            int lineN = m_cursor.getLineNumber(m_characterSize);
            int colN = m_cursor.getCursorPosColumnNumber();

            m_buffer.insertAt(lineN + moveMultiplier + 1, " ", m_stdl::LineType::INPUT_LINE);
            moveMultiplier++;
          }
          m_cursor.moveCursorDown(m_characterSize * moveMultiplier); // here is where we can do some math!
          // if(m_cursor.getScreenPosition().y + moveMultiplier > m_camera.viewBottom()){
          unsigned int off = m_camera.viewBottom() - (m_cursor.getScreenPosition().y);
          moveCameraDown(0);
          // }
          std::cout << "buffer size: " << m_buffer.getInputBuffer().size() << "\n";
        }
        isProcessingCommand = false;
      }
      break;
    case sf::Keyboard::LControl:
      // isUndoPressed = true;
      break;
    default:
      break;
    }
  }
  if (ev.type == sf::Event::KeyPressed && isUndoPressed)
  {
    if (ev.key.code == sf::Keyboard::Z)
    {
      std::cout << "control pressed\n";
      int lineN = m_cursor.getLineNumber(m_characterSize);
      sf::Vector2u cursor_pos = m_buffer.undo(lineN, m_font);
      m_cursor.setPosition(cursor_pos.y, cursor_pos.x);
      isUndoPressed = false;
    }
    if (ev.key.code == sf::Keyboard::I)
    {
      std::cout << "control + I pressed \n";
      // characterSize++;
      isUndoPressed = false;
    }
  }
  else
  {
    isUndoPressed = false;
  }
}

TerminalView &TerminalViewController::getView()
{
  return view;
}
