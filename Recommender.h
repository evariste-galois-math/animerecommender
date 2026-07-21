//
// Created by Danny Rios on 7/21/26.
//

#ifndef ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#define ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#pragma once
#include <unordered_map>
#include <unordered_set>

class Recommender {
private:
    std::unordered_map<int, std::unordered_set<int>> userWatchList;
    std::unordered_map<int, std::unordered_set<int>> itemToUsers;
public:
    void addWatch(int userId, int animeId);
    void removeWatch(int userId, int animeId);
    const std::unordered_set<int>& getWatched(int userId) const;
    double cosineSimilarity(int ItemI, int itemJ) const;
};


#endif //ANIME_RECOMMENDER_CPP_RECOMMENDER_H