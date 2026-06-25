#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iostream>

struct Product {
    std::string id;
    std::string name;
    double price;
    int quantity;
    int lowStockThreshold;
};

struct Customer {
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
};

struct OrderItem {
    std::string productId;
    int quantity;
    double priceAtPurchase;
};

struct Order {
    std::string orderId;
    std::string customerId;
    std::string date; // Format: YYYY-MM-DD
    std::vector<OrderItem> items;
    double totalAmount;
};

// ──────────────────────────────────────────────
// Common string helpers
// ──────────────────────────────────────────────
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

#endif // MODELS_H
