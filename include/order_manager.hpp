#pragma once
#include "types.hpp"
#include <string>

class OrderManager {
public:
    OrderManager();
    bool place_order(const std::string& instrument, double price, 
                    double amount, bool is_buy);
    bool cancel_order(const std::string& order_id);
    OrderBook get_orderbook(const std::string& instrument);
private:
    // Add implementation details later
};