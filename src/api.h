#ifndef API_H
#define API_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Game class definition
class Game {
public:
    double rating = 0.0;              
    std::string name;
    std::vector<std::string> genres;
    std::vector<std::string> platforms;
    std::vector<std::string> perspectives;
    std::vector<std::string> gameModes;
    int releaseYear = 0;              

    // Nlohmann JSON serialization support
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Game, rating, name, genres, platforms, perspectives, gameModes, releaseYear)
};

// Function declarations
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
std::string joinArray(const nlohmann::json& array, const std::string& key);
std::vector<Game> getGamesList();

#endif
