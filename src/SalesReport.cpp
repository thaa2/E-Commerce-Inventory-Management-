#include "SalesReport.h"
#include "Checkout.h"
#include "Product.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>

// ──────────────────────────────────────────────
// Generate and print the full sales report
// ──────────────────────────────────────────────
void viewSalesReport() {
    std::vector<Order>   orders   = loadOrders();
    std::vector<Product> products = loadProducts();

    std::cout << "\n====================================\n";
    std::cout << "            SALES REPORT             \n";
    std::cout << "====================================\n";

    if (orders.empty()) {
        std::cout << "No sales data recorded yet.\n";
        return;
    }

    double totalRevenue   = 0;
    int    totalItemsSold = 0;
    int    totalOrders    = static_cast<int>(orders.size());

    std::map<std::string, int>    productQtySold;
    std::map<std::string, double> productRevenue;
    std::map<std::string, double> dailyRevenue;

    for (const auto& ord : orders) {
        totalRevenue          += ord.totalAmount;
        dailyRevenue[ord.date] += ord.totalAmount;
        for (const auto& item : ord.items) {
            productQtySold[item.productId] += item.quantity;
            productRevenue[item.productId] += item.quantity * item.priceAtPurchase;
            totalItemsSold                 += item.quantity;
        }
    }

    // Summary
    std::cout << std::left << std::setw(25) << "Total Orders:"       << totalOrders    << "\n";
    std::cout << std::left << std::setw(25) << "Total Revenue:"      << "$"
              << std::fixed << std::setprecision(2) << totalRevenue   << "\n";
    std::cout << std::left << std::setw(25) << "Total Items Sold:"   << totalItemsSold << " units\n";
    if (totalOrders > 0) {
        std::cout << std::left << std::setw(25) << "Avg Order Value:"
                  << "$" << std::fixed << std::setprecision(2)
                  << (totalRevenue / totalOrders) << "\n";
    }
    std::cout << std::string(40, '-') << "\n";

    // Top selling products (by qty sold)
    std::vector<std::pair<std::string, int>> sortedProducts(
        productQtySold.begin(), productQtySold.end());
    std::sort(sortedProducts.begin(), sortedProducts.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "\n--- Top Selling Products (by Quantity) ---\n";
    std::cout << std::left
              << std::setw(14) << "Product ID"
              << std::setw(25) << "Name"
              << std::setw(12) << "Units Sold"
              << "Revenue ($)\n";
    std::cout << std::string(65, '-') << "\n";

    int limit = std::min(5, (int)sortedProducts.size());
    for (int i = 0; i < limit; ++i) {
        const std::string& pid = sortedProducts[i].first;
        int    qty = sortedProducts[i].second;
        double rev = productRevenue[pid];

        std::string name = "Unknown (Deleted)";
        auto prodIt = std::find_if(products.begin(), products.end(),
            [&](const Product& p) { return p.id == pid; });
        if (prodIt != products.end()) name = prodIt->name;

        std::cout << std::left
                  << std::setw(14) << pid
                  << std::setw(25) << name
                  << std::setw(12) << qty
                  << "$" << std::fixed << std::setprecision(2) << rev << "\n";
    }

    // Daily sales trend
    std::cout << "\n--- Daily Sales Trend ---\n";
    std::cout << std::left << std::setw(20) << "Date" << "Revenue ($)\n";
    std::cout << std::string(35, '-') << "\n";
    for (const auto& entry : dailyRevenue) {
        std::cout << std::left << std::setw(20) << entry.first
                  << "$" << std::fixed << std::setprecision(2) << entry.second << "\n";
    }
    std::cout << "====================================\n";
}

// ──────────────────────────────────────────────
// Interactive menu
// ──────────────────────────────────────────────
void salesReportMenu() {
    while (true) {
        std::cout << "\n====================================\n";
        std::cout << "       SALES ANALYTICS & REPORTS    \n";
        std::cout << "====================================\n";
        std::cout << "1. Generate Sales Report\n";
        std::cout << "2. Back to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 2) break;

        switch (choice) {
            case 1: viewSalesReport(); break;
            default:
                std::cout << "Invalid choice. Please choose 1 or 2.\n";
        }
    }
}
