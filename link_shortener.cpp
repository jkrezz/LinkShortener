#include "link_shortener.h"
#include <fstream>
#include <iostream>

LinkShortener::LinkShortener(const std::string& file_path) : file_path_(file_path),
id_counter_(1) {
    loadUrlsFromFile();
}

/**
 * @brief Загружает URL-адреса из файла.
 */

void LinkShortener::loadUrlsFromFile() {
    std::ifstream file(file_path_);
    if (file) {
        nlohmann::json json_data;
        file >> json_data;
        for (const auto& item : json_data.items()) {
            url_map_[item.key()] = item.value();
        }
    }
}

/**
 * @brief Сохраняет текущие URL-адреса в файл.
 */

void LinkShortener::saveUrlsToFile() {
    nlohmann::json json_data;
    for (const auto& pair : url_map_) {
        json_data[pair.first] = pair.second;
    }

    std::ofstream file(file_path_);
    if (file) {
        file << json_data.dump(4);
    }
}

/**
 * @brief Сокращает длинный URL.
 * @param long_url Длинный URL, который необходимо сократить.
 * @param custom_short_url Пользовательский короткий URL.
 * @return Короткий URL, либо пустая строка, если URL уже существует.
 */

std::string LinkShortener::shortenUrl(const std::string& long_url,
    const std::string& custom_short_url) {
    std::string short_url = custom_short_url.empty() ? "s" + std::to_string(id_counter_++) : custom_short_url;

    if (url_map_.find(short_url) != url_map_.end()) {
        return "";
    }

    url_map_[short_url] = long_url;
    saveUrlsToFile();
    return short_url;
}

/**
 * @brief Получает длинный URL по укороченному.
 * @param short_url Короткий URL.
 * @return Длинный URL, если найден, иначе пустая строка.
 */

std::string LinkShortener::getLongUrl(const std::string& short_url) {
    auto it = url_map_.find(short_url);
    if (it != url_map_.end()) {
        return it->second;
    }
    return "";
}