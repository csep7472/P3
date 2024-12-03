#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class TextDisplay {
private:
    sf::Text text;
    sf::Font font;

public:
    TextDisplay(const string& string, int size, const sf::Vector2f& position, sf::Color color) {
        if (!font.loadFromFile("../resources/MontserratRegular-BWBEl.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

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


class GraphicDisplay {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    GraphicDisplay(const std::string& textureFilePath, const sf::Vector2f& position) {
        if (!texture.loadFromFile(textureFilePath)) {
            throw std::runtime_error("Failed to load texture");
        }

        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void setPosition(const sf::Vector2f& newPosition) {
        sprite.setPosition(newPosition);
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }
};



#endif //GUI_H
