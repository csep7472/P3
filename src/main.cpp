#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include "splay.h"
#include "api.h"
#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include "hashmap.h"

using namespace std;

vector<string> evaluateGuess(HashMap<string, Game>& gameMap, string& userGuess, Game& correctGame) {
    vector<string> results;

    // Name comparison
    if (gameMap[userGuess].name == correctGame.name) {
        cout << "Name is correct!" << endl;
        results.emplace_back("green");
    } else {
        results.emplace_back("red");
    }

    // Platforms comparison
    if (gameMap[userGuess].platforms == correctGame.platforms) {
        cout << "Platforms are correct!" << endl;
        results.emplace_back("green");
    } else {
        int matches = 0;
        for (const auto& guessPlatform : gameMap[userGuess].platforms) {
            for (const auto& correctPlatform : correctGame.platforms) {
                if (guessPlatform == correctPlatform)
                    matches++;
            }
        }
        results.emplace_back(matches > 0 ? "yellow" : "red");
        cout << (matches > 0 ? "At least one platform matches!" : "No platforms match") << endl;
    }

    // Genres comparison
    if (gameMap[userGuess].genres == correctGame.genres) {
        cout << "Genres are correct!" << endl;
        results.emplace_back("green");
    } else {
        int matches = 0;
        for (const auto& guessGenre : gameMap[userGuess].genres) {
            for (const auto& correctGenre : correctGame.genres) {
                if (guessGenre == correctGenre)
                    matches++;
            }
        }
        results.emplace_back(matches > 0 ? "yellow" : "red");
        cout << (matches > 0 ? "At least one genre matches!" : "No genres match") << endl;
    }

    // Release year comparison
    if (gameMap[userGuess].releaseYear == correctGame.releaseYear) {
        results.emplace_back("equal");
        cout << "The release year is correct!" << endl;
    } else if (gameMap[userGuess].releaseYear < correctGame.releaseYear) {
        results.emplace_back("up");
        cout << "The release year of the guessed game is older than the correct game" << endl;
    } else {
        results.emplace_back("down");
        cout << "The release year of the guessed game is newer than the correct game" << endl;
    }

    // Game modes comparison
    if (gameMap[userGuess].gameModes == correctGame.gameModes) {
        cout << "Game modes are correct!" << endl;
        results.emplace_back("green");
    } else {
        int matches = 0;
        for (const auto& guessMode : gameMap[userGuess].gameModes) {
            for (const auto& correctMode : correctGame.gameModes) {
                if (guessMode == correctMode)
                    matches++;
            }
        }
        results.emplace_back(matches > 0 ? "yellow" : "red");
        cout << (matches > 0 ? "At least one game mode matches!" : "No game modes match") << endl;
    }

    // Perspectives comparison
    if (gameMap[userGuess].perspectives == correctGame.perspectives) {
        cout << "Perspectives are correct!" << endl;
        results.emplace_back("green");
    } else {
        int matches = 0;
        for (const auto& guessPerspective : gameMap[userGuess].perspectives) {
            for (const auto& correctPerspective : correctGame.perspectives) {
                if (guessPerspective == correctPerspective)
                    matches++;
            }
        }
        results.emplace_back(matches > 0 ? "yellow" : "red");
        cout << (matches > 0 ? "At least one perspective matches!" : "No perspectives match") << endl;
    }

    return results;
}

