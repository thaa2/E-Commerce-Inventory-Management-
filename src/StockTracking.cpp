#include "StockTracking.h"
#include "Product.h"
#include <iostream>
#include <iomanip>
#include <limits>

// ──────────────────────────────────────────────
// Print low-stock alerts for products at or below threshold
// ──────────────────────────────────────────────
void checkStockAlerts() {
    std::vector<Product> products = loadProducts();
    bool anyAlert = false;
    for (const auto& p : products) {
        if (p.quantity <= p.lowStockThreshold) {
            if (!anyAlert) {
                std::cout << "\n[STOCK ALERT] The following products are low on stock:\n";
                std::cout << std::string(50, '-') << "\n";
                anyAlert = true;
            }
            std::cout << "  * " << p.name
                      << " (ID: " << p.id << ")"
                      << " — Remaining: " << p.quantity
                      << " (Threshold: " << p.lowStockThreshold << ")\n";
        }
    }
    if (!anyAlert && !products.empty()) {
        std::cout << "[Stock] All products are sufficiently stocked.\n";
    }
}

// ──────────────────────────────────────────────
// Display full inventory status table
// ──────────────────────────────────────────────
void viewInventoryStatus() {
    std::vector<Product> products = loadProducts();
    std::cout << "\n--- Full Inventory Status ---\n";
    if (products.empty()) {
        std::cout << "No products in inventory.\n";
        return;
    }
    std::cout << std::left
              << std::setw(12) << "ID"
              << std::setw(25) << "Name"
              << std::setw(10) << "Stock"
              << std::setw(12) << "Threshold"
              << "Status\n";
    std::cout << std::string(65, '-') << "\n";
    for (const auto& p : products) {
        std::string status = (p.quantity <= p.lowStockThreshold) ? "*** LOW ***" : "OK";
        std::cout << std::left
                  << std::setw(12) << p.id
                  << std::setw(25) << p.name
                  << std::setw(10) << p.quantity
                  << std::setw(12) << p.lowStockThreshold
                  << status << "\n";
    }
}

// ──────────────────────────────────────────────
// Interactive menu
// ──────────────────────────────────────────────
void stockTrackingMenu() {
    while (true) {
        std::cout << "\n====================================\n";
        std::cout << "     STOCK LEVEL TRACKING MENU     \n";
        std::cout << "====================================\n";
        std::cout << "1. View Low Stock Alerts\n";
        std::cout << "2. View Full Inventory Status\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 3) break;

        switch (choice) {
            case 1: checkStockAlerts();    break;
            case 2: viewInventoryStatus(); break;
            default:
                std::cout << "Invalid choice. Please choose between 1 and 3.\n";
        }
    }
}
