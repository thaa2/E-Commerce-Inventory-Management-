#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <iostream>
#include <string>
using namespace std;

class Inventory;
class CustomerBST;
class AuthManager;
class OrderQueue;
class Cart;

class MenuManager {
    private: 
        Inventory&      inventory;
        CustomerBST&    customer;
        AuthManager&    auth;
        OrderQueue      orderQueue;
        Cart            activeCart;
        int             nextOrderId;
}

#endif // MENUMANAGER_H
