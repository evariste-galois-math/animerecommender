#ifndef ANIME_RECOMMENDER_CPP_JIKANCLIENT_H
#define ANIME_RECOMMENDER_CPP_JIKANCLIENT_H
#pragma once
#include <string>
#include <vector>

struct AnimeInfo {
    int id;
    std::string title;
    double score;
};

class JikanClient {
public:
    std::string fetchAnime(int animeId) const;
    AnimeInfo fetchAnimeParsed(int animeId) const;
    std::vector<AnimeInfo> searchAnime(const std::string& query) const;

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};

#endif //ANIME_RECOMMENDER_CPP_JIKANCLIENT_H