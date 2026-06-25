#include "Checkout.h"
#include "Product.h"
#include "Customer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <limits>

static const std::string ORDERS_FILE = "data/orders.csv";

// ──────────────────────────────────────────────
// Helpers
// ──────────────────────────────────────────────
static std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char buf[12];
    if (now) {
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", now);
        return std::string(buf);
    }
    return "2026-06-24";
}

// ──────────────────────────────────────────────
// CSV persistence
// ──────────────────────────────────────────────
std::vector<Order> loadOrders() {
    std::vector<Order> orders;
    std::ifstream file(ORDERS_FILE);
    if (!file.is_open()) return orders;

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // Skip header row
        if (firstLine) {
            firstLine = false;
            if (line.find("OrderID") != std::string::npos) continue;
        }
        // Format: OrderID,CustomerID,Date,TotalAmount,Items
        // Items field: productId:qty:price;productId:qty:price;...
        auto fields = split(line, ',');
        if (fields.size() < 5) continue;
        try {
            Order ord;
            ord.orderId     = fields[0];
            ord.customerId  = fields[1];
            ord.date        = fields[2];
            ord.totalAmount = std::stod(fields[3]);

            auto itemTokens = split(fields[4], ';');
            for (const auto& token : itemTokens) {
                if (token.empty()) continue;
                auto itemFields = split(token, ':');
                if (itemFields.size() >= 3) {
                    OrderItem item;
                    item.productId       = itemFields[0];
                    item.quantity        = std::stoi(itemFields[1]);
                    item.priceAtPurchase = std::stod(itemFields[2]);
                    ord.items.push_back(item);
                }
            }
            orders.push_back(ord);
        } catch (...) {}
    }
    file.close();
    return orders;
}

void saveOrders(const std::vector<Order>& orders) {
    std::ofstream file(ORDERS_FILE);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open orders file for writing.\n";
        return;
    }
    file << "OrderID,CustomerID,Date,TotalAmount,Items\n";
    for (const auto& ord : orders) {
        file << ord.orderId   << ","
             << ord.customerId << ","
             << ord.date       << ","
             << std::fixed << std::setprecision(2) << ord.totalAmount << ",";
        for (size_t i = 0; i < ord.items.size(); ++i) {
            file << ord.items[i].productId  << ":"
                 << ord.items[i].quantity   << ":"
                 << std::fixed << std::setprecision(2) << ord.items[i].priceAtPurchase;
            if (i < ord.items.size() - 1) file << ";";
        }
        file << "\n";
    }
    file.close();
}

