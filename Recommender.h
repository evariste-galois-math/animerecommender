

#ifndef ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#define ANIME_RECOMMENDER_CPP_RECOMMENDER_H
#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Recommender {
private:
    std::unordered_map<int, std::unordered_map<int, double>> userWatchList;
    std::unordered_map<int, std::unordered_set<int>> itemToUsers;
    std::unordered_map<int, std::unordered_map<int, double>> similarityMatrix;

public:
    void addWatch(int userId, int animeId, double rating);
    void removeWatch(int userId, int animeId);
    const std::unordered_map<int, double>& getWatched(int userId) const;

    double cosineSimilarity(int ItemI, int itemJ) const;
    void buildSimilarityMatrix();
    double getSimilarity(int itemI, int itemJ) const;

    double predictScore(int userId, int targetItem) const;
    std::vector<std::pair<int,double>> recommendTopN(int userId, int n) const;
};


#endif //ANIME_RECOMMENDER_CPP_RECOMMENDER_H