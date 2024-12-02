#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include "splay.h"
#include "api.h"

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
    SplayTree tree;
    for (const auto& game : allGames) {
        tree.insert(game);
    }
    cout << allGames.size() << endl;
    // Rough Outline
    // Load video game data api into an unordered map
    // set up trie
    // receive user's guess
    // evaluate guess
    // display output on interface
    // compare results using an unordered map and another data structure. A trie will be used in both.
    return 0;
}