// ──────────────────────────────────────────────
// Checkout menu
// ──────────────────────────────────────────────
void checkoutMenu() {
    std::vector<Customer> customers = loadCustomers();
    if (customers.empty()) {
        std::cout << "\n[Error] No customers registered. Please add a customer first!\n";
        return;
    }

    std::vector<Product> products = loadProducts();
    if (products.empty()) {
        std::cout << "\n[Error] No products available. Please add products first!\n";
        return;
    }

    std::cout << "\n====================================\n";
    std::cout << "          CHECKOUT PROCESS           \n";
    std::cout << "====================================\n";

    // 1. Identify customer
    std::cout << "Available Customers:\n";
    for (const auto& cust : customers) {
        std::cout << "  ID: " << cust.id << " | Name: " << cust.name << "\n";
    }
    std::cout << "Enter Customer ID for checkout: ";
    std::string custId;
    std::getline(std::cin, custId);
    custId = trim(custId);

    auto custIt = std::find_if(customers.begin(), customers.end(),
        [&](const Customer& c) { return c.id == custId; });
    if (custIt == customers.end()) {
        std::cout << "[Error] Customer ID not found. Checkout aborted.\n";
        return;
    }

    // 2. Build cart
    std::vector<OrderItem> cart;
    while (true) {
        std::cout << "\nAvailable Products:\n";
        std::cout << std::left
                  << std::setw(15) << "ID"
                  << std::setw(25) << "Name"
                  << std::setw(12) << "Price ($)"
                  << "Stock\n";
        std::cout << std::string(60, '-') << "\n";
        for (const auto& p : products) {
            std::cout << std::left
                      << std::setw(15) << p.id
                      << std::setw(25) << p.name
                      << std::setw(12) << std::fixed << std::setprecision(2) << p.price
                      << p.quantity << "\n";
        }

        std::cout << "\nEnter Product ID to add (leave empty to proceed to payment): ";
        std::string prodId;
        std::getline(std::cin, prodId);
        prodId = trim(prodId);
        if (prodId.empty()) break;

        auto prodIt = std::find_if(products.begin(), products.end(),
            [&](const Product& p) { return p.id == prodId; });
        if (prodIt == products.end()) {
            std::cout << "Product ID not found. Try again.\n";
            continue;
        }
        if (prodIt->quantity <= 0) {
            std::cout << "'" << prodIt->name << "' is out of stock.\n";
            continue;
        }

        // Calculate remaining stock (accounting for items already in cart)
        int qtyInCart = 0;
        auto cartIt = std::find_if(cart.begin(), cart.end(),
            [&](const OrderItem& item) { return item.productId == prodId; });
        if (cartIt != cart.end()) qtyInCart = cartIt->quantity;

        int maxAvailable = prodIt->quantity - qtyInCart;
        if (maxAvailable <= 0) {
            std::cout << "All available stock of '" << prodIt->name << "' is already in your cart.\n";
            continue;
        }

        std::cout << "Enter quantity for '" << prodIt->name << "' (Max " << maxAvailable << "): ";
        int qty;
        while (!(std::cin >> qty)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter an integer: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (qty <= 0) {
            std::cout << "Quantity must be greater than zero.\n";
            continue;
        }
        if (qty > maxAvailable) {
            std::cout << "Insufficient stock. Only " << maxAvailable << " units available.\n";
            continue;
        }

        if (cartIt != cart.end()) {
            cartIt->quantity += qty;
        } else {
            OrderItem item;
            item.productId       = prodIt->id;
            item.quantity        = qty;
            item.priceAtPurchase = prodIt->price;
            cart.push_back(item);
        }
        std::cout << "[Cart] Added " << qty << " x '" << prodIt->name << "'\n";
    }

    if (cart.empty()) {
        std::cout << "Cart is empty. Checkout cancelled.\n";
        return;
    }

    // 3. Display invoice
    double totalAmount = 0;
    std::cout << "\n====================================\n";
    std::cout << "              INVOICE                \n";
    std::cout << "====================================\n";
    std::cout << "Customer: " << custIt->name << " (ID: " << custIt->id << ")\n";
    std::cout << std::string(55, '-') << "\n";
    std::cout << std::left
              << std::setw(22) << "Product"
              << std::setw(8)  << "Qty"
              << std::setw(12) << "Price ($)"
              << "Subtotal ($)\n";
    std::cout << std::string(55, '-') << "\n";

    for (const auto& item : cart) {
        auto prodIt = std::find_if(products.begin(), products.end(),
            [&](const Product& p) { return p.id == item.productId; });
        std::string name = (prodIt != products.end()) ? prodIt->name : item.productId;
        double subtotal  = item.quantity * item.priceAtPurchase;
        totalAmount += subtotal;
        std::cout << std::left
                  << std::setw(22) << name
                  << std::setw(8)  << item.quantity
                  << std::setw(12) << std::fixed << std::setprecision(2) << item.priceAtPurchase
                  << subtotal << "\n";
    }
    std::cout << std::string(55, '-') << "\n";
    std::cout << "TOTAL AMOUNT: $" << std::fixed << std::setprecision(2) << totalAmount << "\n";
    std::cout << "====================================\n";

    std::cout << "Confirm checkout? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        std::vector<Order> orders = loadOrders();
        std::string orderId = "ORD" + std::to_string(1000 + (int)orders.size() + 1);

        // Deduct stock
        for (const auto& item : cart) {
            auto prodIt = std::find_if(products.begin(), products.end(),
                [&](const Product& p) { return p.id == item.productId; });
            if (prodIt != products.end()) prodIt->quantity -= item.quantity;
        }
        saveProducts(products);

        Order ord;
        ord.orderId     = orderId;
        ord.customerId  = custId;
        ord.date        = getCurrentDate();
        ord.items       = cart;
        ord.totalAmount = totalAmount;
        orders.push_back(ord);
        saveOrders(orders);

        std::cout << "\n[Success] Checkout complete! Order ID: " << orderId << "\n";

        // Post-checkout low stock warnings
        for (const auto& item : cart) {
            auto prodIt = std::find_if(products.begin(), products.end(),
                [&](const Product& p) { return p.id == item.productId; });
            if (prodIt != products.end() && prodIt->quantity <= prodIt->lowStockThreshold) {
                std::cout << "[WARNING] '" << prodIt->name
                          << "' is now low on stock! (Remaining: "
                          << prodIt->quantity << ")\n";
            }
        }
    } else {
        std::cout << "Checkout cancelled. Stock not deducted.\n";
    }
}
