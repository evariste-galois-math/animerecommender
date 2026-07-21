#include <iostream>
#include "Recommender.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    Recommender rec;

    // Anime 100: watched by users 1, 2, 3
    rec.addWatch(1, 100);
    rec.addWatch(2, 100);
    rec.addWatch(3, 100);

    // Anime 101: watched by users 2, 3, 4
    rec.addWatch(2, 101);
    rec.addWatch(3, 101);
    rec.addWatch(4, 101);

    double sim = rec.cosineSimilarity(100, 101);
    std::cout << "Similarity: " << sim << std::endl;

    return 0;
}
