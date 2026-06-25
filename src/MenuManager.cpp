#include "../include/MenuManager.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>

// CONSTRUCTOR

MenuManager::MenuManager(Inventory &inv, CustomerBST &cust, AuthManager &a)
    : inventory(inv), customer(cust), auth(a) {
  orderCount = 0;
  nextOrderId = 1;
}

// STATIC HELPERS

void MenuManager::cls() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void MenuManager::header(const std::string &title) {
  std::cout << "\n===================================\n";
  std::cout << "  " << title << "\n";
  std::cout << "====================================\n\n";
}

void MenuManager::pause() {
  std::cout << " Press ENTER to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int MenuManager::getInt(const std::string &prompt, int minVal, int maxVal) {
  int val;
  while (true) {
    std::cout << " " << prompt;
    std::cin >> val;
    if (!std::cin.fail() && val > minVal && val <= maxVal) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return val;
    } else {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "! Enter a number " << minVal << "-" << maxVal << ".\n";
    }
  }
}

std::string MenuManager::getString(const std::string &prompt) {
  std::string val;
  std::cout << " " << prompt;
  std::getline(std::cin, val);
  return val;
}

bool MenuManager::confirm(const std::string &msg) {
  std::cout << " " << msg << "(y/n): ";
  char c;
  std::cin >> c;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return c == 'y' || c == 'Y';
}

// LOGIN

void MenuManager::showLoginScreen() {
  while (true) {
    cls();
    header("LOGIN PAGE");
    std::string user = getString("Username: ");
    std::string pass = getString("Password: ");

    int tries = 1;
    if (auth.login(user, pass)) {
      std::cout << "\n Welcome, " << auth.currentName() << " !\n";
      pause();
      return;
    } else {
      while (tries < 3) {
        std::cout << " !Wrong. " << (3 - tries) << " attempt(s) left.\n";
        user = getString("Username: ");
        pass = getString("Password: ");
        if (auth.login(user, pass)) {
          std::cout << " Welcome, " << auth.currentName() << "!\n";
          pause();
          return;
        }
        tries++;
      }
    }
    std::cout << "\n !! Too many failed attempt. GoodBye.\n";
    exit(0);
  }
}

// MAIN MENU

void MenuManager::showMainMenu() {
  while (true) {
    cls();
    header("MAIN MENU [" + auth.currentName() + "-" +
           (auth.isAdmin() ? "Admin" : "Staff") + "]");

    std::cout << " 1. Inventory\n";
    std::cout << " 2. Cart & Checkout\n";
    std::cout << " 3. Orders\n";
    std::cout << " 4. Customers\n";
    std::cout << " 5. Reports\n";
    if (auth.isAdmin()) {
      std::cout << " 6. User Management\n";
      std::cout << " 0. Logout\n";

      int ch = getInt("Select: ", 0, auth.isAdmin() ? 6 : 5);
      switch (ch) {
      case 1:
        showInventoryMenu();
        break;
      case 2:
        showCartMenu();
        break;
      case 3:
        showOrderMenu();
        break;
      case 4:
        showCustomerMenu();
        break;
      case 5:
        showReportMenu();
        break;
      case 6:
        if (auth.isAdmin()) {
          showUserMenu();
          break;
        }

      case 0:
        auth.logout();
        std::cout << "\n Logged Out.\n";
        return;
      }
    }
  }
}

// INVENOTORY MENU

void MenuManager::showInventoryMenu() {
  while (true) {
    cls();
    header("INVENTORY");
    std::cout << " 1. View All\n";
    std::cout << " 2. Search by SKU\n";
    std::cout << " 3. Sort by Price\n";
    std::cout << " 4. Sort by Name\n";
    std::cout << " 5. Sort by Quantity\n";
    std::cout << " 6. Low stock Alert\n";
    if (auth.isAdmin()) {
      std::cout << " 7. Add Product\n";
      std::cout << " 8. Delete Product\n";
    }
    std::cout << " 0. Back\n\n";

    int ch = getInt("Select: ", 0, 8);
    if (ch == 0) {
      return;
    }
    cls();
    if (ch == 1) {
      header("VIEW ALL\n");
      inventory.displayAll();
      pause();
    } else if (ch == 2) {
      header("SEARCH BY SKU\n");
      std::string sku = getString("SKU: ");
      int id = inventory.hashSearch(sku);
      if (id == -1) {
        std::cout << "! NOT FOUND\n";
      } else {
        Product *p = inventory.findById(id);
        if (p)
          p->display();
      }
      pause();
    } else if (ch == 3) {
      header("SORT BY PRICE\n");
      Product arr[200];
      int n = inventory.copyToArray(arr, 200);
      inventory.sortByPrice(arr, n);
      for (int i = 0; i < n; i++) {
        arr[i].display();
        pause();
      }
    } else if (ch == 4) {
      header("SORT BY NAME\n");
      Product arr[200];
      int n = inventory.copyToArray(arr, 200);
      inventory.sortByName(arr, n);
      for (int i = 0; i < n; i++) {
        arr[i].display();
        pause();
      }
    } else if (ch == 5) {
      header("SORT BY QUANTITY\n");
      Product arr[200];
      int n = inventory.copyToArray(arr, 200);
      inventory.sortByQuantity(arr, n);
      for (int i = 0; i < n; i++) {
        arr[i].display();
        pause();
      }
    } else if (ch == 6) {
      header("LOW STOCK ALERT\n");
      inventory.displayLowStock();
      pause();
    } else if (ch == 7) {
      header("ADD PRODUCT\n");
      Product p;
      p.sku = getString("SKU  :");
      p.name = getString("Name :");
      p.category = getstring("Category :");
      p.price = getInt("Price  :", 0, 999999);
      p.quantity = getInt("Quantity :", 0, 99999);
      p.reorderLevel = getInt("Reorder Level:  :", 0, 99999);
      inventory.addProduct(p);
      std::cout << " Product added\n";
      pause();
    } else if (ch == 8 && auth.isAdmin()) {
      header("DELETE PRODUCT\n");
      int id = getInt("Product Id: ", 1, 999999);
      Product *p = inventory.findById(id);
      if (!p) {
        std::cout << "! Product not found\n";
      } else {
        p->display();
        if (confirm("Delete this product?\n")) {
          inventory.removeProduct(id);
          std::cout << "DELETED!\n";
        }
      }
      pause();
    }
  }
}

