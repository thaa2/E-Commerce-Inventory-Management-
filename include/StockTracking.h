#ifndef STOCK_TRACKING_H
#define STOCK_TRACKING_H

#include "Models.h"

// Print low-stock alerts for all products below their threshold
void checkStockAlerts();

// Display full inventory status table
void viewInventoryStatus();

// Interactive CLI menu for Stock Level Tracking
void stockTrackingMenu();

#endif // STOCK_TRACKING_H
