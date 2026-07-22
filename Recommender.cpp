

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
        //all users who have watched both I, J
        auto itI = itemToUsers.find(itemI);
        auto itJ = itemToUsers.find(itemJ);

        if (itI == itemToUsers.end() || itJ == itemToUsers.end()) {
            return 0.0;
        }

        const auto& usersI = itI->second;
        const auto& usersJ = itJ->second;

        int intersectionCount = 0;

        //Intersection of users in anime
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

    void Recommender::buildSimilarityMatrix() {
        for (const auto& PairI : itemToUsers) {
            int itemI = PairI.first;

            for (const auto& PairJ : itemToUsers) {
                int itemJ = PairJ.first;

                //Skip a pairing if it's either the same item twice
                //Or a pair you've already handled in the opposite order
                if (itemJ <= itemI) {
                    continue;
                }

                //used that opposite ordering here
                double sim = cosineSimilarity(itemI, itemJ);
                similarityMatrix[itemI][itemJ] = sim;
                similarityMatrix[itemJ][itemI] = sim;
            }
        }
    }

    double Recommender::getSimilarity(int itemI, int itemJ) const {
        auto itI = similarityMatrix.find(itemI);
        if (itI == similarityMatrix.end()) {
            return 0.0;
        }

        auto itJ = itI->second.find(itemJ);
        if (itJ == itI->second.end()) {
            return 0.0;
        }

        return itJ->second;

        //We get two items. We find the similarity on the first item.
        //If that is nothing, then we return 0. Otherwise, we use the similarity score of the second and pass the value itemJ.
        //That would let us access the sim value. If that value is not there, we return 0. Otherwise, we return that similarity value.
    }


