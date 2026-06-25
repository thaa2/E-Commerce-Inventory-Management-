#include "Product.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>

static const std::string PRODUCTS_FILE = "data/products.csv";

// ──────────────────────────────────────────────
// Internal CSV helpers
// ──────────────────────────────────────────────
static std::string escapeCSVField(const std::string& str) {
    if (str.find(',') != std::string::npos || str.find('"') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : str) {
            if (c == '"') escaped += "\"\"";
            else          escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return str;
}

static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::string current;
    bool inQuotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                current += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            result.push_back(trim(current));
            current.clear();
        } else {
            current += c;
        }
    }
    result.push_back(trim(current));
    return result;
}

static bool tryParseProductLine(const std::string& line, Product& out) {
    auto fields = parseCSVLine(line);
    if (fields.size() < 5) return false;
    try {
        out.id               = fields[0];
        out.name             = fields[1];
        out.price            = std::stod(fields[2]);
        out.quantity         = std::stoi(fields[3]);
        out.lowStockThreshold = std::stoi(fields[4]);
        return !out.id.empty();
    } catch (...) {
        return false;
    }
}

// ──────────────────────────────────────────────
// Public interface
// ──────────────────────────────────────────────
std::vector<Product> loadProducts() {
    std::vector<Product> products;
    std::ifstream file(PRODUCTS_FILE);
    if (!file.is_open()) return products;

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // Skip header row (contains "ID" or "Name")
        if (firstLine) {
            firstLine = false;
            if (line.find("ID") != std::string::npos ||
                line.find("Name") != std::string::npos) continue;
        }
        Product p;
        if (tryParseProductLine(line, p)) products.push_back(p);
    }
    file.close();
    return products;
}

void saveProducts(const std::vector<Product>& products) {
    std::ofstream file(PRODUCTS_FILE);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open products file for writing.\n";
        return;
    }
    file << "ID,Name,Price,Quantity,LowStockThreshold\n";
    for (const auto& p : products) {
        file << escapeCSVField(p.id)   << ","
             << escapeCSVField(p.name) << ","
             << std::fixed << std::setprecision(2) << p.price << ","
             << p.quantity << ","
             << p.lowStockThreshold   << "\n";
    }
    file.close();
}

bool validateProduct(const Product& p, std::string& errorMsg,
                     const std::vector<Product>& existingProducts, bool isNew) {
    if (trim(p.id).empty())   { errorMsg = "Product ID cannot be empty.";   return false; }
    if (trim(p.name).empty()) { errorMsg = "Product Name cannot be empty."; return false; }
    if (p.price < 0)          { errorMsg = "Price cannot be negative.";     return false; }
    if (p.quantity < 0)       { errorMsg = "Quantity cannot be negative.";  return false; }
    if (p.lowStockThreshold < 0) {
        errorMsg = "Low stock threshold cannot be negative."; return false;
    }
    if (isNew) {
        for (const auto& ep : existingProducts) {
            if (ep.id == p.id) {
                errorMsg = "Product ID '" + p.id + "' already exists.";
                return false;
            }
        }
    }
    return true;
}

