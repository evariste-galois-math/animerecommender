#include "Server.h"
#include "Recommender.h"
#include "JikanClient.h"
#include "Database.h"

int main() {
        Database db("anime.db");
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