#include "Server.h"
#include "Recommender.h"
#include "JikanClient.h"
#include "Database.h"
#include <cstdlib>
#include <iostream>

int main() {
    const char* dbUrl = std::getenv("DATABASE_URL");
    if (!dbUrl) {
        std::cerr << "DATABASE_URL environment variable not set." << std::endl;
        return 1;
    }

    Database db(dbUrl);
    JikanClient client;
    Recommender rec;

    auto savedWatches = db.loadAllWatches();
    for (const auto& record : savedWatches) {
        rec.addWatch(record.userId, record.animeId, record.rating);
    }

    rec.buildSimilarityMatrix();

    Server server(rec, client, db);
    server.run(8080);

    return 0;
}