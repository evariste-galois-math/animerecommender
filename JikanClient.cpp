
#include "JikanClient.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

const std::string BASE_URL = "https://api.tenrai.org/v1";

size_t JikanClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string JikanClient::fetchAnime(int animeId) const {
    std::string response;
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string url = BASE_URL + "/anime/" + std::to_string(animeId);
        char errbuf[CURL_ERROR_SIZE] = {0};

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        CURLcode res = curl_easy_perform(curl);

        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (res != CURLE_OK) {
            std::cerr << "curl error for ID " << animeId << ": " << errbuf << std::endl;
            response = "";
        } else if (httpCode != 200) {
            std::cerr << "HTTP " << httpCode << " for ID " << animeId << std::endl;
            response = "";
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

AnimeInfo JikanClient::fetchAnimeParsed(int animeId) const {
    std::string rawResponse = fetchAnime(animeId);

    if (rawResponse.empty()) {
        return AnimeInfo{animeId, "Unknown Title (Network Error)", 0.0};
    }

    json parsed = json::parse(rawResponse, nullptr, false);

    if (parsed.is_discarded() || !parsed.contains("data") || parsed["data"].is_null()) {
        return AnimeInfo{animeId, "Unknown Title (Rate Limited or Not Found)", 0.0};
    }

    int id = parsed["data"].value("mal_id", animeId);
    std::string title = parsed["data"].value("title", "Unknown Title");

    // score can come back as JSON null instead of being missing, so .value() alone isn't safe here
    double score;
    if (parsed["data"].contains("score") && !parsed["data"]["score"].is_null()) {
        score = parsed["data"]["score"].get<double>();
    } else {
        score = 0.0;
    }

    return AnimeInfo{id, title, score};
}

std::vector<AnimeInfo> JikanClient::searchAnime(const std::string& query) const {
    std::string response;
    CURL* curl = curl_easy_init();

    if (curl) {
        char* encodedQuery = curl_easy_escape(curl, query.c_str(), query.length());
        std::string url = BASE_URL + "/anime?q=" + std::string(encodedQuery);
        curl_free(encodedQuery);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        long httpCode = 0;
        CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (res != CURLE_OK || httpCode != 200) {
            response = "";
        }

        curl_easy_cleanup(curl);
    }

    std::vector<AnimeInfo> results;

    if (response.empty()) {
        return results;
    }

    json parsed = json::parse(response, nullptr, false);

    if (parsed.is_discarded() || !parsed.contains("data")) {
        return results;
    }

    for (const auto& item : parsed["data"]) {
        int id = item.value("mal_id", 0);
        std::string title = item.value("title", "Unknown");

        // same null-safety issue as above — score can be explicitly null
        double score;
        if (item.contains("score") && !item["score"].is_null()) {
            score = item["score"].get<double>();
        } else {
            score = 0.0;
        }

        results.push_back(AnimeInfo{id, title, score});
    }

    return results;
}