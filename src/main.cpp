#include "../include/AuthManager.h"
#include "../include/Cart.h"
#include "../include/Customer.h"
#include "../include/User.h"
#include "../include/Order.h"
#include "../include/MenuManager.h"
#include "../include/FileManager.h"
#include "../include/Inventory.h"
#include <iostream>

int main() {
    Inventory inventory;
    CustomerBST customers;
    AuthManager auth;

    // Load database files
    inventory.loadFromFile("data/products.csv");
    customers.loadFromFile("data/customers.csv");

    MenuManager menu(inventory, customers, auth);
    menu.loadOrders("data/orders.csv");

    // Login screen
    menu.showLoginScreen();

    // Main application menu
    menu.showMainMenu();

    return 0;
}