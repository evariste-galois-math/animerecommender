#ifndef ANIME_RECOMMENDER_CPP_DATABASE_H
#define ANIME_RECOMMENDER_CPP_DATABASE_H
#pragma once
#include <libpq-fe.h>
#include <vector>
#include <string>

struct WatchRecord {
    int userId;
    int animeId;
    double rating;
};

class Database {
public:
    Database(const std::string& connectionString);
    ~Database();

    void saveWatch(int userId, int animeId, double rating);
    std::vector<WatchRecord> loadAllWatches() const;

private:
    PGconn* conn;
};

#endif //ANIME_RECOMMENDER_CPP_DATABASE_H