int main() {
    vector<Game> allGames = getGamesList();
    SplayTree splaytree;
    HashMap<string, Game> gameMap(100);  // Create a HashMap with 100 buckets
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Video Game Wordle", sf::Style::Fullscreen);

    // gets window size to dynamically adjust text and button locations
    sf::Vector2u windowSize = window.getSize();

    // creating and setting position for title text
    TextDisplay title("Video Game Wordle", 50,  sf::Vector2f(0, 0), sf::Color::Black);
    sf::FloatRect titleRect = title.getText().getGlobalBounds();
    float centerX = (windowSize.x - titleRect.width) / 2;
    title.setPosition(sf::Vector2f(centerX-10, windowSize.y * 0.1f));

    // creating and setting position for Splay Tree and Hash Map text
    TextDisplay splayText("Splay Tree", 30, sf::Vector2f(centerX, windowSize.y * 0.5f), sf::Color::Black);
    TextDisplay hashText("Hash Map", 30, sf::Vector2f(centerX + titleRect.width, windowSize.y * 0.5f), sf::Color::Black);
    sf::FloatRect hashRect = hashText.getText().getGlobalBounds();
    hashText.setPosition(sf::Vector2f(centerX + titleRect.width - hashRect.width, windowSize.y * 0.5f));

    TextDisplay welcomeText("Select a Game Mode: ", 40, sf::Vector2f(centerX, windowSize.y * 0.3f), sf::Color::Black);
    sf::FloatRect welcomeTextRect = welcomeText.getText().getGlobalBounds();
    float welcomeCenter = (windowSize.x - welcomeTextRect.width) / 2;
    welcomeText.setPosition(sf::Vector2f(welcomeCenter, windowSize.y * 0.3));

    // creates and positions boxes for splay and hash mode
    sf::RectangleShape splayBox(sf::Vector2f(splayText.getText().getGlobalBounds().width + 20, 40));
    splayBox.setPosition(centerX - 10, windowSize.y * 0.5f);
    splayBox.setFillColor(sf::Color::Green);
    sf::RectangleShape hashBox(sf::Vector2f(hashText.getText().getGlobalBounds().width + 20, 40));
    hashBox.setPosition(centerX + titleRect.width - hashRect.width - 10, windowSize.y * 0.5f);
    hashBox.setFillColor(sf::Color::Blue);

    // create and position time it takes to insert
    TextDisplay insertTime("time to insert all games: ", 20, sf::Vector2f(0, windowSize.y - 30), sf::Color::Black);

    // user input area
    sf::RectangleShape searchBox(sf::Vector2f(600, 30));
    searchBox.setPosition((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2);
    searchBox.setOutlineThickness(3);
    searchBox.setOutlineColor(sf::Color::Black);
    TextDisplay input("Enter Your Guess here", 20, sf::Vector2f(0, 0), sf::Color::Black);
    input.setPosition(sf::Vector2f(searchBox.getPosition().x + 5, windowSize.y * 0.2));
    sf::RectangleShape guessButton(sf::Vector2f(80, 30));
    guessButton.setPosition(searchBox.getPosition().x + 620, windowSize.y * 0.2);
    guessButton.setOutlineThickness(3);
    guessButton.setOutlineColor(sf::Color::Black);
    TextDisplay guessText("Guess!", 20, sf::Vector2f(0, 0), sf::Color::Black);
    sf::FloatRect guessTextRect = guessText.getText().getGlobalBounds();
    guessText.setPosition(sf::Vector2f(guessButton.getPosition().x + (guessButton.getSize().x - guessTextRect.width) / 2, windowSize.y * 0.2));

    // Boxes for rules
    vector<sf::RectangleShape> boxes(5);
    for (int i = 0; i < 5; ++i) {
        boxes[i].setSize(sf::Vector2f(150, 120));
        boxes[i].setOutlineThickness(3);
        boxes[i].setOutlineColor(sf::Color::Black);
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setPosition((windowSize.x - (5 * 150)) / 6.0f * (i + 1) + 150 * i, windowSize.y * 0.05);
    }
    TextDisplay red("Red: None of the \nclues match.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (0 + 1) + 150 * 0 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay yellow("Yellow: One or \nmore of the clues \nmatch, but not all.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (1 + 1) + 150 * 1 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay green("Green: All clues \nmatch!", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (2 + 1) + 150 * 2 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay up("Arrow Up: The \ngame you have \nguessed is older \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (3 + 1) + 150 * 3 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay down("Arrow down: The \ngame you have \nguessed is newer \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (4 + 1) + 150 * 4 + 5, windowSize.y * 0.05), sf::Color::Black);

    // boxes for guess hints
    vector<sf::RectangleShape> hints(7);
    for (int i = 0; i < 7; ++i) {
        hints[i].setSize(sf::Vector2f(100, 30));
        hints[i].setOutlineThickness(3);
        hints[i].setOutlineColor(sf::Color::Black);
        hints[i].setFillColor(sf::Color::White);
        hints[i].setPosition((windowSize.x - (7 * 80)) / 8.0f * (i + 1) + 80 * i, windowSize.y * 0.25);
    }
    vector<string> list {"Name", "Genres", "Platform", "Perspectives", "Game Modes", "Release Year", "Ratings"};
    vector<TextDisplay> hint_displays;
    TextDisplay name("Name", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay genre("Genres", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay platform("Platform", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay pers("Perspectives", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay gamemode("Game Modes", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay year("Release Year", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay rating("Ratings", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, windowSize.y * 0.26), sf::Color::Black);



    bool gameStarted = false;
    string inputText = "";
    string mode = "";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // reference: https://gamedev.stackexchange.com/questions/142308/how-to-create-a-button-in-sfml
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                chrono::duration<double> time;
                if (splayBox.getGlobalBounds().contains(mousePosF) && !gameStarted) {
                    auto start = chrono::high_resolution_clock::now();
                    for (const auto& game : allGames) {
                        splaytree.insert(game);
                        cout << game.name << endl;
                    }
                    auto end = chrono::high_resolution_clock::now();
                    time = end - start;
                    gameStarted = true;
                    mode = "splay";
                }

                else if (hashBox.getGlobalBounds().contains(mousePosF) && !gameStarted) {
                    auto start = chrono::high_resolution_clock::now();
                    for (const auto& game : allGames) {
                        gameMap[game.name] = game;  // Insert into custom HashMap
                    }
                    auto end = chrono::high_resolution_clock::now();
                    time = end - start;
                    gameStarted = true;
                    mode = "hash";
                }
                insertTime.setText("time to insert all games: " + to_string(time.count()) + "s");

                if (guessButton.getGlobalBounds().contains(mousePosF) && gameStarted) {

                }
            }

            if (event.type == sf::Event::TextEntered && gameStarted) {
                if (event.text.unicode == '\b' && inputText.size() > 0) {
                    inputText.erase(inputText.size()-1, 1);
                }

                else if (event.text.unicode == '\r' && inputText.size() > 0) {
                    if (mode == "splay") {
                        if (splaytree.search(inputText) == nullptr) {
                            cout << "Not Found" << endl;
                        }
                        else {
                            cout << "Found" << endl;
                        }
                    }

                    else if (mode == "hash") {

                    }
                }

                else if (event.text.unicode != '\b') {
                    inputText += static_cast<char>(event.text.unicode);
                }

                // update input text
                input.setText(inputText);
            }
        }


        if (gameStarted) {
            window.clear(sf::Color::White);
            insertTime.draw(window);
            window.draw(searchBox);
            window.draw(guessButton);
            for (int i = 0; i < 5; i++) {
                window.draw(boxes[i]);
            }
            for (int i = 0; i < 7; i++) {
                window.draw(hints[i]);
            }
            for (auto& hintDisplay : hint_displays) {
                hintDisplay.draw(window);
            }
            name.draw(window);
            genre.draw(window);
            platform.draw(window);
            pers.draw(window);
            gamemode.draw(window);
            rating.draw(window);
            year.draw(window);
            red.draw(window);
            yellow.draw(window);
            green.draw(window);
            up.draw(window);
            down.draw(window);
            input.draw(window);
            guessText.draw(window);
            window.display();
        }
        else {
            window.clear(sf::Color::Cyan);
            window.draw(splayBox);
            window.draw(hashBox);
            title.draw(window);
            welcomeText.draw(window);
            splayText.draw(window);
            hashText.draw(window);
            window.display();
        }
    }

    return 0;
}
