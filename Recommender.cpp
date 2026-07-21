//
// Created by Danny Rios on 7/21/26.
//

#include "Recommender.h"
#include <cmath>
    void Recommender::addWatch(int userId, int animeId) {
        userWatchList[userId].insert(animeId);
        itemToUsers[animeId].insert(userId);
    }
    void Recommender::removeWatch(int userId, int animeId) {
        userWatchList[userId].erase(animeId);
        itemToUsers[animeId].erase(userId);
    }

    const std::unordered_set<int>& Recommender::getWatched(int userId) const {
        auto it = userWatchList.find(userId);

        if (it != userWatchList.end()) {
            return it->second;
        }

        static const std::unordered_set<int> empty;
        return empty;
    }

    double Recommender::cosineSimilarity(int itemI, int itemJ) const {
        //all users who have watched animes' I, J
        auto itI = itemToUsers.find(itemI);
        auto itJ = itemToUsers.find(itemJ);

        if (itI == itemToUsers.end() || itJ == itemToUsers.end()) {
            return 0.0;
        }

        const auto& usersI = itI->second;
        const auto& usersJ = itJ->second;

        int intersectionCount = 0;

        //intersection of users in anime
        if (usersI.size() <= usersJ.size()) {
            for (int userId : usersI) {
                if (usersJ.count(userId)) {
                    intersectionCount++;
                }
            }
        }
        else {
            for (int userId: usersJ) {
                if (usersI.count(userId)) {
                    intersectionCount++;
                }
            }
        }

        //cosine similarity formula
        return intersectionCount /
        (std::sqrt(usersI.size()) *
        std::sqrt(usersJ.size()));
    }