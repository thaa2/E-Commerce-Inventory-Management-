#ifndef PRODUCT_H
#define PRODUCT_H

#include "Models.h"
#include <vector>
#include <string>

// Load all products from data/products.csv
std::vector<Product> loadProducts();

// Save all products to data/products.csv
void saveProducts(const std::vector<Product>& products);

// Validate a product; if isNew, checks for duplicate ID
bool validateProduct(const Product& p, std::string& errorMsg,
                     const std::vector<Product>& existingProducts, bool isNew);

// Interactive CLI menu for Product CRUD
void productMenu();

#endif // PRODUCT_H
