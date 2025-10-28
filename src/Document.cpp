#include "Document.h"
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

Document::Document()
{
  std::cout<<"init document()\n";
}

// this process could actually run on a different thread(should i then get a reference of it or a copy?)
bool Document::saveDocument(std::string filename, std::vector<std::string>& buffer)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::stringstream toBeSaved;
    for (auto str : buffer) {
        toBeSaved << str;
        toBeSaved << '\n';
    }
    outputFile << toBeSaved.str();

    outputFile.close();
    std::cout << "file saved to : " << filename <<  std::endl;

  return false;
}


