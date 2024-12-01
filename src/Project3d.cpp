#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std::this_thread;
using namespace std::chrono_literals;

// Game class
class Game {
public:
    double rating = 0.0;              
    std::string name;                 
    std::string genres;               
    std::string platforms;            
    std::string perspectives;         
    std::string gameModes;            
    int releaseYear = 0;              

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Game, rating, name, genres, platforms, perspectives, gameModes, releaseYear)
};

// Callback function for libcurl
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* output = static_cast<std::string*>(userdata);
    output->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string joinArray(const nlohmann::json& array, const std::string& key) {
    std::string result;
    if (array.is_array()) {
        for (const auto& item : array) {
            if (item.contains(key)) {
                if (!result.empty()) {
                    result += ", ";
                }
                result += item[key].get<std::string>();
            }
        }
    }
    return result;
}

int main() {
    std::vector<Game> allGames;  
    std::string output;          
    CURL* curl = curl_easy_init();

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Client-ID: fb89fyfzjfsfqmfmre5akrssfnfdyq");
        headers = curl_slist_append(headers, "Authorization: Bearer nddadgrn6sj42d7feurnu84d0cnkih");

        int limit = 10;  
        int offset = 0;   
        bool moreResults = true;

        while (moreResults) {
            output.clear();  
            std::cout << "Loop Started" << std::endl;
            std::string data = "fields name, genres.name, rating, platforms.name, game_modes.name, release_dates.y, player_perspectives.name; limit " + std::to_string(limit) +
                "; offset " + std::to_string(offset) +
                "; where rating > 0 & genres.name != null & platforms.name != null & game_modes.name != null & release_dates.y != null & player_perspectives.name != null; sort rating desc;";

            curl_easy_setopt(curl, CURLOPT_URL, "https://api.igdb.com/v4/games/");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
                break;
            }
            else {
                try {
                    nlohmann::json parsedJson = nlohmann::json::parse(output);

                    if (parsedJson.empty() || parsedJson.size() < limit) {
                        moreResults = false;
                        std::cout << "Json is empty" << std::endl;
                        break;
                    }

                    for (const auto& gameJson : parsedJson) {
                        Game game;

                        if (gameJson.contains("name")) {
                            game.name = gameJson["name"].get<std::string>();
                        }

                        if (gameJson.contains("rating")) {
                            game.rating = gameJson["rating"].get<double>();
                        }

                        if (gameJson.contains("genres")) {
                            game.genres = joinArray(gameJson["genres"], "name");
                        }

                        if (gameJson.contains("platforms")) {
                            game.platforms = joinArray(gameJson["platforms"], "name");
                        }

                        if (gameJson.contains("game_modes")) {
                            game.gameModes = joinArray(gameJson["game_modes"], "name");
                        }

                        if (gameJson.contains("player_perspectives")) {
                            game.perspectives = joinArray(gameJson["player_perspectives"], "name");
                        }

                        if (gameJson.contains("release_dates") && !gameJson["release_dates"].empty()) {
                            for (int i = 0; i < gameJson["release_dates"].size(); i++) {
                                if (gameJson["release_dates"][i].contains("y")) {
                                    game.releaseYear = gameJson["release_dates"][i]["y"].get<int>();
                                    break;
                                }
                            }
                        }
                        allGames.push_back(game);
                    }
                    offset += limit;

                }
                catch (const nlohmann::json::parse_error& e) {
                    std::cerr << "JSON parsing error: " << e.what() << std::endl;
                    break;
                }
            }
            if (offset >= 30) {
                moreResults = false;
            }
            std::cout << "Loop Ended: " << allGames.size() << std::endl;
            sleep_for(1.5s);
        }

        curl_easy_cleanup(curl);

        std::cout << "Total Games Retrieved: " << allGames.size() << std::endl;
        std::cout << "All Games Retrieved:" << std::endl;
        for (const auto& game : allGames) {
            std::cout << "Game Name: " << game.name << std::endl;
            std::cout << "Genres: " << game.genres << std::endl;
            std::cout << "Rating: " << game.rating << std::endl;
            std::cout << "Game Modes: " << game.gameModes << std::endl;
            std::cout << "Perspectives: " << game.perspectives << std::endl;
            std::cout << "Release Year: " << game.releaseYear << std::endl;
            std::cout << "Platforms: " << game.platforms << std::endl;
            std::cout << "------------------------" << std::endl;
        }
    }

    return 0;
}
