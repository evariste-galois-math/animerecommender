

#ifndef ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#define ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#pragma once
#include <unordered_map>
#include <unordered_set>

class Recommender {
private:
    std::unordered_map<int, std::unordered_set<int>> userWatchList;
    std::unordered_map<int, std::unordered_set<int>> itemToUsers;
    std::unordered_map<int, std::unordered_map<int, double>> similarityMatrix;
public:
    void addWatch(int userId, int animeId);
    void removeWatch(int userId, int animeId);
    const std::unordered_set<int>& getWatched(int userId) const;

    double cosineSimilarity(int ItemI, int itemJ) const;
    void buildSimilarityMatrix();
    double getSimilarity(int itemI, int itemJ) const;
};


#endif //ANIME_RECOMMENDER_CPP_RECOMMENDER_H