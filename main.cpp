#include <iostream>
#include "Recommender.h"

int main() {
    Recommender rec;

    // Anime 100: rated by users 1, 2, 3
    rec.addWatch(1, 100, 5.0);
    rec.addWatch(2, 100, 4.0);
    rec.addWatch(3, 100, 3.0);

    // Anime 101: rated by users 2, 3, 4
    rec.addWatch(2, 101, 5.0);
    rec.addWatch(3, 101, 4.0);
    rec.addWatch(4, 101, 5.0);

    // Anime 102: rated by users 1, 3
    rec.addWatch(1, 102, 4.0);
    rec.addWatch(3, 102, 5.0);

    rec.buildSimilarityMatrix();

    // Predict how much user 1 would like anime 101 (unwatched by user 1)
    double score = rec.predictScore(1, 101);
    std::cout << "Predicted score for user 1 on anime 101: " << score << std::endl;

    return 0;
}
