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
    sf::Font font;
    if (!font.loadFromFile("../resources/MontserratRegular-BWBEl.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    // creating and setting position for title text
    TextDisplay title(font, "Video Game Wordle", 50,  sf::Vector2f(0, 0), sf::Color::Black);
    sf::FloatRect titleRect = title.getText().getGlobalBounds();
    float centerX = (windowSize.x - titleRect.width) / 2;
    title.setPosition(sf::Vector2f(centerX-10, windowSize.y * 0.1f));

    // creating and setting position for Splay Tree and Hash Map text
    TextDisplay splayText(font, "Splay Tree", 30, sf::Vector2f(centerX, windowSize.y * 0.5f), sf::Color::Black);
    TextDisplay hashText(font, "Hash Map", 30, sf::Vector2f(centerX + titleRect.width, windowSize.y * 0.5f), sf::Color::Black);
    sf::FloatRect hashRect = hashText.getText().getGlobalBounds();
    hashText.setPosition(sf::Vector2f(centerX + titleRect.width - hashRect.width, windowSize.y * 0.5f));

    TextDisplay welcomeText(font, "Select a Game Mode: ", 40, sf::Vector2f(centerX, windowSize.y * 0.3f), sf::Color::Black);
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
    TextDisplay insertTime(font, "time to insert all games: ", 20, sf::Vector2f(0, windowSize.y - 30), sf::Color::Black);

    // user input area
    sf::RectangleShape searchBox(sf::Vector2f(600, 30));
    searchBox.setPosition((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2);
    searchBox.setOutlineThickness(3);
    searchBox.setOutlineColor(sf::Color::Black);
    TextDisplay input(font, "Enter Your Guess here", 20, sf::Vector2f(0, 0), sf::Color::Black);
    input.setPosition(sf::Vector2f(searchBox.getPosition().x + 5, windowSize.y * 0.2));
    sf::RectangleShape guessButton(sf::Vector2f(80, 30));
    guessButton.setPosition(searchBox.getPosition().x + 620, windowSize.y * 0.2);
    guessButton.setOutlineThickness(3);
    guessButton.setOutlineColor(sf::Color::Black);
    TextDisplay guessText(font, "Guess!", 20, sf::Vector2f(0, 0), sf::Color::Black);
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
    TextDisplay red(font, "Red: None of the \nclues match.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (0 + 1) + 150 * 0 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay yellow(font, "Yellow: One or \nmore of the clues \nmatch, but not all.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (1 + 1) + 150 * 1 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay green(font, "Green: All clues \nmatch!", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (2 + 1) + 150 * 2 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay up(font, "Arrow Up: The \ngame you have \nguessed is older \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (3 + 1) + 150 * 3 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay down(font, "Arrow down: The \ngame you have \nguessed is newer \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (4 + 1) + 150 * 4 + 5, windowSize.y * 0.05), sf::Color::Black);

    // boxes for guess hints
    vector<sf::RectangleShape> hints(7);
    for (int i = 0; i < 7; ++i) {
        hints[i].setSize(sf::Vector2f(100, 30));
        hints[i].setOutlineThickness(3);
        hints[i].setOutlineColor(sf::Color::Black);
        hints[i].setFillColor(sf::Color::White);
        hints[i].setPosition((windowSize.x - (7 * 80)) / 8.0f * (i + 1) + 80 * i, windowSize.y * 0.25);
    }
    TextDisplay name(font, "Name", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay genre(font, "Genres", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay platform(font, "Platform", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay pers(font, "Perspectives", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay gamemode(font, "Game Modes", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay year(font, "Release Year", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay rating(font, "Ratings", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, windowSize.y * 0.26), sf::Color::Black);

    // game not found
    TextDisplay notFound(font, "Game Not Found", 12, sf::Vector2f(windowSize.x, windowSize.y), sf::Color::Red);

    // game stuff
    bool gameStarted = false;
    string inputText = "";
    string mode = "";
    Game* guessedGame;
    vector<string> names;
    vector<string> genres;
    vector<string> platforms;
    vector<string> perspectives;
    vector<string> gameModes;
    vector<int> years;
    vector<double> ratings;
    vector<TextDisplay> nameDisplays;
    vector<TextDisplay> genreDisplays;
    vector<TextDisplay> platformDisplays;
    vector<TextDisplay> perspectiveDisplays;
    vector<TextDisplay> modeDisplays;
    vector<TextDisplay> yearDisplays;
    vector<TextDisplay> ratingDisplays;

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
                        Game* guessedGame = splaytree.search(inputText);
                        // guessed game is not in database
                        if (guessedGame == nullptr) {
                            notFound.setText("Game Not Found");
                            notFound.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2 + 30));
                        }
                        else {
                            // gussed game found in database
                            if (find(names.begin(), names.end(), inputText) == names.end()) {
                                // after hitting enter, everything in the search box disappears
                                inputText = "";
                                // remove search error from screen
                                notFound.setPosition(sf::Vector2f(windowSize.x, windowSize.y));
                                // append all the hint info into vectors
                                names.push_back(guessedGame->name);
                                genres.push_back(guessedGame->genres);
                                platforms.push_back(guessedGame->platforms);
                                perspectives.push_back(guessedGame->perspectives);
                                gameModes.push_back(guessedGame->gameModes);
                                years.push_back(guessedGame->releaseYear);
                                ratings.push_back(guessedGame->rating);
                                // replace the comma with \n so they print on different lines
                                for (int i = 0; i < names.size(); i++) {
                                    std::replace(names[i].begin(), names[i].end(), ',', '\n');
                                    std::replace(genres[i].begin(), genres[i].end(), ',', '\n');
                                    std::replace(platforms[i].begin(), platforms[i].end(), ',', '\n');
                                    std::replace(perspectives[i].begin(), perspectives[i].end(), ',', '\n');
                                    std::replace(gameModes[i].begin(), gameModes[i].end(), ',', '\n');
                                }
                                // append to vector so that they can be displayed
                                for (size_t i = 0; i < names.size(); i++) {
                                    float firstRowY = windowSize.y * 0.3f;
                                    float verticalSpace = windowSize.y * 0.9f - firstRowY;
                                    float spaceBetweenRows = verticalSpace / 6.0f;
                                    nameDisplays.push_back(TextDisplay(font, names[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    genreDisplays.push_back(TextDisplay(font, genres[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    platformDisplays.push_back(TextDisplay(font, platforms[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    perspectiveDisplays.push_back(TextDisplay(font, perspectives[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    modeDisplays.push_back(TextDisplay(font, gameModes[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    yearDisplays.push_back(TextDisplay(font, to_string(years[i]), 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                    ratingDisplays.push_back(TextDisplay(font, to_string(ratings[i]), 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, firstRowY + (i * spaceBetweenRows)), sf::Color::Black));
                                }
                            }
                            // guessed game found in database but already guessed
                            else {
                                notFound.setText("Already Guessed");
                                notFound.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2 + 30));
                            }
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
            notFound.draw(window);
            for (size_t i = 0; i < nameDisplays.size(); i++) {
                nameDisplays[i].draw(window);
                genreDisplays[i].draw(window);
                platformDisplays[i].draw(window);
                perspectiveDisplays[i].draw(window);
                modeDisplays[i].draw(window);
                yearDisplays[i].draw(window);
                ratingDisplays[i].draw(window);
            }
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
