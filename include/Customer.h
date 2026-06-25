#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Models.h"
#include <vector>
#include <string>

// Load all customers from data/customers.csv
std::vector<Customer> loadCustomers();

// Save all customers to data/customers.csv
void saveCustomers(const std::vector<Customer>& customers);

// Validate a customer; if isNew, checks for duplicate ID
bool validateCustomer(const Customer& c, std::string& errorMsg,
                      const std::vector<Customer>& existingCustomers, bool isNew);

// Interactive CLI menu for Customer CRUD
void customerMenu();

#endif // CUSTOMER_H
