#include "websocket_client.hpp"

int main() {
    const std::string api_key = "YOUR_API_KEY";
    const std::string api_secret = "YOUR_API_SECRET";
    const std::string uri = "wss://test.deribit.com/ws/api/v2";
    
    WebSocketClient client(api_key, api_secret);
    
    // Set message handler
    client.set_message_handler([](const std::string& message) {
        std::cout << "Received: " << message << std::endl;
    });
    
    // Connect to Deribit
    if (client.connect(uri)) {
        // Subscribe to orderbook
        json subscribe_message = {
            {"jsonrpc", "2.0"},
            {"id", 3600},
            {"method", "public/subscribe"},
            {"params", {
                {"channels", {"book.BTC-PERPETUAL.100ms"}}
            }}
        };
        
        client.send(subscribe_message);
    }
    
    // Keep program running
    std::cin.get();
    
    return 0;
}