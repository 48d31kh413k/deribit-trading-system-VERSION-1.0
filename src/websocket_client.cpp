#include "websocket_client.hpp"
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <chrono>
#include <iostream>

WebSocketClient::WebSocketClient(const std::string& api_key, const std::string& api_secret)
    : api_key_(api_key), api_secret_(api_secret), connected_(false) {
    
    // Set up WebSocket++ logging and other settings
    client_.set_access_channels(websocketpp::log::alevel::none);
    client_.clear_access_channels(websocketpp::log::alevel::frame_payload);
    
    // Initialize ASIO transport
    client_.init_asio();
    
    // Set TLS handler
    client_.set_tls_init_handler([](websocketpp::connection_hdl) {
        return std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    });
    
    // Set callbacks
    client_.set_message_handler(bind(&WebSocketClient::on_message, this, ::_1, ::_2));
    client_.set_open_handler(bind(&WebSocketClient::on_open, this, ::_1));
    client_.set_close_handler(bind(&WebSocketClient::on_close, this, ::_1));
    client_.set_error_handler(bind(&WebSocketClient::on_error, this, ::_1));
}

WebSocketClient::~WebSocketClient() {
    if (connected_) {
        close();
    }
}

bool WebSocketClient::connect(const std::string& uri) {
    try {
        websocketpp::lib::error_code ec;
        Client::connection_ptr con = client_.get_connection(uri, ec);
        
        if (ec) {
            std::cerr << "Connection error: " << ec.message() << std::endl;
            return false;
        }
        
        client_.connect(con);
        
        // Start ASIO io_service run loop
        client_.run();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}

bool WebSocketClient::send(const json& message) {
    if (!connected_) {
        std::cerr << "Not connected" << std::endl;
        return false;
    }
    
    try {
        client_.send(connection_hdl_, message.dump(), websocketpp::frame::opcode::text);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Send error: " << e.what() << std::endl;
        return false;
    }
}

void WebSocketClient::set_message_handler(MessageCallback callback) {
    message_callback_ = callback;
}

void WebSocketClient::close() {
    if (connected_) {
        try {
            client_.close(connection_hdl_, websocketpp::close::status::normal, "");
            connected_ = false;
        } catch (const std::exception& e) {
            std::cerr << "Close error: " << e.what() << std::endl;
        }
    }
}

void WebSocketClient::on_message(websocketpp::connection_hdl hdl, Client::message_ptr msg) {
    if (message_callback_) {
        message_callback_(msg->get_payload());
    }
}

void WebSocketClient::on_open(websocketpp::connection_hdl hdl) {
    connection_hdl_ = hdl;
    connected_ = true;
    
    // Authenticate with Deribit
    auto timestamp = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
    
    json auth_message = {
        {"jsonrpc", "2.0"},
        {"id", 9929},
        {"method", "public/auth"},
        {"params", {
            {"grant_type", "client_credentials"},
            {"client_id", api_key_},
            {"client_secret", api_secret_},
            {"timestamp", timestamp}
        }}
    };
    
    send(auth_message);
}

void WebSocketClient::on_close(websocketpp::connection_hdl hdl) {
    connected_ = false;
    std::cout << "Connection closed" << std::endl;
}

void WebSocketClient::on_error(websocketpp::connection_hdl hdl) {
    Client::connection_ptr con = client_.get_con_from_hdl(hdl);
    std::cerr << "Error: " << con->get_ec().message() << std::endl;
}

std::string WebSocketClient::generate_auth_signature(const std::string& timestamp) {
    std::string data = timestamp + "\n" + api_secret_;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    HMAC_CTX* hmac = HMAC_CTX_new();
    HMAC_Init_ex(hmac, api_secret_.c_str(), api_secret_.length(), EVP_sha256(), nullptr);
    HMAC_Update(hmac, (unsigned char*)data.c_str(), data.length());
    HMAC_Final(hmac, hash, nullptr);
    HMAC_CTX_free(hmac);
    
    std::string signature;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char hex[3];
        sprintf(hex, "%02x", hash[i]);
        signature += hex;
    }
    
    return signature;
}