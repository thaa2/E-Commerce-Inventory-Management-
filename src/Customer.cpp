#include "Customer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>

static const std::string CUSTOMERS_FILE = "data/customers.csv";

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

// ──────────────────────────────────────────────
// Public interface
// ──────────────────────────────────────────────
std::vector<Customer> loadCustomers() {
    std::vector<Customer> customers;
    std::ifstream file(CUSTOMERS_FILE);
    if (!file.is_open()) return customers;

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // Skip header row
        if (firstLine) {
            firstLine = false;
            if (line.find("ID") != std::string::npos ||
                line.find("Name") != std::string::npos) continue;
        }
        auto fields = parseCSVLine(line);
        if (fields.size() >= 4) {
            Customer c;
            c.id    = fields[0];
            c.name  = fields[1];
            c.email = fields[2];
            c.phone = fields[3];
            if (!c.id.empty()) customers.push_back(c);
        }
    }
    file.close();
    return customers;
}

void saveCustomers(const std::vector<Customer>& customers) {
    std::ofstream file(CUSTOMERS_FILE);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not open customers file for writing.\n";
        return;
    }
    file << "ID,Name,Email,Phone\n";
    for (const auto& c : customers) {
        file << escapeCSVField(c.id)    << ","
             << escapeCSVField(c.name)  << ","
             << escapeCSVField(c.email) << ","
             << escapeCSVField(c.phone) << "\n";
    }
    file.close();
}

bool validateCustomer(const Customer& c, std::string& errorMsg,
                      const std::vector<Customer>& existingCustomers, bool isNew) {
    if (trim(c.id).empty())    { errorMsg = "Customer ID cannot be empty.";    return false; }
    if (trim(c.name).empty())  { errorMsg = "Customer Name cannot be empty.";  return false; }
    if (trim(c.phone).empty()) { errorMsg = "Customer Phone cannot be empty."; return false; }

    std::string email = trim(c.email);
    if (email.empty()) { errorMsg = "Customer Email cannot be empty."; return false; }
    size_t atPos  = email.find('@');
    size_t dotPos = email.find('.', atPos);
    if (atPos == std::string::npos || dotPos == std::string::npos ||
        atPos == 0 || dotPos == atPos + 1 || dotPos == email.length() - 1) {
        errorMsg = "Invalid email format (e.g. user@example.com).";
        return false;
    }

    if (isNew) {
        for (const auto& ec : existingCustomers) {
            if (ec.id == c.id) {
                errorMsg = "Customer ID '" + c.id + "' already exists.";
                return false;
            }
        }
    }
    return true;
}

void customerMenu() {
    while (true) {
        std::vector<Customer> customers = loadCustomers();

        std::cout << "\n====================================\n";
        std::cout << "      CUSTOMER MANAGEMENT (CRUD)    \n";
        std::cout << "====================================\n";
        std::cout << "1. Add New Customer\n";
        std::cout << "2. View All Customers\n";
        std::cout << "3. Search Customer by ID\n";
        std::cout << "4. Update Customer\n";
        std::cout << "5. Delete Customer\n";
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
                Customer c;
                std::cout << "\n--- Add New Customer ---\n";
                std::cout << "Enter Customer ID: ";
                std::getline(std::cin, c.id);
                c.id = trim(c.id);

                std::cout << "Enter Customer Name: ";
                std::getline(std::cin, c.name);
                c.name = trim(c.name);

                std::cout << "Enter Customer Email: ";
                std::getline(std::cin, c.email);
                c.email = trim(c.email);

                std::cout << "Enter Customer Phone: ";
                std::getline(std::cin, c.phone);
                c.phone = trim(c.phone);

                std::string errorMsg;
                if (validateCustomer(c, errorMsg, customers, true)) {
                    customers.push_back(c);
                    saveCustomers(customers);
                    std::cout << "\n[Success] Customer registered successfully!\n";
                } else {
                    std::cout << "\n[Error] " << errorMsg << "\n";
                }
                break;
            }
            case 2: {
                std::cout << "\n--- All Customers ---\n";
                if (customers.empty()) {
                    std::cout << "No customers registered.\n";
                } else {
                    std::cout << std::left
                              << std::setw(15) << "ID"
                              << std::setw(25) << "Name"
                              << std::setw(30) << "Email"
                              << "Phone\n";
                    std::cout << std::string(80, '-') << "\n";
                    for (const auto& c : customers) {
                        std::cout << std::left
                                  << std::setw(15) << c.id
                                  << std::setw(25) << c.name
                                  << std::setw(30) << c.email
                                  << c.phone << "\n";
                    }
                }
                break;
            }
            case 3: {
                std::cout << "\n--- Search Customer ---\n";
                std::cout << "Enter Customer ID: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(customers.begin(), customers.end(),
                    [&](const Customer& c) { return c.id == targetId; });

                if (it != customers.end()) {
                    std::cout << "\nCustomer Found:\n";
                    std::cout << "  ID:    " << it->id    << "\n";
                    std::cout << "  Name:  " << it->name  << "\n";
                    std::cout << "  Email: " << it->email << "\n";
                    std::cout << "  Phone: " << it->phone << "\n";
                } else {
                    std::cout << "Customer with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            case 4: {
                std::cout << "\n--- Update Customer ---\n";
                std::cout << "Enter Customer ID to update: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(customers.begin(), customers.end(),
                    [&](const Customer& c) { return c.id == targetId; });

                if (it != customers.end()) {
                    Customer updated = *it;
                    std::cout << "Current: " << it->name
                              << " | " << it->email
                              << " | " << it->phone << "\n";

                    std::cout << "New Name (leave empty to keep): ";
                    std::string input;
                    std::getline(std::cin, input);
                    if (!trim(input).empty()) updated.name = trim(input);

                    std::cout << "New Email (leave empty to keep): ";
                    std::getline(std::cin, input);
                    if (!trim(input).empty()) updated.email = trim(input);

                    std::cout << "New Phone (leave empty to keep): ";
                    std::getline(std::cin, input);
                    if (!trim(input).empty()) updated.phone = trim(input);

                    std::string errorMsg;
                    if (validateCustomer(updated, errorMsg, customers, false)) {
                        *it = updated;
                        saveCustomers(customers);
                        std::cout << "\n[Success] Customer updated successfully!\n";
                    } else {
                        std::cout << "\n[Error] " << errorMsg << "\n";
                    }
                } else {
                    std::cout << "Customer with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            case 5: {
                std::cout << "\n--- Delete Customer ---\n";
                std::cout << "Enter Customer ID to delete: ";
                std::string targetId;
                std::getline(std::cin, targetId);
                targetId = trim(targetId);

                auto it = std::find_if(customers.begin(), customers.end(),
                    [&](const Customer& c) { return c.id == targetId; });

                if (it != customers.end()) {
                    std::cout << "Are you sure you want to delete '"
                              << it->name << "'? (y/n): ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (confirm == 'y' || confirm == 'Y') {
                        customers.erase(it);
                        saveCustomers(customers);
                        std::cout << "\n[Success] Customer deleted successfully!\n";
                    } else {
                        std::cout << "Deletion cancelled.\n";
                    }
                } else {
                    std::cout << "Customer with ID '" << targetId << "' not found.\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please choose between 1 and 6.\n";
        }
    }
}
