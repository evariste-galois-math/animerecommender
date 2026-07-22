

#include "Recommender.h"
#include <algorithm>
#include <cmath>

    void Recommender::addWatch(int userId, int animeId, double rating) {
        userWatchList[userId][animeId] = rating;
        itemToUsers[animeId].insert(userId);
    }

    void Recommender::removeWatch(int userId, int animeId) {
        userWatchList[userId].erase(animeId);
        itemToUsers[animeId].erase(userId);
    }

    const std::unordered_map<int, double>& Recommender::getWatched(int userId) const {

        auto it = userWatchList.find(userId);

        if (it != userWatchList.end()) {
            return it->second;
        }

        static const std::unordered_map<int,double> empty;
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

    double Recommender::predictScore(int userId, int targetItem) const {
        const auto& watched = getWatched(userId);

        if (watched.empty()) {
            return 0.0;
        }

        double numerator = 0.0;
        double denominator = 0.0;

        for (const auto& pair: watched) {
            numerator += getSimilarity(targetItem, pair.first) * pair.second;
            denominator += std::abs(getSimilarity(targetItem, pair.first));
        }
        if (denominator == 0.0) {
            return 0.0;
        }
        return numerator / denominator;
        // userId is already handled via getWatched(userId), so each pair here
        // is (animeId, rating) for that one user's watch list
    }

    std::vector<std::pair<int, double>> Recommender::recommendTopN(int userId, int n) const { //we pass the userId and n
        std::vector<std::pair<int, double>> candidates; //we also set up an array as a return type and to store all candidates
        const auto& watched = getWatched(userId); //we get a list of all watched animes

        for (const auto& itemPair : itemToUsers) { //iterate through list and store animeId
            int animeId = itemPair.first;

            if (watched.find(animeId) != watched.end()) {
                continue; //already watched, skip
            }

            double score = predictScore(userId, animeId);//score is from using userid and animeId to get the predicted score of anime we haven't seen.
            candidates.push_back({animeId, score}); //we push the anime and its score into the vector
        }

        std::sort(candidates.begin(), candidates.end(),
            [](const std::pair<int, double>& a, const std::pair<int,double>& b) {
                return a.second > b.second;
            }); //this is the lambda function used to sort it. it will return it in descending order

        int actualN = std::min(n, (int)candidates.size()); // we end up deciding between the input of n or the actual number of n that we computed
        return std::vector<std::pair<int, double>>(candidates.begin(), candidates.begin() + actualN); //we return a slice of candidates. The first actualN of them.
    }


