#pragma once
#include <vector>
#include <string>

class Document{
  public:
    Document();
    bool saveDocument(std::string filename, std::vector<std::string> &buffer);

  private:
};