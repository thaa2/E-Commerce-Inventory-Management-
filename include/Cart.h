#ifndef CART_H
#define CART_H

#include <string>

const int MAX_CART    = 20;
const int MAX_HISTORY = 50;

// ---------------------------------------------------------
// CartItem — plain data only
// ---------------------------------------------------------
struct CartItem {
    int    productId;
    std::string productName;
    int    quantity;
    double unitPrice;
};

// ---------------------------------------------------------
// CartAction — one entry on the undo stack
// ---------------------------------------------------------
struct CartAction {
    CartItem item;
    std::string type;   // "ADD" or "REMOVE"
};

// ---------------------------------------------------------
// Cart — plain data only. itemCount/top drive a fixed array
// cart plus a fixed array stack for undo history.
// ---------------------------------------------------------
struct Cart {
    CartItem items[MAX_CART];
    int      itemCount;

    CartAction history[MAX_HISTORY];
    int        top;       // -1 means history stack is empty
};

// ---------------------------------------------------------
// Free functions — Cart
// ---------------------------------------------------------
void   initCart(Cart& c);                                  // reset to defaults
bool   addItem(Cart& c, const CartItem& item, int availableStock);
bool   removeItem(Cart& c, int productId);
bool   undoLast(Cart& c);
void   displayCart(const Cart& c);
bool   isCartEmpty(const Cart& c);
void   clearCart(Cart& c);
int    getCartItemCount(const Cart& c);
double getCartTotal(const Cart& c);
int    findCartIndexByProductId(const Cart& c, int productId);  // -1 if not found

// internal stack helpers, exposed as free functions instead of private methods
void   pushCartHistory(Cart& c, const CartAction& action);
bool   popCartHistory(Cart& c, CartAction& out);

#endif // CART_H