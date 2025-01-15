#pragma once

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <json.hpp>
#include <functional>
#include <string>
#include <memory>

using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class WebSocketClient {
public:
    using Client = websocketpp::client<websocketpp::config::asio_tls_client>;
    using MessageCallback = std::function<void(const std::string&)>;

    WebSocketClient(const std::string& api_key, const std::string& api_secret);
    ~WebSocketClient();

    bool connect(const std::string& uri);
    bool send(const json& message);
    void set_message_handler(MessageCallback callback);
    void close();

private:
    void on_message(websocketpp::connection_hdl hdl, Client::message_ptr msg);
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_error(websocketpp::connection_hdl hdl);
    std::string generate_auth_signature(const std::string& timestamp);

    Client client_;
    websocketpp::connection_hdl connection_hdl_;
    std::string api_key_;
    std::string api_secret_;
    MessageCallback message_callback_;
    bool connected_;
};