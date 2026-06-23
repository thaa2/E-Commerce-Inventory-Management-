#include "Cart.h"
#include <iostream>
#include <iomanip>

using namespace std;

void initCart(Cart& c) {
    c.itemCount = 0;
    c.top       = -1;
}

// ===========================================================
// Stack helpers (undo history) — LIFO, plain array
// ===========================================================

void pushCartHistory(Cart& c, const CartAction& action) {
    if (c.top >= MAX_HISTORY - 1) {
        return;   // history full, oldest actions just become un-undoable
    }
    c.top++;
    c.history[c.top] = action;
}

bool popCartHistory(Cart& c, CartAction& out) {
    if (c.top == -1) {
        return false;
    }
    out = c.history[c.top];
    c.top--;
    return true;
}

// ===========================================================
// Cart contents
// ===========================================================

int findCartIndexByProductId(const Cart& c, int productId) {
    for (int i = 0; i < c.itemCount; i++) {
        if (c.items[i].productId == productId) {
            return i;
        }
    }
    return -1;
}

bool addCartItem(Cart& c, const CartItem& item, int availableStock) {
    if (item.quantity <= 0) {
        cout << "Quantity must be positive.\n";
        return false;
    }
    if (item.quantity > availableStock) {
        cout << "Not enough stock available for " << item.productName << ".\n";
        return false;
    }

    int existing = findCartIndexByProductId(c, item.productId);
    if (existing != -1) {
        c.items[existing].quantity += item.quantity;
    } else {
        if (c.itemCount >= MAX_CART) {
            cout << "Cart is full, cannot add more items.\n";
            return false;
        }
        c.items[c.itemCount] = item;
        c.itemCount++;
    }

    CartAction action;
    action.item = item;
    action.type = "ADD";
    pushCartHistory(c, action);

    return true;
}

bool removeCartItem(Cart& c, int productId) {
    int idx = findCartIndexByProductId(c, productId);
    if (idx == -1) {
        return false;
    }

    for (int i = idx; i < c.itemCount - 1; i++) {
        c.items[i] = c.items[i + 1];
    }
    c.itemCount--;
    return true;
}

bool undoLastCartAction(Cart& c) {
    CartAction last;
    if (!popCartHistory(c, last)) {
        cout << "Nothing to undo.\n";
        return false;
    }

    if (last.type == "ADD") {
        removeCartItem(c, last.item.productId);
        cout << "Undid: removed " << last.item.productName << " from cart.\n";
    } else if (last.type == "REMOVE") {
        if (c.itemCount < MAX_CART) {
            c.items[c.itemCount] = last.item;
            c.itemCount++;
        }
        cout << "Undid: restored " << last.item.productName << " to cart.\n";
    }
    return true;
}

void displayCart(const Cart& c) {
    if (c.itemCount == 0) {
        cout << "Cart is empty.\n";
        return;
    }
    cout << "===== Your Cart =====\n";
    cout << left << setw(6) << "Qty" << setw(20) << "Product" << setw(10) << "Price" << "\n";
    for (int i = 0; i < c.itemCount; i++) {
        cout << left << setw(6) << c.items[i].quantity
             << setw(20) << c.items[i].productName
             << setw(10) << fixed << setprecision(2) << c.items[i].unitPrice << "\n";
    }
    cout << "Total: $" << fixed << setprecision(2) << getCartTotal(c) << "\n";
}

bool isCartEmpty(const Cart& c) {
    return c.itemCount == 0;
}

void clearCart(Cart& c) {
    c.itemCount = 0;
    c.top       = -1;
}

int getCartItemCount(const Cart& c) {
    return c.itemCount;
}

double getCartTotal(const Cart& c) {
    double sum = 0.0;
    for (int i = 0; i < c.itemCount; i++) {
        sum += c.items[i].quantity * c.items[i].unitPrice;
    }
    return sum;
}