#include "SearchFilter.h"
#include "Checkout.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

// ──────────────────────────────────────────────
// Print a single order's details
// (renamed from displayOrder to avoid linker conflict with Order.cpp)
// ──────────────────────────────────────────────
void printOrderDetails(const Order& ord) {
    std::cout << "\n-------------------------------------\n";
    std::cout << "Order ID:    " << ord.orderId     << "\n";
    std::cout << "Customer ID: " << ord.customerId  << "\n";
    std::cout << "Date:        " << ord.date        << "\n";
    std::cout << "Items Purchased:\n";
    std::cout << std::left
              << "  " << std::setw(14) << "Product ID"
              << std::setw(8)  << "Qty"
              << "Price/Unit ($)\n";
    for (const auto& item : ord.items) {
        std::cout << "  " << std::left
                  << std::setw(14) << item.productId
                  << std::setw(8)  << item.quantity
                  << std::fixed << std::setprecision(2) << item.priceAtPurchase << "\n";
    }
    std::cout << "Total Amount: $"
              << std::fixed << std::setprecision(2) << ord.totalAmount << "\n";
    std::cout << "-------------------------------------\n";
}

// ──────────────────────────────────────────────
// Interactive search & filter menu
// ──────────────────────────────────────────────
void searchFilterMenu() {
    while (true) {
        std::vector<Order> orders = loadOrders();

        std::cout << "\n====================================\n";
        std::cout << "      ORDER SEARCH & FILTER MENU    \n";
        std::cout << "====================================\n";
        std::cout << "1. Search Order by Order ID\n";
        std::cout << "2. Filter Orders by Customer ID\n";
        std::cout << "3. Filter Orders by Date Range\n";
        std::cout << "4. Filter Orders by Value Range\n";
        std::cout << "5. View All Orders\n";
        std::cout << "6. Back to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                std::cout << "Enter Order ID (e.g. ORD1001): ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(orders.begin(), orders.end(),
                    [&](const Order& ord) { return ord.orderId == targetId; });
                if (it != orders.end()) {
                    printOrderDetails(*it);
                } else {
                    std::cout << "Order '" << targetId << "' not found.\n";
                }
                break;
            }
            case 2: {
                std::cout << "Enter Customer ID: ";
                std::string targetCustId;
                std::getline(std::cin, targetCustId);
                targetCustId = trim(targetCustId);

                int count = 0;
                std::cout << "\nOrders for Customer '" << targetCustId << "':\n";
                for (const auto& ord : orders) {
                    if (ord.customerId == targetCustId) {
                        printOrderDetails(ord);
                        ++count;
                    }
                }
                if (count == 0) {
                    std::cout << "No orders found for Customer ID '" << targetCustId << "'.\n";
                } else {
                    std::cout << "Total matching orders: " << count << "\n";
                }
                break;
            }
            case 3: {
                std::cout << "Enter Start Date (YYYY-MM-DD): ";
                std::string startDate;
                std::getline(std::cin, startDate);
                startDate = trim(startDate);

                std::cout << "Enter End Date   (YYYY-MM-DD): ";
                std::string endDate;
                std::getline(std::cin, endDate);
                endDate = trim(endDate);

                int count = 0;
                std::cout << "\nOrders between " << startDate << " and " << endDate << ":\n";
                for (const auto& ord : orders) {
                    if (ord.date >= startDate && ord.date <= endDate) {
                        printOrderDetails(ord);
                        ++count;
                    }
                }
                if (count == 0) {
                    std::cout << "No orders found in that date range.\n";
                } else {
                    std::cout << "Total matching orders: " << count << "\n";
                }
                break;
            }
            case 4: {
                std::cout << "Enter Minimum Order Amount ($): ";
                double minVal;
                while (!(std::cin >> minVal)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Enter a number: ";
                }
                std::cout << "Enter Maximum Order Amount ($): ";
                double maxVal;
                while (!(std::cin >> maxVal)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Enter a number: ";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int count = 0;
                std::cout << "\nOrders between $" << minVal << " and $" << maxVal << ":\n";
                for (const auto& ord : orders) {
                    if (ord.totalAmount >= minVal && ord.totalAmount <= maxVal) {
                        printOrderDetails(ord);
                        ++count;
                    }
                }
                if (count == 0) {
                    std::cout << "No orders found in that value range.\n";
                } else {
                    std::cout << "Total matching orders: " << count << "\n";
                }
                break;
            }
            case 5: {
                std::cout << "\n--- All Recorded Orders ---\n";
                if (orders.empty()) {
                    std::cout << "No orders recorded yet.\n";
                } else {
                    for (const auto& ord : orders) printOrderDetails(ord);
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please choose between 1 and 6.\n";
        }
    }
}
