#include <Inventory.h>
#include <Customer.h>
#include <AuthManager.h>
#include <Order.h>
#include <Cart.h>
#include <string>
#include <iostream>
using namespace std;

class MenuManager{
    public:
        Inventory&      inventory;
        CustomerBST&    customer;
        AuthManager&    auth;

        Order           orders[MAX_ORDERS];
        int             orderCount;
        int             nextorderId;
        
        OrderQueue      orderQueue;
        Cart            cart;

        MenuManager (Inventory & inv, CustomerBST& cust, AuthManager&y a);

        void loadOrders (const string &path);
        void saveOrders(const string &path) const;

        void showLoginScreen();
        void showMainMenu();
        void showInventoryMenu();
        void showCartMenu();
        void showOrderMenu();
        void showCustomerMenu();
        void showReportMenu();
        void showUserMenu();

        static int getInt(const string &prompt, int min, int max);
        static string getString(const string &prompt);
        static bool confirm(const string &message);
        static void header (const string &title);
};