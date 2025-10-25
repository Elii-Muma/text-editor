#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <cmath>

const float CURSOR_WIDTH = 2.f;
const float CURSOR_HEIGHT = 30.f;
const int cRowInc = 30;
const int MAX_LINE = 50;
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 800;
float cPosCol{0};
float cPosRow{0};
int lineN{0};
int maxDrawable{0};

std::vector<std::string> inputBuffer;
std::vector<sf::Text> inputRenderBuffer;

sf::Text tempTxt;

int getCharPosAt(float cursorX) {
    float total = 0.f;
    for (int i = 0; i < inputBuffer[lineN].size(); ++i) {
        std::string cur_char{inputBuffer[lineN][i]};
        tempTxt.setString(cur_char);
        float charWidth = tempTxt.getLocalBounds().width + 4.f;
        //return the index of character the cursor is at
        //if found
        if (cursorX < total + charWidth / 2)
            return i;
        total += charWidth;
    }
    //else its at the end or if empty, at the beginning
    return inputBuffer[lineN].size();
}

void cursorMoveLeft() {
    int index = getCharPosAt(cPosCol) - 1;
    if (index < 0) return;
    std::string cur_char{inputBuffer[lineN][index]};
    tempTxt.setString(cur_char);
    float width = tempTxt.getLocalBounds().width + 4.f;
    cPosCol -= width;
    if (cPosCol < 0) cPosCol = 0;
}

void cursorMoveRight() {
    int index = getCharPosAt(cPosCol);
    if (index >= inputBuffer[lineN].size()) return;
    std::string cur_char{inputBuffer[lineN][index]};
    tempTxt.setString(cur_char);
    float width = tempTxt.getLocalBounds().width + 4.f;
    cPosCol += width;
}

int main() {
    sf::RenderWindow win(sf::VideoMode({ WIN_WIDTH, WIN_HEIGHT }), "Text Editor");
    sf::Event ev;
    sf::RectangleShape cursor;
    cursor.setSize(sf::Vector2f(CURSOR_WIDTH, CURSOR_HEIGHT));

    sf::Font font;
    font.loadFromFile("resources/FiraCode-Regular.ttf");

    tempTxt.setFont(font);
    tempTxt.setString("");

    std::string newStr{""};

    for (int i = 0; i < MAX_LINE; i++) {
        sf::Text text;
        text.setFont(font);
        std::string str{""};
        inputRenderBuffer.push_back(text);
        inputBuffer.push_back(str);
    }

    while (win.isOpen()) {
        while (win.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                win.close();

            if (ev.type == sf::Event::TextEntered) {
                if (ev.text.unicode < 32 || ev.text.unicode > 126) continue; // skip non-printable
                std::string str{ static_cast<char>(ev.text.unicode) };
                int index = getCharPosAt(cPosCol);
                inputBuffer[lineN].insert(index, str);
                tempTxt.setString(str);
                cPosCol += tempTxt.getLocalBounds().width + 4.f;
                if (maxDrawable < lineN)
                    maxDrawable = lineN;
            }

            if (ev.type == sf::Event::KeyPressed) {
                float cursorX = cPosCol;
                int len = inputBuffer[lineN].size();

                switch (ev.key.code) {
                case sf::Keyboard::Enter:
                    inputBuffer.insert(inputBuffer.begin() + lineN + 1, "");
                    inputRenderBuffer.insert(inputRenderBuffer.begin() + lineN + 1, tempTxt);
                    cPosRow += cRowInc;
                    cPosCol = 0;
                    lineN++;
                    maxDrawable++;
                    break;
                case sf::Keyboard::Left:
                    if (cursorX > 0)
                        cursorMoveLeft();
                    break;
                case sf::Keyboard::Right:
                    if (cursorX < WIN_WIDTH && getCharPosAt(cursorX) < len)
                        cursorMoveRight();
                    break;
                case sf::Keyboard::Up:
                    if (cPosRow > 0) {
                        cPosRow -= cRowInc;
                        lineN--;
                        cPosCol = 0;
                    }
                    break;
                case sf::Keyboard::Down:
                    if (cPosRow + cRowInc < WIN_HEIGHT && lineN + 1 < inputBuffer.size()) {
                        cPosRow += cRowInc;
                        lineN++;
                        cPosCol = 0;
                    }
                    break;
                }
            }
        }

        cursor.setPosition(cPosCol, cPosRow);
        win.clear(sf::Color(130, 130, 130));
        win.draw(cursor);

        int drawColPos{0};
        for (int i = 0; i <= maxDrawable; i++) {
            inputRenderBuffer[i].setString(inputBuffer[i]);
            inputRenderBuffer[i].setPosition(0, drawColPos);
            win.draw(inputRenderBuffer[i]);
            drawColPos += cRowInc;
        }

        win.display();
    }

    return 0;
}
