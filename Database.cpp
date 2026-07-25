#include "Database.h"
#include <iostream>

Database::Database(const std::string& connectionString) {
    conn = PQconnectdb(connectionString.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
    }

    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS watches ("
        "user_id INTEGER, "
        "anime_id INTEGER, "
        "rating REAL);";

    PGresult* res = PQexec(conn, createTableSQL);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to create table: " << PQerrorMessage(conn) << std::endl;
    }
    PQclear(res);
}

Database::~Database() {
    PQfinish(conn);
}

void Database::saveWatch(int userId, int animeId, double rating) {
    std::string userIdStr = std::to_string(userId);
    std::string animeIdStr = std::to_string(animeId);
    std::string ratingStr = std::to_string(rating);

    const char* paramValues[3] = { userIdStr.c_str(), animeIdStr.c_str(), ratingStr.c_str() };

    const char* sql = "INSERT INTO watches (user_id, anime_id, rating) VALUES ($1, $2, $3);";

    PGresult* res = PQexecParams(conn, sql, 3, nullptr, paramValues, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Insert failed: " << PQerrorMessage(conn) << std::endl;
    }

    PQclear(res);
}

std::vector<WatchRecord> Database::loadAllWatches() const {
    std::vector<WatchRecord> results;

    const char* sql = "SELECT user_id, anime_id, rating FROM watches;";
    PGresult* res = PQexec(conn, sql);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return results;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        int userId = std::stoi(PQgetvalue(res, i, 0));
        int animeId = std::stoi(PQgetvalue(res, i, 1));
        double rating = std::stod(PQgetvalue(res, i, 2));

        results.push_back(WatchRecord{userId, animeId, rating});
    }

    PQclear(res);
    return results;
}