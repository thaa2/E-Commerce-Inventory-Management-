#include "Order.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ===========================================================
// Order — free functions
// ===========================================================

void initOrder(Order& o) {
    o.orderId      = 0;
    o.customerId   = 0;
    o.customerName = "";
    o.totalAmount  = 0.0;
    o.status       = "Pending";
    o.dateCreated  = "";
    o.itemCount    = 0;
}

bool addOrderItem(Order& o, const OrderItem& item) {
    if (o.itemCount >= MAX_ITEMS) {
        std::cout << "Order is full, cannot add more items.\n";
        return false;
    }
    o.items[o.itemCount] = item;
    o.itemCount++;
    recalculateOrderTotal(o);
    return true;
}

void recalculateOrderTotal(Order& o) {
    double sum = 0.0;
    for (int i = 0; i < o.itemCount; i++) {
        sum += o.items[i].quantity * o.items[i].unitPrice;
    }
    o.totalAmount = sum;
}

void displayOrder(const Order& o) {
    std::cout << "-------------------------------------------\n";
    std::cout << "Order #" << o.orderId
         << "  | Customer: " << o.customerName
         << "  | Status: " << o.status << "\n";
    std::cout << "Date: " << o.dateCreated << "\n";
    std::cout << std::left << std::setw(6) << "Qty" << std::setw(20) << "Product" << std::setw(10) << "Price" << "\n";
    for (int i = 0; i < o.itemCount; i++) {
        std::cout << std::left << std::setw(6) << o.items[i].quantity
             << std::setw(20) << o.items[i].productName
             << std::setw(10) << std::fixed << std::setprecision(2) << o.items[i].unitPrice << "\n";
    }
    std::cout << "Total: $" << std::fixed << std::setprecision(2) << o.totalAmount << "\n";
    std::cout << "-------------------------------------------\n";
}

std::string orderToCSV(const Order& o) {
    std::ostringstream oss;
    oss << o.orderId << "," << o.customerId << "," << o.customerName << ","
        << std::fixed << std::setprecision(2) << o.totalAmount << ","
        << o.status << "," << o.dateCreated << "," << o.itemCount;
    return oss.str();
}

Order orderFromCSV(const std::string& line) {
    Order o;
    initOrder(o);

    std::stringstream ss(line);
    std::string field;

    std::getline(ss, field, ','); o.orderId      = std::stoi(field);
    std::getline(ss, field, ','); o.customerId   = std::stoi(field);
    std::getline(ss, field, ','); o.customerName = field;
    std::getline(ss, field, ','); o.totalAmount  = std::stod(field);
    std::getline(ss, field, ','); o.status       = field;
    std::getline(ss, field, ','); o.dateCreated  = field;
    std::getline(ss, field, ','); o.itemCount    = field.empty() ? 0 : std::stoi(field);

    return o;
}

// ===========================================================
// OrderQueue — free functions
// ===========================================================

void initOrderQueue(OrderQueue& q) {
    q.front = 0;
    q.rear  = -1;
    q.count = 0;
}

bool isOrderQueueEmpty(const OrderQueue& q) {
    return q.count == 0;
}

bool isOrderQueueFull(const OrderQueue& q) {
    return q.count == MAX_QUEUE;
}

int orderQueueSize(const OrderQueue& q) {
    return q.count;
}

bool enqueueOrder(OrderQueue& q, const Order& o) {
    if (isOrderQueueFull(q)) {
        std::cout << "Order queue is full, cannot accept new order.\n";
        return false;
    }
    q.rear = (q.rear + 1) % MAX_QUEUE;
    q.items[q.rear] = o;
    q.count++;
    return true;
}

bool dequeueOrder(OrderQueue& q, Order& out) {
    if (isOrderQueueEmpty(q)) {
        return false;
    }
    out = q.items[q.front];
    q.front = (q.front + 1) % MAX_QUEUE;
    q.count--;
    return true;
}

bool peekOrder(const OrderQueue& q, Order& out) {
    if (isOrderQueueEmpty(q)) {
        return false;
    }
    out = q.items[q.front];
    return true;
}

void displayAllOrders(const OrderQueue& q) {
    if (isOrderQueueEmpty(q)) {
        std::cout << "No orders waiting in the queue.\n";
        return;
    }
    std::cout << "===== Orders in Queue (front -> rear) =====\n";
    int idx = q.front;
    for (int i = 0; i < q.count; i++) {
        displayOrder(q.items[idx]);
        idx = (idx + 1) % MAX_QUEUE;
    }
}