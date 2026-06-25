#include "MenuManager.h"
#include "Product.h"
#include "Customer.h"
#include "StockTracking.h"
#include "Checkout.h"
#include "SearchFilter.h"
#include "SalesReport.h"
#include <iostream>
#include <limits>

void runMainMenu() {
    // Show any low-stock warnings on startup
    checkStockAlerts();

    while (true) {
        std::cout << "\n====================================\n";
        std::cout << "             MAIN MENU              \n";
        std::cout << "====================================\n";
        std::cout << "1. Product Management (CRUD)\n";
        std::cout << "2. Customer Management (CRUD)\n";
        std::cout << "3. Stock Level Tracking & Alerts\n";
        std::cout << "4. Customer Checkout Process\n";
        std::cout << "5. Order Search & Filter\n";
        std::cout << "6. Sales Analytics & Reports\n";
        std::cout << "7. Exit System\n";
        std::cout << "Enter your choice (1-7): ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 7) {
            std::cout << "\nThank you for using the E-Commerce Management System. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: productMenu();       break;
            case 2: customerMenu();      break;
            case 3: stockTrackingMenu(); break;
            case 4: checkoutMenu();      break;
            case 5: searchFilterMenu();  break;
            case 6: salesReportMenu();   break;
            default:
                std::cout << "Invalid choice. Please select between 1 and 7.\n";
        }
    }
}
