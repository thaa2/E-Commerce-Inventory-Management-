#include "../include/AuthManager.h"
#include "../include/Customer.h"
#include "../include/FileManager.h"
#include "../include/Inventory.h"
#include "../include/MenuManager.h"

int main() {
  // 1. Make sure all CSV files exist
  FileManager::ensureFile("data/products.csv",
                          "id,sku,name,category,price,quantity,reorderLevel");
  FileManager::ensureFile("data/customers.csv", "id,name,email,phone");
  FileManager::ensureFile(
      "data/orders.csv",
      "orderId,customerId,customerName,date,discount,status,items");
  FileManager::ensureFile("data/users.csv",
                          "id,username,passwordHash,role,isActive");

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
  return 0;
}