void productMenu() {
    while (true) {
        std::vector<Product> products = loadProducts();

        std::cout << "\n====================================\n";
        std::cout << "      PRODUCT MANAGEMENT (CRUD)     \n";
        std::cout << "====================================\n";
        std::cout << "1. Add New Product\n";
        std::cout << "2. View All Products\n";
        std::cout << "3. Search Product by ID\n";
        std::cout << "4. Update Product\n";
        std::cout << "5. Delete Product\n";
        std::cout << "6. Back to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                Product p;
                std::cout << "\n--- Add New Product ---\n";
                std::cout << "Enter Product ID: ";
                std::getline(std::cin, p.id);
                p.id = trim(p.id);

                std::cout << "Enter Product Name: ";
                std::getline(std::cin, p.name);
                p.name = trim(p.name);

                std::cout << "Enter Price ($): ";
                while (!(std::cin >> p.price)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid price. Enter a number: ";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter Quantity: ";
                while (!(std::cin >> p.quantity)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid quantity. Enter an integer: ";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter Low Stock Threshold: ";
                while (!(std::cin >> p.lowStockThreshold)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid threshold. Enter an integer: ";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string errorMsg;
                if (validateProduct(p, errorMsg, products, true)) {
                    products.push_back(p);
                    saveProducts(products);
                    std::cout << "\n[Success] Product added successfully!\n";
                } else {
                    std::cout << "\n[Error] " << errorMsg << "\n";
                }
                break;
            }
            case 2: {
                std::cout << "\n--- All Products ---\n";
                if (products.empty()) {
                    std::cout << "No products available.\n";
                } else {
                    std::cout << std::left
                              << std::setw(12) << "ID"
                              << std::setw(25) << "Name"
                              << std::setw(12) << "Price ($)"
                              << std::setw(10) << "Stock"
                              << "Low Threshold\n";
                    std::cout << std::string(70, '-') << "\n";
                    for (const auto& p : products) {
                        std::cout << std::left
                                  << std::setw(12) << p.id
                                  << std::setw(25) << p.name
                                  << std::setw(12) << std::fixed << std::setprecision(2) << p.price
                                  << std::setw(10) << p.quantity
                                  << p.lowStockThreshold << "\n";
                    }
                }
                break;
            }
            case 3: {
                std::cout << "\n--- Search Product ---\n";
                std::cout << "Enter Product ID: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(products.begin(), products.end(),
                    [&](const Product& p) { return p.id == targetId; });

                if (it != products.end()) {
                    std::cout << "\nProduct Found:\n";
                    std::cout << "  ID:            " << it->id   << "\n";
                    std::cout << "  Name:          " << it->name  << "\n";
                    std::cout << "  Price:         $" << std::fixed << std::setprecision(2) << it->price << "\n";
                    std::cout << "  Quantity:      " << it->quantity << "\n";
                    std::cout << "  Low Threshold: " << it->lowStockThreshold << "\n";
                } else {
                    std::cout << "Product with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            case 4: {
                std::cout << "\n--- Update Product ---\n";
                std::cout << "Enter Product ID to update: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(products.begin(), products.end(),
                    [&](const Product& p) { return p.id == targetId; });

                if (it != products.end()) {
                    Product updated = *it;
                    std::cout << "Current: " << it->name
                              << " | $" << it->price
                              << " | Qty: " << it->quantity
                              << " | Threshold: " << it->lowStockThreshold << "\n";

                    std::cout << "New Name (leave empty to keep): ";
                    std::string input;
                    std::getline(std::cin, input);
                    if (!trim(input).empty()) updated.name = trim(input);

                    std::cout << "New Price (-1 to keep): ";
                    double newPrice;
                    if (std::cin >> newPrice && newPrice >= 0) updated.price = newPrice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "New Quantity (-1 to keep): ";
                    int newQty;
                    if (std::cin >> newQty && newQty >= 0) updated.quantity = newQty;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "New Low Threshold (-1 to keep): ";
                    int newThresh;
                    if (std::cin >> newThresh && newThresh >= 0) updated.lowStockThreshold = newThresh;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::string errorMsg;
                    if (validateProduct(updated, errorMsg, products, false)) {
                        *it = updated;
                        saveProducts(products);
                        std::cout << "\n[Success] Product updated successfully!\n";
                    } else {
                        std::cout << "\n[Error] " << errorMsg << "\n";
                    }
                } else {
                    std::cout << "Product with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            case 5: {
                std::cout << "\n--- Delete Product ---\n";
                std::cout << "Enter Product ID to delete: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(products.begin(), products.end(),
                    [&](const Product& p) { return p.id == targetId; });

                if (it != products.end()) {
                    std::cout << "Are you sure you want to delete '"
                              << it->name << "'? (y/n): ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (confirm == 'y' || confirm == 'Y') {
                        products.erase(it);
                        saveProducts(products);
                        std::cout << "\n[Success] Product deleted successfully!\n";
                    } else {
                        std::cout << "Deletion cancelled.\n";
                    }
                } else {
                    std::cout << "Product with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please choose between 1 and 6.\n";
        }
    }
}