// CART MENU

void MenuManager::showCartMenu() {
  while (true) {
    cls();
    header("CART MENU");
    std::cout << " 1. View Cart\n";
    std::cout << " 2. Add Item\n";
    std::cout << " 3. Remove Item\n";
    std::cout << " 4. Undo Last Action\n";
    std::cout << " 5. Checkout\n";
    std::cout << " 6. Clear Cart\n";
    std::cout << " 0. Back\n\n";

    int ch = getInt("Select: ", 0, 6);
    if (ch == 0) {
      return;
    }
    cls();
    if (ch == 1) {
      header("CART CONTENTS");
      cart.display();
      pause();
    } else if (ch == 2) {
      header("ADD ITEM");
      int sku = getString("Product SKU: ");
      int id = inventory.hashSearch(sku);
      if (id == -1) {
        std::cout << " ! Product Not Found\n";
        pause();
        continue;
      }
      Product *p = inventory.findById(id);
      p->display();
      std::cout << "Stock Available: " << p->quantity << "\n";
      int qty = getInt("Quantity", 1, p->quantity);

      CartItem item;
      item.productId = p->id;
      item.productName = p->name;
      item.unitPrice = p->price;
      item.quantity = qty;

      cart.addItem(item, p->quantity);
      std::cout << " Added to Cart!\n";
      pause();
    } else if (ch == 3) {
      header("REMOVE ITEM");
      cart.display();
      int id = getInt("Product ID to remove: ", 1, 999999);
      if (cart.removeItem(id)) {
        std::cout << " Removed.\n";
      } else {
        std::cout << " ! Not in cart.\n";
        pause();
      }
    } else if (ch == 4) {
      header("UNDO");
      if (!cart.undoLast()) {
        std::cout << " ! Nothing to undo.\n";
        pause();
      } else {
        cart.undoList();
      }
    } else if (ch == 5) {
      header("CHECKOUT");
      if (cart.isEmpty()) {
        pause();
        continue;
      }
      cart.display();

      std::string name = getString("Customer name: ");
      CustomerNode *cn = customers.search(name);
      int cusId = cn ? cn->data.id : 0;
      if (!cn) {
        std::cout << "Walk-in customer\n";
      }
      double discount = 0;
      if (auth.isAdmin()) {
        discount = getInt("Discount amount: ", 0, 9999);
      }
      if (!confirm("Confirm Checkout?")) {
        std::cout << "Cancelled\n";
        pause();
        continus;
      }
      Order o = cart.buildOrder(nextorderId++, custId, name, discount);

      for (int i = 0; i < o.itemCount; i++) {
        inventory.adjustStock(o.item[i].productId, -o.item[i].quantity);
      }
      orderQueue.enqueue(o);
      orders[orderCount++] = o;
      saveorders("data/orders.csv");
      o.display();
      cart.clear();
      std::cout << "\n Checkout Complete \n";
      pause();

    } else if (ch == 6) {
      if (confirm("Clear cart?")) {
        cart.clear();
        std::cout << "Cart Cleared.\n";
      }
      pause();
    }
  }
}

// ORDER MENU

