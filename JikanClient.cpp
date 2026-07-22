#include "JikanClient.h"
#include <curl/curl.h>

size_t JikanClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string JikanClient::fetchAnime(int animeId) const {
    std::string response;
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string url = "https://api.jikan.moe/v4/anime/" + std::to_string(animeId);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "";
        }

        curl_easy_cleanup(curl);
    }

    return response;
}