#include <iostream>
#include "Recommender.h"
#include "JikanClient.h"

int main() {
    JikanClient client;
    std::string result = client.fetchAnime(1);
    std::cout << result << std::endl;
    return 0;
}
