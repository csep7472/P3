#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include "splay.h"
#include "api.h"
#include <SFML/Graphics.hpp>
#include <chrono>

#include "GUI.h"

using namespace std;


vector<string> evaluateGuess(unordered_map<string, Game> gameMap, string& userGuess, Game& correctGame) {
    // Vector that stores results for name, platforms, genre, release year, game modes, and perspectives in that order
    vector<string> results;
    // To compare:
    if (gameMap[userGuess].name == correctGame.name) {
        cout << "Name is correct!" << endl;
        results.emplace_back("green");
    }
    else {
        results.emplace_back("red");
    }
    // Need to make this iterate through the vector and determine if results should be red, yellow, or green
    if (gameMap[userGuess].platforms == correctGame.platforms) {
        cout << "Platforms are correct!" << endl;
        results.emplace_back("green");
    }
    else {
        int matches = 0;
        for (const auto &guessPlatform: gameMap[userGuess].platforms) {
            for (const auto &correctPlatform: correctGame.platforms) {
                if (guessPlatform == correctPlatform)
                    matches += 1;
            }
        }
        if (matches > 0) {
            results.emplace_back("yellow");
            cout << "At least one platform matches!" << endl;
        }
        else {
            results.emplace_back("red");
            cout << "No platforms match" << endl;
        }
    }
    // Do the same for genres
    if (gameMap[userGuess].genres == correctGame.genres) {
        cout << "Genres are correct!" << endl;
        results.emplace_back("green");
    }
    else {
        int matches = 0;
        for (const auto &guessGenre: gameMap[userGuess].genres) {
            for (const auto &correctGenre: correctGame.genres) {
                if (guessGenre == correctGenre)
                    matches += 1;
            }
        }
        if (matches > 0) {
            results.emplace_back("yellow");
            cout << "At least one genre matches!" << endl;
        }
        else {
            results.emplace_back("red");
            cout << "No genres match" << endl;
        }
    }
    if (gameMap[userGuess].releaseYear == correctGame.releaseYear) {
        results.emplace_back("equal");
        cout << "The release year is correct!" << endl;
    }
    else if (gameMap[userGuess].releaseYear < correctGame.releaseYear) {
        results.emplace_back("up");
        cout << "The release year of the guessed game is older than the correct game" << endl;
    }
    else {
        results.emplace_back("down");
        cout << "The release year of the guessed game is newer than the correct game" << endl;
    }
    if (gameMap[userGuess].gameModes == correctGame.gameModes) {
        cout << "Game modes are correct!" << endl;
        results.emplace_back("green");
    }
    else {
        int matches = 0;
        for (const auto &guessGameMode: gameMap[userGuess].gameModes) {
            for (const auto &correctGameMode: correctGame.gameModes) {
                if (guessGameMode == correctGameMode)
                    matches += 1;
            }
        }
        if (matches > 0) {
            results.emplace_back("yellow");
            cout << "At least one game mode matches!" << endl;
        }
        else {
            results.emplace_back("red");
            cout << "No game modes match" << endl;
        }
    }
    if (gameMap[userGuess].perspectives == correctGame.perspectives) {
        cout << "Perspectives are correct!" << endl;
        results.emplace_back("green");
    }
    else {
        int matches = 0;
        for (const auto &guessPerspective: gameMap[userGuess].perspectives) {
            for (const auto &correctPerspective: correctGame.perspectives) {
                if (guessPerspective == correctPerspective)
                    matches += 1;
            }
        }
        if (matches > 0) {
            results.emplace_back("yellow");
            cout << "At least one perspective matches!" << endl;
        }
        else {
            results.emplace_back("red");
            cout << "No perspectives match" << endl;
        }
    }
    return results;
}

int main() {
    vector<Game> allGames = getGamesList();
    SplayTree splaytree;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Video Game Wordle");

    // gets window size to dynamically adjust text and button locations
    sf::Vector2u windowSize = window.getSize();

    // creating and setting position for title text
    TextDisplay title("Video Game Wordle", 50,  sf::Vector2f(0, 0), sf::Color::Black);
    sf::FloatRect titleRect = title.getText().getLocalBounds();
    float centerX = (windowSize.x - titleRect.width) / 2;
    title.setPosition(sf::Vector2f(centerX-10, windowSize.y * 0.1f));

    // creating and setting position for Splay Tree and Hash Map text
    TextDisplay splayText("Splay Tree", 30, sf::Vector2f(centerX, windowSize.y * 0.5f), sf::Color::Black);
    TextDisplay hashText("Hash Map", 30, sf::Vector2f(centerX + titleRect.width, windowSize.y * 0.5f), sf::Color::Black);
    sf::FloatRect hashRect = hashText.getText().getLocalBounds();
    hashText.setPosition(sf::Vector2f(centerX + titleRect.width - hashRect.width, windowSize.y * 0.5f));

    TextDisplay welcomeText("Select a Game Mode: ", 40, sf::Vector2f(centerX, windowSize.y * 0.3f), sf::Color::Black);
    sf::FloatRect welcomeTextRect = welcomeText.getText().getLocalBounds();
    float welcomeCenter = (windowSize.x - welcomeTextRect.width) / 2;
    welcomeText.setPosition(sf::Vector2f(welcomeCenter, windowSize.y * 0.3));

    // creates and positions boxes for splay and hash mode
    sf::RectangleShape splayBox(sf::Vector2f(splayText.getText().getLocalBounds().width + 20, 40));
    splayBox.setPosition(centerX - 10, windowSize.y * 0.5f);
    splayBox.setFillColor(sf::Color::Green);
    sf::RectangleShape hashBox(sf::Vector2f(hashText.getText().getLocalBounds().width + 20, 40));
    hashBox.setPosition(centerX + titleRect.width - hashRect.width - 10, windowSize.y * 0.5f);
    hashBox.setFillColor(sf::Color::Blue);


    bool gameStarted = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
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
                    }
                    auto end = chrono::high_resolution_clock::now();
                    time = end - start;
                    gameStarted = true;
                }

                else if (hashBox.getGlobalBounds().contains(mousePosF) && !gameStarted) {
                    auto start = chrono::high_resolution_clock::now();
                    for (const auto& game : allGames) {
                        // insert into hashmap
                    }
                    auto end = chrono::high_resolution_clock::now();
                    time = end - start;
                    gameStarted = true;
                }

                cout << "time to insert all games: " << time.count() << " seconds." << endl;
            }
        }


        if (gameStarted) {
            window.clear(sf::Color::White);
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
