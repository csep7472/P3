#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class TextDisplay {
private:
    sf::Text text;

public:
    TextDisplay() {

        text.setString("");
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(0, 0));
    }

    TextDisplay(sf::Font& font, const string& string, int size, const sf::Vector2f& position, sf::Color color) {
        text.setFont(font);
        text.setString(string);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setPosition(position);
    }

    sf::Text& getText() {
        return text;
    }

    void setText(const std::string& newText) {
        text.setString(newText);
    }
    void setPosition(const sf::Vector2f& newPosition) {
        text.setPosition(newPosition);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(text);
    }
};


#endif //GUI_H
