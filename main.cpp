#include "link_shortener.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

/**
 * @brief Обрабатывает HTTP-запросы к серверу.
 *
 * @param req Входящий HTTP запрос.
 * @param res Ответ HTTP.
 * @param shortener Объект для управления ссылками.
 */

void handle_request(const http::request<http::string_body>& req,
    http::response<http::string_body>& res, LinkShortener& shortener) {
    if (req.method() == http::verb::post && req.target() == "/shorten") {
        auto json_body = nlohmann::json::parse(req.body(), nullptr, false);
        if (!json_body.is_object() || !json_body.contains("long_url")
            || !json_body.contains("short_url")) {
            res.result(http::status::bad_request);
            res.body() = "Invalid request format";
        } else {
            std::string long_url = json_body["long_url"];
            std::string custom_short_url;

            if (json_body.contains("short_url")) {
                custom_short_url = json_body["short_url"];
            }

            std::string short_url = shortener.shortenUrl(long_url, custom_short_url);

            if (short_url.empty()) {
                res.result(http::status::conflict);
                res.body() = "Short URL already exists";
            } else {
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = "{\"short_url\": \"http://127.0.0.1:8080/" + short_url + "\"}";
            }
        }
        res.prepare_payload();
    }
    else if (req.method() == http::verb::get) {
        std::string short_url = req.target().substr(1);
        std::string long_url = shortener.getLongUrl(short_url);

        if (long_url.empty()) {
            res.result(http::status::not_found);
            res.body() = "Not found";
        } else {
            res.result(http::status::moved_permanently);
            res.set(http::field::location, long_url);
        }
        res.prepare_payload();
    }
}

/**
 * @brief Главная функция, запускающая сервер.
 */

int main() {
    try {
        const std::string file_path = "urls.json";
        const std::string address = "0.0.0.0";
        const int port = 8080;

        net::io_context ioc;
        LinkShortener shortener(file_path);

        net::ip::tcp::acceptor acceptor(ioc, {net::ip::make_address(address),
            static_cast<unsigned short>(port)});
        std::cout << "Server running at http://" << address << ":" << port << std::endl;

        while (true) {
            net::ip::tcp::socket socket(ioc);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            handle_request(req, res, shortener);
            http::write(socket, res);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

// curl -X POST http://0.0.0.0:8080/shorten      -H "Content-Type: application/json"      -d '{"long_url": "https://jetbrains.com", "short_url": "jb"}'










































