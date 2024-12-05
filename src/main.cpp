#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include "splay.h"
#include "api.h"
#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include "hashmap.h"
#include <time.h>

using namespace std;


int main() {
    vector<Game> allGames = getGamesList();
    for (auto game : allGames) {
        cout << game.name << endl;
    }
    srand(time(nullptr));
    int randomIndex = rand() % allGames.size();
    Game correctGame = allGames[randomIndex];
    cout << correctGame.name << endl;
    SplayTree splaytree;
    HashMap<string, Game> gameMap(100);  // Create a HashMap with 100 buckets
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Video Game Wordle", sf::Style::Fullscreen);

    // gets window size to dynamically adjust text and button locations
    sf::Vector2u windowSize = window.getSize();
    sf::Font font;
    if (!font.loadFromFile("../resources/MontserratRegular-BWBEl.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    sf::Font Boldfont;
    if (!Boldfont.loadFromFile("../resources/MontserratBold-DOWZd.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    // creating and setting position for title text
    TextDisplay title(Boldfont, "Video Game Wordle", 50,  sf::Vector2f(0, 0), sf::Color::Cyan);
    sf::FloatRect titleRect = title.getText().getGlobalBounds();
    float centerX = (windowSize.x - titleRect.width) / 2;
    title.setPosition(sf::Vector2f(centerX-10, windowSize.y * 0.2f));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3);

    // Select a gamemode text
    TextDisplay welcomeText(Boldfont, "Select a Game Mode: ", 40, sf::Vector2f(centerX, windowSize.y * 0.4f), sf::Color(200, 200, 200));
    sf::FloatRect welcomeTextRect = welcomeText.getText().getGlobalBounds();
    float welcomeCenter = (windowSize.x - welcomeTextRect.width) / 2;
    welcomeText.setPosition(sf::Vector2f(welcomeCenter, windowSize.y * 0.4));
    welcomeText.setOutlineColor(sf::Color::Black);
    welcomeText.setOutlineThickness(2);

    // creating and setting position for Splay Tree and Hash Map text
    TextDisplay splayText(Boldfont, "Splay Tree", 30, sf::Vector2f(centerX, windowSize.y * 0.6f), sf::Color(211, 211, 211));
    splayText.setOutlineColor(sf::Color::Black);
    splayText.setOutlineThickness(2);
    TextDisplay hashText(Boldfont, "Hash Map", 30, sf::Vector2f(centerX + titleRect.width, windowSize.y * 0.6f), sf::Color(211, 211, 211));
    hashText.setOutlineColor(sf::Color::Black);
    hashText.setOutlineThickness(2);
    sf::FloatRect hashRect = hashText.getText().getGlobalBounds();
    hashText.setPosition(sf::Vector2f(centerX + titleRect.width - hashRect.width, windowSize.y * 0.6f));

    // creates and positions boxes for splay and hash mode
    sf::RectangleShape splayBox(sf::Vector2f(splayText.getText().getGlobalBounds().width + 20, 40));
    splayBox.setPosition(centerX - 10, windowSize.y * 0.6f);
    splayBox.setFillColor(sf::Color(204, 102, 51));
    splayBox.setOutlineColor(sf::Color::Black);
    splayBox.setOutlineThickness(3);
    sf::RectangleShape hashBox(sf::Vector2f(hashText.getText().getGlobalBounds().width + 20, 40));
    hashBox.setPosition(centerX + titleRect.width - hashRect.width - 10, windowSize.y * 0.6f);
    hashBox.setFillColor(sf::Color(68, 123, 135));
    hashBox.setOutlineColor(sf::Color::Black);
    hashBox.setOutlineThickness(3);

    // create and position time it takes to insert
    TextDisplay insertTime(font, "time to insert all games: ", 20, sf::Vector2f(0, windowSize.y - 30), sf::Color::Black);

    // user input area
    sf::RectangleShape searchBox(sf::Vector2f(600, 30));
    searchBox.setPosition((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2);
    searchBox.setOutlineThickness(3);
    searchBox.setOutlineColor(sf::Color::Black);
    TextDisplay input(Boldfont, "Enter Your Guess Here!", 20, sf::Vector2f(0, 0), sf::Color::Black);
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
        boxes[i].setFillColor(sf::Color(230, 230, 250));
        boxes[i].setPosition((windowSize.x - (5 * 150)) / 6.0f * (i + 1) + 150 * i, windowSize.y * 0.05);
    }
    TextDisplay red(font, "Red: None of the \nclues match.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (0 + 1) + 150 * 0 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay yellow(font, "Yellow: One or \nmore of the clues \nmatch, but not all.", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (1 + 1) + 150 * 1 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay green(font, "Green: All clues \nmatch!", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (2 + 1) + 150 * 2 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay up(font, "Plus Sign: The \ngame you have \nguessed is older \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (3 + 1) + 150 * 3 + 5, windowSize.y * 0.05), sf::Color::Black);
    TextDisplay down(font, "Minus Sign: The \ngame you have \nguessed is newer \nthan the correct \ngame", 15,sf::Vector2f((windowSize.x - (5 * 150)) / 6.0f * (4 + 1) + 150 * 4 + 5, windowSize.y * 0.05), sf::Color::Black);

    // boxes for guess hints
    vector<sf::RectangleShape> hints(7);
    for (int i = 0; i < 7; ++i) {
        hints[i].setSize(sf::Vector2f(100, 30));
        hints[i].setOutlineThickness(3);
        hints[i].setOutlineColor(sf::Color::Black);
        hints[i].setFillColor(sf::Color(230, 230, 250));
        hints[i].setPosition((windowSize.x - (7 * 80)) / 8.0f * (i + 1) + 80 * i, windowSize.y * 0.25);
    }
    TextDisplay name(Boldfont, "Name", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay genre(Boldfont, "Genres", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay platform(Boldfont, "Platform", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay pers(Boldfont, "Perspectives", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay gamemode(Boldfont, "Game Modes", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay year(Boldfont, "Release Year", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, windowSize.y * 0.26), sf::Color::Black);
    TextDisplay rating(Boldfont, "Ratings", 12,sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, windowSize.y * 0.26), sf::Color::Black);

    // game not found
    TextDisplay notFound(font, "Game Not Found", 12, sf::Vector2f(windowSize.x, windowSize.y), sf::Color::Red);

    // Correct game is
    TextDisplay correct(font, "Correct Answer: " + correctGame.name, 12, sf::Vector2f(windowSize.x, windowSize.y), sf::Color::Black);

    // restart button
    sf::RectangleShape restartButton(sf::Vector2f(200, 50));
    restartButton.setPosition(windowSize.x - 100, 0);
    restartButton.setFillColor(sf::Color(230, 230, 250));
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setOutlineThickness(3);
    TextDisplay restart(Boldfont, "Restart", 20, sf::Vector2f(windowSize.x - 90, 8), sf::Color(45, 45, 45));

    // title screen background
    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("../resources/TitleBackground.jpg")) {
        cout << "Background not loaded" << endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
                              static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    // game background
    // title screen background
    sf::Texture gameTexture;
    if(!gameTexture.loadFromFile("../resources/gameBackground.jpg")) {
        cout << "Background not loaded" << endl;
    }
    sf::Sprite gameSprite(gameTexture);
    gameSprite.setPosition(0, 0);

    // sounds
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("../resources/backgroundMusic.wav")) {
        cerr << "Can't load audio" << endl;
    }
    sf::Sound backgroundMusic;
    backgroundMusic.setBuffer(buffer);
    backgroundMusic.setVolume(25.f);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // game stuff
    bool gameStarted = false;
    bool found = false;
    string inputText = "";
    string mode = "";
    chrono::duration<double> timeInsertSplay;
    chrono::duration<double> timeInsertHash;
    chrono::duration<double> timeSearchSplay;
    chrono::duration<double> timeSearchHash;
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
    vector<pair<string, sf::Color>> nameColor;
    vector<pair<string, sf::Color>> genreColor;
    vector<pair<string, sf::Color>> platformColor;
    vector<pair<string, sf::Color>> perspectiveColor;
    vector<pair<string, sf::Color>> modeColor;
    vector<pair<string, sf::Color>> yearColor;
    vector<pair<string, sf::Color>> ratingColor;

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
                if (splayBox.getGlobalBounds().contains(mousePosF) && !gameStarted) {
                    auto startSplayInsert = chrono::high_resolution_clock::now();
                    for (const auto& game : allGames) {
                        splaytree.insert(game);
                    }
                    auto endSplayInsert = chrono::high_resolution_clock::now();
                    timeInsertSplay = endSplayInsert - startSplayInsert;
                    gameStarted = true;
                    mode = "splay";
                    insertTime.setText("time to insert all games: " + to_string(timeInsertSplay.count()) + "s");
                }

                else if (hashBox.getGlobalBounds().contains(mousePosF) && !gameStarted) {
                    auto startInsertHash = chrono::high_resolution_clock::now();
                    for (const auto& game : allGames) {
                        gameMap[game.name] = game;  // Insert into custom HashMap
                    }
                    auto endInsertHash = chrono::high_resolution_clock::now();
                    timeInsertHash = endInsertHash - startInsertHash;
                    gameStarted = true;
                    mode = "hash";
                    insertTime.setText("time to insert all games: " + to_string(timeInsertHash.count()) + "s");
                }

                else if (guessButton.getGlobalBounds().contains(mousePosF) && gameStarted) {
                    // add the same thing from under else if (event.text.unicode == '\r' && inputText.size() > 0)
                }

                else if (restartButton.getGlobalBounds().contains(mousePosF) && gameStarted) {
                    found = false;
                    inputText = "Enter Your Guess Here!";
                    input.setText(inputText);

                    names.clear();
                    genres.clear();
                    platforms.clear();
                    perspectives.clear();
                    gameModes.clear();
                    years.clear();
                    ratings.clear();

                    nameDisplays.clear();
                    genreDisplays.clear();
                    platformDisplays.clear();
                    perspectiveDisplays.clear();
                    modeDisplays.clear();
                    yearDisplays.clear();
                    ratingDisplays.clear();

                    nameColor.clear();
                    genreColor.clear();
                    platformColor.clear();
                    perspectiveColor.clear();
                    modeColor.clear();
                    yearColor.clear();
                    ratingColor.clear();

                    correctGame = allGames[rand() % allGames.size()];
                    notFound.setText("");
                    cout << correctGame.name << endl;
                    notFound.setPosition(sf::Vector2f(windowSize.x, windowSize.y));
                    correct.setPosition(sf::Vector2f(windowSize.x, windowSize.y));
                }
            }

            if (event.type == sf::Event::TextEntered && gameStarted) {
                if (event.text.unicode == '\b' && inputText.size() > 0) {
                    inputText.erase(inputText.size()-1, 1);
                }

                else if (event.text.unicode == '\r' && inputText.size() > 0 && !found) {
                    if (mode == "splay") {
                        auto startSearchSplay = chrono::high_resolution_clock::now();
                        Game* guessedGame = splaytree.search(inputText);
                        auto endSearchSplay = chrono::high_resolution_clock::now();
                        timeSearchSplay = endSearchSplay - startSearchSplay;
                        insertTime.setText("time to search game: " + to_string(timeSearchSplay.count()) + "s");
                        // guessed game is not in database
                        if (guessedGame == nullptr) {
                            notFound.setText("Game Not Found");
                            notFound.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2 + 30));
                        }
                        else {
                            // gussed game found in database... checking if it has been guessed already
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
                                    for (size_t pos = 0; (pos = names[i].find(", ", pos)) != string::npos; pos += 1) {
                                        names[i].replace(pos, 2, "\n");
                                    }
                                    for (size_t pos = 0; (pos = genres[i].find(", ", pos)) != string::npos; pos += 1) {
                                        genres[i].replace(pos, 2, "\n");
                                    }
                                    for (size_t pos = 0; (pos = platforms[i].find(", ", pos)) != string::npos; pos += 1) {
                                        platforms[i].replace(pos, 2, "\n");
                                    }
                                    for (size_t pos = 0; (pos = perspectives[i].find(", ", pos)) != string::npos; pos += 1) {
                                        perspectives[i].replace(pos, 2, "\n");
                                    }
                                    for (size_t pos = 0; (pos = gameModes[i].find(", ", pos)) != string::npos; pos += 1) {
                                        gameModes[i].replace(pos, 2, "\n");
                                    }
                                }
                                sf::Color displayColor;

                                // Compare guessed and correct answers
                                // name comparison
                                if (guessedGame->name == correctGame.name) {
                                    displayColor = sf::Color::Green;
                                }
                                else if (guessedGame->name.find(correctGame.name) != string::npos) {
                                    displayColor = sf::Color::Yellow;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                nameColor.push_back({guessedGame->name, displayColor});

                                // genre comparison
                                if (guessedGame->genres == correctGame.genres) {
                                    displayColor = sf::Color::Green;
                                }
                                else if (guessedGame->genres.find(correctGame.genres) != string::npos) {
                                    displayColor = sf::Color::Yellow;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                genreColor.push_back({guessedGame->genres, displayColor});

                                // platform comparison
                                if (guessedGame->platforms == correctGame.platforms) {
                                    displayColor = sf::Color::Green;
                                }
                                else if (guessedGame->platforms.find(correctGame.platforms) != string::npos) {
                                    displayColor = sf::Color::Yellow;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                platformColor.push_back({guessedGame->platforms, displayColor});

                                // perspective comparison
                                if (guessedGame->perspectives == correctGame.perspectives) {
                                    displayColor = sf::Color::Green;
                                }
                                else if (guessedGame->perspectives.find(correctGame.perspectives) != string::npos) {
                                    displayColor = sf::Color::Yellow;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                perspectiveColor.push_back({guessedGame->perspectives, displayColor});

                                // gamemode comparison
                                if (guessedGame->gameModes == correctGame.gameModes) {
                                    displayColor = sf::Color::Green;
                                }
                                else if (guessedGame->gameModes.find(correctGame.gameModes) != string::npos) {
                                    displayColor = sf::Color::Yellow;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                modeColor.push_back({guessedGame->gameModes, displayColor});

                                // year comparison
                                if (guessedGame->releaseYear == correctGame.releaseYear) {
                                    displayColor = sf::Color::Green;
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                yearColor.push_back({std::to_string(guessedGame->releaseYear), displayColor});

                                // Compare and assign colors for rating
                                if (guessedGame->rating == correctGame.rating) {
                                    displayColor = sf::Color::Green;  // Exact match
                                }
                                else {
                                    displayColor = sf::Color::Red;
                                }
                                ratingColor.push_back({std::to_string(guessedGame->rating), displayColor});
                                // append to vector so that they can be displayed
                                for (size_t i = 0; i < names.size(); i++) {
                                    float firstRowY = windowSize.y * 0.3f;
                                    float verticalSpace = windowSize.y * 0.9f - firstRowY;
                                    float spaceBetweenRows = verticalSpace / 6.0f;
                                    nameDisplays.push_back(TextDisplay(Boldfont, names[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, firstRowY + (i * spaceBetweenRows)), nameColor[i].second));
                                    genreDisplays.push_back(TextDisplay(Boldfont, genres[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, firstRowY + (i * spaceBetweenRows)), genreColor[i].second));
                                    platformDisplays.push_back(TextDisplay(Boldfont, platforms[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, firstRowY + (i * spaceBetweenRows)), platformColor[i].second));
                                    perspectiveDisplays.push_back(TextDisplay(Boldfont, perspectives[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, firstRowY + (i * spaceBetweenRows)), perspectiveColor[i].second));
                                    modeDisplays.push_back(TextDisplay(Boldfont, gameModes[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, firstRowY + (i * spaceBetweenRows)), modeColor[i].second));

                                    string yearStr = to_string(years[i]);
                                    if (years[i] > correctGame.releaseYear) {
                                        yearStr += " -";
                                    }
                                    else if (years[i] < correctGame.releaseYear) {
                                        yearStr += " +";
                                    }
                                    yearDisplays.push_back(TextDisplay(font, yearStr, 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, firstRowY + (i * spaceBetweenRows)), yearColor[i].second));
                                    string ratingStr = to_string(ratings[i]);
                                    if (ratings[i] > correctGame.rating) {
                                        ratingStr += " -";
                                    }
                                    else if (ratings[i] < correctGame.rating) {
                                        ratingStr += " +";
                                    }
                                    ratingDisplays.push_back(TextDisplay(font, ratingStr, 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, firstRowY + (i * spaceBetweenRows)), ratingColor[i].second));
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
                        auto startSearchHash = chrono::high_resolution_clock::now();

                        // Check if the game exists in the hashmap
                        if (!gameMap.contains(inputText)) {
                            auto endSearchHash = chrono::high_resolution_clock::now();
                            timeSearchHash = endSearchHash - startSearchHash;

                            insertTime.setText("time to search game: " + to_string(timeSearchHash.count()) + "s");
                            notFound.setText("Game Not Found");
                            notFound.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2 + 30));
                        }
                        else {
                            auto endSearchHash = chrono::high_resolution_clock::now();
                            timeSearchHash = endSearchHash - startSearchHash;

                            insertTime.setText("time to search game: " + to_string(timeSearchHash.count()) + "s");

                            // Retrieve the guessed game
                            Game* guessedGame = &gameMap[inputText];

                            // Check if the guessed game matches the correct game
                            if (guessedGame->name == correctGame.name) {
                                found == true;
                                break;
                            }
                            else {
                                // Guessed game is in the database, now check if it has been guessed already
                                if (find(names.begin(), names.end(), inputText) == names.end()) {
                                    // Game not guessed before; proceed to process it
                                    inputText = ""; // Clear the search box
                                    notFound.setPosition(sf::Vector2f(windowSize.x, windowSize.y)); // Remove search error from screen

                                    // Append the game information into vectors
                                    names.push_back(guessedGame->name);
                                    genres.push_back(guessedGame->genres);
                                    platforms.push_back(guessedGame->platforms);
                                    perspectives.push_back(guessedGame->perspectives);
                                    gameModes.push_back(guessedGame->gameModes);
                                    years.push_back(guessedGame->releaseYear);
                                    ratings.push_back(guessedGame->rating);

                                    // Replace ", " with "\n" in game attributes for display
                                    for (size_t i = 0; i < names.size(); i++) {
                                        for (size_t pos = 0; (pos = names[i].find(", ", pos)) != string::npos; pos += 1) {
                                            names[i].replace(pos, 2, "\n");
                                        }
                                        for (size_t pos = 0; (pos = genres[i].find(", ", pos)) != string::npos; pos += 1) {
                                            genres[i].replace(pos, 2, "\n");
                                        }
                                        for (size_t pos = 0; (pos = platforms[i].find(", ", pos)) != string::npos; pos += 1) {
                                            platforms[i].replace(pos, 2, "\n");
                                        }
                                        for (size_t pos = 0; (pos = perspectives[i].find(", ", pos)) != string::npos; pos += 1) {
                                            perspectives[i].replace(pos, 2, "\n");
                                        }
                                        for (size_t pos = 0; (pos = gameModes[i].find(", ", pos)) != string::npos; pos += 1) {
                                            gameModes[i].replace(pos, 2, "\n");
                                        }
                                    }

                                    sf::Color displayColor;

                                    // Compare guessed and correct answers, and assign colors accordingly
                                    // Name comparison
                                    displayColor = (guessedGame->name == correctGame.name)
                                                   ? sf::Color::Green
                                                   : (guessedGame->name.find(correctGame.name) != string::npos)
                                                     ? sf::Color::Yellow
                                                     : sf::Color::Red;
                                    nameColor.push_back({guessedGame->name, displayColor});

                                    // Genre comparison
                                    displayColor = (guessedGame->genres == correctGame.genres)
                                                   ? sf::Color::Green
                                                   : (guessedGame->genres.find(correctGame.genres) != string::npos)
                                                     ? sf::Color::Yellow
                                                     : sf::Color::Red;
                                    genreColor.push_back({guessedGame->genres, displayColor});

                                    // Platform comparison
                                    displayColor = (guessedGame->platforms == correctGame.platforms)
                                                   ? sf::Color::Green
                                                   : (guessedGame->platforms.find(correctGame.platforms) != string::npos)
                                                     ? sf::Color::Yellow
                                                     : sf::Color::Red;
                                    platformColor.push_back({guessedGame->platforms, displayColor});

                                    // Perspective comparison
                                    displayColor = (guessedGame->perspectives == correctGame.perspectives)
                                                   ? sf::Color::Green
                                                   : (guessedGame->perspectives.find(correctGame.perspectives) != string::npos)
                                                     ? sf::Color::Yellow
                                                     : sf::Color::Red;
                                    perspectiveColor.push_back({guessedGame->perspectives, displayColor});

                                    // Game mode comparison
                                    displayColor = (guessedGame->gameModes == correctGame.gameModes)
                                                   ? sf::Color::Green
                                                   : (guessedGame->gameModes.find(correctGame.gameModes) != string::npos)
                                                     ? sf::Color::Yellow
                                                     : sf::Color::Red;
                                    modeColor.push_back({guessedGame->gameModes, displayColor});

                                    // Year comparison
                                    displayColor = (guessedGame->releaseYear == correctGame.releaseYear) ? sf::Color::Green : sf::Color::Red;
                                    yearColor.push_back({std::to_string(guessedGame->releaseYear), displayColor});

                                    // Rating comparison
                                    displayColor = (guessedGame->rating == correctGame.rating) ? sf::Color::Green : sf::Color::Red;
                                    ratingColor.push_back({std::to_string(guessedGame->rating), displayColor});

                                    // Append text displays for game information
                                    for (size_t i = 0; i < names.size(); i++) {
                                        float firstRowY = windowSize.y * 0.3f;
                                        float verticalSpace = windowSize.y * 0.9f - firstRowY;
                                        float spaceBetweenRows = verticalSpace / 6.0f;

                                        nameDisplays.push_back(TextDisplay(font, names[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (0 + 1) + 80 * 0 + 5, firstRowY + (i * spaceBetweenRows)), nameColor[i].second));
                                        genreDisplays.push_back(TextDisplay(font, genres[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (1 + 1) + 80 * 1 + 5, firstRowY + (i * spaceBetweenRows)), genreColor[i].second));
                                        platformDisplays.push_back(TextDisplay(font, platforms[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (2 + 1) + 80 * 2 + 5, firstRowY + (i * spaceBetweenRows)), platformColor[i].second));
                                        perspectiveDisplays.push_back(TextDisplay(font, perspectives[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (3 + 1) + 80 * 3 + 5, firstRowY + (i * spaceBetweenRows)), perspectiveColor[i].second));
                                        modeDisplays.push_back(TextDisplay(font, gameModes[i], 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (4 + 1) + 80 * 4 + 5, firstRowY + (i * spaceBetweenRows)), modeColor[i].second));

                                        string yearStr = to_string(years[i]);
                                        if (years[i] > correctGame.releaseYear) yearStr += " -";
                                        else if (years[i] < correctGame.releaseYear) yearStr += " +";
                                        yearDisplays.push_back(TextDisplay(font, yearStr, 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (5 + 1) + 80 * 5 + 5, firstRowY + (i * spaceBetweenRows)), yearColor[i].second));

                                        string ratingStr = to_string(ratings[i]);
                                        if (ratings[i] > correctGame.rating) ratingStr += " -";
                                        else if (ratings[i] < correctGame.rating) ratingStr += " +";
                                        ratingDisplays.push_back(TextDisplay(font, ratingStr, 12, sf::Vector2f((windowSize.x - (7 * 80)) / 8.0f * (6 + 1) + 80 * 6 + 5, firstRowY + (i * spaceBetweenRows)), ratingColor[i].second));
                                    }
                                }
                                else {
                                    // Guessed game is already guessed
                                    notFound.setText("Already Guessed");
                                    notFound.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.2 + 30));
                                }
                            }
                        }
                    }
                }

                else if (event.text.unicode != '\b') {
                    inputText += static_cast<char>(event.text.unicode);
                }

                // update input text
                input.setText(inputText);
            }

            if (names.size() >= 7) {
                correct.setPosition(sf::Vector2f((windowSize.x - searchBox.getSize().x) / 2, windowSize.y * 0.18));
            }
        }


        if (gameStarted) {
            window.clear(sf::Color(18, 18, 18));
            window.draw(gameSprite);
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
            correct.draw(window);
            window.draw(restartButton);
            restart.draw(window);
            window.display();
        }
        else {
            window.clear();
            window.draw(backgroundSprite);
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