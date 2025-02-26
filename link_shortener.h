#ifndef LINK_SHORTENER_H
#define LINK_SHORTENER_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

class LinkShortener {
public:
    explicit LinkShortener(const std::string& file_path);

    std::string shortenUrl(const std::string& long_url, const std::string& custom_short_url = "");
    std::string getLongUrl(const std::string& short_url);

private:
    void loadUrlsFromFile();
    void saveUrlsToFile();

    std::unordered_map<std::string, std::string> url_map_;
    std::string file_path_;
    int id_counter_;
};

#endif // LINK_SHORTENER_H