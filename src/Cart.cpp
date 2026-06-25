#include "Cart.h"
#include <iostream>
#include <iomanip>

void initCart(Cart& c) {
    c.itemCount = 0;
    c.top       = -1;
}

// ===========================================================
// Stack helpers (undo history) — LIFO, plain array
// ===========================================================

void pushCartHistory(Cart& c, const CartAction& action) {
    if (c.top >= MAX_HISTORY - 1) {
        return;   // history full
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

bool addItem(Cart& c, const CartItem& item, int availableStock) {
    if (item.quantity <= 0) {
        std::cout << "Quantity must be positive.\n";
        return false;
    }
    if (item.quantity > availableStock) {
        std::cout << "Not enough stock available for " << item.productName << ".\n";
        return false;
    }

    int existing = findCartIndexByProductId(c, item.productId);
    if (existing != -1) {
        c.items[existing].quantity += item.quantity;
    } else {
        if (c.itemCount >= MAX_CART) {
            std::cout << "Cart is full, cannot add more items.\n";
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

bool removeItem(Cart& c, int productId) {
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

bool undoLast(Cart& c) {
    CartAction last;
    if (!popCartHistory(c, last)) {
        std::cout << "Nothing to undo.\n";
        return false;
    }

    if (last.type == "ADD") {
        removeItem(c, last.item.productId);
        std::cout << "Undid: removed " << last.item.productName << " from cart.\n";
    } else if (last.type == "REMOVE") {
        if (c.itemCount < MAX_CART) {
            c.items[c.itemCount] = last.item;
            c.itemCount++;
        }
        std::cout << "Undid: restored " << last.item.productName << " to cart.\n";
    }
    return true;
}

void displayCart(const Cart& c) {
    if (c.itemCount == 0) {
        std::cout << "Cart is empty.\n";
        return;
    }
    std::cout << "===== Your Cart =====\n";
    std::cout << std::left << std::setw(6) << "Qty" << std::setw(20) << "Product" << std::setw(10) << "Price" << "\n";
    for (int i = 0; i < c.itemCount; i++) {
        std::cout << std::left << std::setw(6) << c.items[i].quantity
             << std::setw(20) << c.items[i].productName
             << std::setw(10) << std::fixed << std::setprecision(2) << c.items[i].unitPrice << "\n";
    }
    std::cout << "Total: $" << std::fixed << std::setprecision(2) << getCartTotal(c) << "\n";
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