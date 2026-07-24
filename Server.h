#ifndef ANIME_RECOMMENDER_CPP_SERVER_H
#define ANIME_RECOMMENDER_CPP_SERVER_H
#pragma once
#include "httplib.h"
#include "Recommender.h"
#include "JikanClient.h"
#include "Database.h"

class Server {
public:
    Server(Recommender& rec, JikanClient& client, Database& db);
    void run(int port);

private:
    Recommender& recommender;
    JikanClient& jikanClient;
    Database& database;
};

#endif //ANIME_RECOMMENDER_CPP_SERVER_H