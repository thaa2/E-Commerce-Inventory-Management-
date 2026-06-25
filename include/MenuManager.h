#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include "AuthManager.h"
#include "Cart.h"
#include "Customer.h"
#include "Inventory.h"
#include "Order.h"

const int MAX_ORDERS = 100;

class MenuManager {
public:
  Inventory &inventory;
  CustomerBST &customers;
  AuthManager &auth;

  Order orders[MAX_ORDERS];
  int orderCount;
  int nextOrderId;

  OrderQueue orderQueue;
  Cart cart;

  MenuManager(Inventory &inv, CustomerBST &cust, AuthManager &a);

  void loadOrders(const std::string &path);
  void saveOrders(const std::string &path) const;

  void cls();
  void pause();
  void showLoginScreen();
  void showMainMenu();
  void showInventoryMenu();
  void showCartMenu();
  void showOrderMenu();
  void showCustomerMenu();
  void showReportMenu();
  void showUserMenu();

  static int getInt(const std::string &prompt, int min, int max);
  static std::string getString(const std::string &prompt);
  static bool confirm(const std::string &msg);
  static void header(const std::string &title);
};
#endif
