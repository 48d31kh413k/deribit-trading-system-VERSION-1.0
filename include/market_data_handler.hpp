#pragma once
#include "types.hpp"
#include <string>

class MarketDataHandler {
public:
    MarketDataHandler();
    void subscribe(const std::string& instrument);
    void unsubscribe(const std::string& instrument);
    OrderBook get_orderbook(const std::string& instrument);
private:
    // Add implementation details later
};