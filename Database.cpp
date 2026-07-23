
#include "Database.h"

#include <iostream>

    Database::Database(const std::string& filename) {
        int rc = sqlite3_open(filename.c_str(), &db);

        if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        }

        const char* createTABLESQL = "CREATE TABLE IF NOT EXISTS watches ("
                                 "user_id INTEGER, "
                                 "anime_id INTEGER, "
                                 "rating REAL);";

        char* errMsg = nullptr;
        sqlite3_exec(db, createTABLESQL, nullptr, nullptr, &errMsg);
    }

    Database::~Database() {
        sqlite3_close(db);
    }

    void Database::saveWatch(int userId, int animeId, double rating) {
        const char* sql = "INSERT INTO watches(user_id, anime_id, rating) VALUES (?, ?, ?);";
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, animeId);
        sqlite3_bind_double(stmt, 3, rating);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    std::vector<WatchRecord> Database::loadAllWatches() const {
        std::vector<WatchRecord> results;
        const char* sql = "SELECT user_id, anime_id, rating FROM watches;";
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);
            int animeId = sqlite3_column_int(stmt, 1);
            double rating = sqlite3_column_double(stmt, 2);

            results.push_back(WatchRecord{userId, animeId, rating});
        }

        sqlite3_finalize(stmt);
        return results;
    }