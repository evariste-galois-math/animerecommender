#include <iostream>
#include "Recommender.h"

int main() {

    Recommender rec;

    // Anime 100: users 1, 2, 3
    rec.addWatch(1, 100);
    rec.addWatch(2, 100);
    rec.addWatch(3, 100);

    // Anime 101: users 2, 3, 4
    rec.addWatch(2, 101);
    rec.addWatch(3, 101);
    rec.addWatch(4, 101);

    // Anime 102: users 1, 3
    rec.addWatch(1, 102);
    rec.addWatch(3, 102);

    rec.buildSimilarityMatrix();

    std::cout << "matrix sim(100,101): " << rec.getSimilarity(100, 101) << std::endl;
    std::cout << "matrix sim(101,100): " << rec.getSimilarity(101, 100) << std::endl;
    std::cout << "matrix sim(100,102): " << rec.getSimilarity(100, 102) << std::endl;
    std::cout << "matrix sim(101,102): " << rec.getSimilarity(101, 102) << std::endl;

    return 0;

}
