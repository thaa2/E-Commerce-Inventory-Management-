#ifndef ORDER_H
#define ORDER_H

#include <string>

const int MAX_QUEUE = 100;
const int MAX_ITEMS = 20;

// ---------------------------------------------------------
// OrderItem — plain data only, no functions attached
// ---------------------------------------------------------
struct OrderItem {
    int    productId;
    std::string productName;
    int    quantity;
    double unitPrice;
};

// ---------------------------------------------------------
// Order — plain data only. itemCount tracks how many slots
// in items[] are actually used.
// ---------------------------------------------------------
struct Order {
    int    orderId;
    int    customerId;
    std::string customerName;
    double totalAmount;
    std::string status;
    std::string dateCreated;

    OrderItem items[MAX_ITEMS];
    int    itemCount;
};

// ---------------------------------------------------------
// OrderQueue — plain data only. front/rear/count drive a
// circular array. No functions attached to the struct.
// ---------------------------------------------------------
struct OrderQueue {
    Order items[MAX_QUEUE];
    int   front;
    int   rear;
    int   count;
};

// ---------------------------------------------------------
// Free functions — Order
// ---------------------------------------------------------
void   initOrder(Order& o);                       // reset to defaults (replaces constructor)
bool   addOrderItem(Order& o, const OrderItem& item);
void   recalculateOrderTotal(Order& o);
void   displayOrder(const Order& o);
std::string orderToCSV(const Order& o);
Order  orderFromCSV(const std::string& line);

// ---------------------------------------------------------
// Free functions — OrderQueue
// ---------------------------------------------------------
void   initOrderQueue(OrderQueue& q);
bool   isOrderQueueEmpty(const OrderQueue& q);
bool   isOrderQueueFull(const OrderQueue& q);
int    orderQueueSize(const OrderQueue& q);
bool   enqueueOrder(OrderQueue& q, const Order& o);
bool   dequeueOrder(OrderQueue& q, Order& out);
bool   peekOrder(const OrderQueue& q, Order& out);
void   displayAllOrders(const OrderQueue& q);

#endif // ORDER_H