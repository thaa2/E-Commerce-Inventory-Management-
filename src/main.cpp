#include "../include/AuthManager.h"
#include "../include/Customer.h"
#include "../include/FileManager.h"
#include "../include/Inventory.h"
#include "../include/MenuManager.h"
#include <iostream>
#include <cstdlib>


int main() {
  

  // 2. Create and load all managers
  Inventory inventory;
  CustomerBST customers;
  AuthManager auth;

  inventory.loadFromFile("data/products.csv");
  customers.loadFromFile("data/customers.csv");
  auth.loadFromFile("data/users.csv");
  auth.seedAdmin();

  // 3. Start the app
  MenuManager menu(inventory, customers, auth);
  menu.loadOrders("data/orders.csv");
  menu.showLoginScreen();
  menu.showMainMenu();
  return 0;
}
