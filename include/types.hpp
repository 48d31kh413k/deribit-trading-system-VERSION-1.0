#pragma once
#include <string>
#include <vector>

struct Order {
    std::string id;
    std::string instrument;
    double price;
    double amount;
    bool is_buy;
    std::string status;
};

struct OrderBook {
    std::vector<std::pair<double, double>> bids;
    std::vector<std::pair<double, double>> asks;
    std::string instrument;
    long timestamp;
};