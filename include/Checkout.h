#ifndef CHECKOUT_H
#define CHECKOUT_H

#include "Models.h"
#include <vector>

// Load all orders from data/orders.csv
std::vector<Order> loadOrders();

// Save all orders to data/orders.csv
void saveOrders(const std::vector<Order>& orders);

// Interactive CLI menu for the checkout process
void checkoutMenu();

#endif // CHECKOUT_H