void MenuManager::showOrderMenu() {
  while (true) {
    cls();
    header("ORDERS");
    std::cout << " 1. Process Next Order\n";
    std::cout << " 2. View Pending Queue\n";
    std::cout << " 3. View Order History\n";
    std::cout << " 0. Back\n\n\";

        int ch = getInt("Select: ", 0, 3);
    if (ch == 0) {
      return;
    }
    cls();
    if (ch == 1) {
      header("PROCESS NEXT ORDER\n");
      Order o;
      if (orderQueue.dequeue(o)) {
        o.status = Status::Complete;
        std::cout << " Order #" << o.orderId << " for " << o.customerName
                  << " -Completed!\n";
        for (int i = 0; i < orderCount; i++)
          if (orders[i].orderId == o.orderId) {
            orders[i].status = Status::Complete;
            saveOrders("data/orders.csv");
          } else {
            std::cout << " ! No pending orders.\n";
          }
        pause();
      } else if (ch == 2) {
        header("PENDING QUEUE");
        orderQueue.displayAll();
        pause();
      } else if (ch == 3) {
        header("ORDER HISTORY");
        if (orderCount == 0) {
          std::cout << "No orders yet.\n";
        } else {
          for (int i = 0; i < orderCount; i++) {
            orders[i].display();
          }
          pause();
        }
      }
    }
  }

  // CUSTOMERS

  void MenuManager::showCustomerMenu() {
    while (true) {
      cls();
      header("CUSTOMERS");
      std::cout << " 1. View All\n";
      std::cout << " 2. Search by Name\n";
      std::cout << " 3. Add Coutomer\n";
      std::cout << " 0. Back\n\n";

      int ch = getInt("Select: ", 0, 3);
      if (ch == 0) {
        return;
      }
      cls();
      if (ch == 1) {
        header("ALL CUSTOMERS");
        customers.displayAll();
        pause();
      } else if (ch == 2) {
        header("SEARCH CUSTOMERS");
        std::string name = getString("Name: ");
        CustomerNode *n = customers.search(name);
        if (!n) {
          std::cout << " ! Not Found\n";
        } else {
          n->data.display();
        }
        pause();
      } else if (ch == 3) {
        header("ADD CUSTOMER");
        Customer c;
        c.name = getString("Name    :");
        c.email = getString("Email  :");
        c.phone = getString("Phone  :");
        customers.insert(c);
        customers.saveToFile("data/customers.csv");
        std::cout << "Customer added!\n";
        pause();
      }
    }
  }

  // REPORTS MENU

  void MenuManager::showReportMenu() {
    while (true) {
      cls();
      header("REPORTS");
      std::cout << " 1. Inventory Value\n";
      std::cout << " 2. Low Stock\n";
      std::cout << " 3. Revenue Summary\n";
      std::cout << " 0. Back\n\n";

      int ch = getInt("Select: ", 0, 3);
      if (ch == 0) {
        return;
      }
      cls();
      if (ch == 1) {
        header("INVENTORY VALUE\n");
        inventory.displayAll();
        std::cout << "\n Total Value: $" << fixed << setprecision(2)
                  << inventory.totalValue() << "\n";
        pause();
      } else if (ch == 2) {
        header("LOW STOCK");
        inventory.displayLowStock();
        pause();
      } else if (ch == 3) {
        header("REVENUE SUMMARY");
        double revenue = 0;
        for (int i = 0; i < orderCount; i++) {
          if (orders[i].status == Status::Completed) {
            revenue += orders[i] getTotal();
          }
        }
        std::cout << "Total Orders  :" << orderCount << "\n";
        std::cout << "Total Revenue : $ " << fixed << setprecision(2) << revenue
                  << "\n";
        pause();
      }
    }
  }

  // USER MENU

  void MenuManager::showUserMenu() {
    while (true) {
      cls();
      header("USER MANAGEMENT");
      std::cout << " 1. View All Users\n";
      std::cout << " 2. Add User\n";
      std::cout << " 3. Deactivate User\n";
      std::cout << " 4. Back\n\n";

      int ch = getInt("Select: ", 0, 3);
      if (ch == 0) {
        return;
      }
      cls();

      if (ch == 1) {
        header("ALL USERS");
        auth.displayAll();
        pause();
      } else if (ch == 2) {
        header("ADD USER");
        u.username = getString("Username: ");
        std::string pass = getString("Password: ");
        u.passwordHash = User::hashPassword(pass);
        int r = getInt("Role (1=Admin 2=Staff): ", 1, 2);
        u.role = (r == 1) ? "Admin" : "Staff";
        u.isActive = true;
        if (auth.addUser(u)) {
          std::cout << " User added\n";
        } else {
          std::cout << " ! Failed (Duplicate or Full)\n";
        }
        pause();
      } else if (ch == 3) {
        header("DEACTIVATE USER");
        auth.displayAll();
        int id = getInt("User ID: ", 1, 999);
        if (confirm("Deactivate?")) {
          if (auth.deactivateUser(id)) {
            std::cout << "User deactivated.\n";
          } else {
            std::cout << " ! Not Found\n";
          }
        }
        pause();
      }
    }
  }

  // LOAD / SAVE ORDER
void MenuManager::loadOrders(const std::string &pat{
    ifstream file(path);
    if (!file.is_open()) {
      return;
    }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
      if (line.empty())
        continue;
      orders[orderCount] = Order::fromCSV(line);
      if (orders[orderCount].orderId >= nextOrderId) {
        nextOrderId = orders[orderCount].orderId + 1;
      }
      orderCount++;
    }
}

void MenuManager::saveOrders(const std::string& path) const{
    ofstream file(path);
    file << "orderId,customerId,customerName,date,discount,status,items\n";
    for (int i = 0; i < orderCount; i++) {
      file << orders[i].toCSV() << "\n";
    }
}
