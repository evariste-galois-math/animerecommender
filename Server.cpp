#include "Server.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Server::Server(Recommender& rec, JikanClient& jikan, Database& db)
    : recommender(rec), jikanClient(jikan), database(db) {

}

void Server::run(int port) {
    httplib::Server svr;

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    svr.Options(".*", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 204;
    });

    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        std::string query = req.get_param_value("q");
        auto results = jikanClient.searchAnime(query);

        json responseJson = json::array();
        for (const auto& anime : results) {
            responseJson.push_back({
                {"id", anime.id},
                {"title", anime.title},
                {"score", anime.score}
            });
        }

        res.set_content(responseJson.dump(), "application/json");
    });

    svr.Post("/watch", [&](const httplib::Request& req, httplib::Response& res) {
        json body = json::parse(req.body, nullptr, false);

        if (body.is_discarded() || !body.contains("userId") || !body.contains("animeId") || !body.contains("rating")) {
            res.status = 400;
            res.set_content("{\"error\":\"missing fields\"}", "application/json");
            return;
        }

        int userId = body["userId"];
        int animeId = body["animeId"];
        double rating = body["rating"];

        recommender.addWatch(userId, animeId, rating);
        database.saveWatch(userId, animeId, rating);

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    svr.Get(R"(/recommendations/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int userId = std::stoi(req.matches[1]);
        auto recs = recommender.recommendTopN(userId, 5);

        json responseJson = json::array();
        for (const auto& recPair : recs) {
            int animeId = recPair.first;
            double predictedScore = recPair.second;

            AnimeInfo info = jikanClient.fetchAnimeParsed(animeId);

            responseJson.push_back({
                {"id", animeId},
                {"title", info.title},
                {"predictedScore", predictedScore}
            });
        }

        res.set_content(responseJson.dump(), "application/json");
    });

    svr.listen("localhost", port);
}