#include "User.h"
#include "AuthManager.h"
#include "Customer.h"
#include <iostream>
#include <iomanip>
#include <limits>

const int MAX_LOGIN_ATTEMPTS = 3;

// Function to display login menu
void displayLoginMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "           LOGIN PAGE" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to display Admin Dashboard
void displayAdminDashboard(const std::string& username) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      🔐 ADMIN DASHBOARD" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Logged in as: " << username << " (Admin)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. View All Users" << std::endl;
    std::cout << "2. View All Customers" << std::endl;
    std::cout << "3. Add Customer" << std::endl;
    std::cout << "4. Search Customer" << std::endl;
    std::cout << "5. Add New User" << std::endl;
    std::cout << "6. Deactivate User" << std::endl;
    std::cout << "7. Logout" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to display User Dashboard
void displayUserDashboard(const std::string& username) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      👤 USER DASHBOARD" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Logged in as: " << username << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. View All Customers" << std::endl;
    std::cout << "2. Search Customer" << std::endl;
    std::cout << "3. Logout" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

// Login function
int login(AuthManager& auth) {
    std::string username, password;
    int attempts = 0;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "           LOGIN" << std::endl;
    std::cout << "========================================" << std::endl;
    
    while (attempts < MAX_LOGIN_ATTEMPTS) {
        std::cout << "\nLogin Attempt " << (attempts + 1) << " of " << MAX_LOGIN_ATTEMPTS << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        std::cout << "Username: ";
        std::getline(std::cin, username);
        
        std::cout << "Password: ";
        std::getline(std::cin, password);
        
        if (auth.login(username, password)) {
            std::cout << "\n✅ Login successful!" << std::endl;
            return 0;
        }
        
        attempts++;
        std::cout << "\n❌ Invalid username or password!" << std::endl;
        if (attempts < MAX_LOGIN_ATTEMPTS) {
            std::cout << "You have " << (MAX_LOGIN_ATTEMPTS - attempts) << " attempts remaining." << std::endl;
        }
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  ACCOUNT LOCKED!" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Too many failed login attempts." << std::endl;
    return -1;
}

// Admin functions
void addCustomer(CustomerBST& customers) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    Customer c;
    std::cout << "\n=== Add New Customer ===" << std::endl;
    std::cout << "Name: ";
    std::getline(std::cin, c.name);
    std::cout << "Email: ";
    std::getline(std::cin, c.email);
    std::cout << "Phone: ";
    std::getline(std::cin, c.phone);
    
    customers.insert(c);
    customers.saveToFile("data/customers.csv");
    std::cout << "✅ Customer added successfully!" << std::endl;
}

void searchCustomer(CustomerBST& customers) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string name;
    std::cout << "\n=== Search Customer ===" << std::endl;
    std::cout << "Enter name to search: ";
    std::getline(std::cin, name);
    
    CustomerNode* found = customers.search(name);
    if (found) {
        std::cout << "\n✅ Customer found:" << std::endl;
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(20) << "Name"
                  << std::setw(25) << "Email"
                  << "Phone" << std::endl;
        std::cout << std::string(55, '-') << std::endl;
        found->data.display();
    } else {
        std::cout << "\n❌ Customer not found!" << std::endl;
    }
}

void addUser(AuthManager& auth) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    User u;
    std::cout << "\n=== Add New User ===" << std::endl;
    std::cout << "Username: ";
    std::getline(std::cin, u.username);
    std::cout << "Password: ";
    std::string password;
    std::getline(std::cin, password);
    u.passwordHash = User::hashPassword(password);
    std::cout << "Role (Admin/Staff/Manager): ";
    std::getline(std::cin, u.role);
    u.isActive = true;
    
    if (auth.addUser(u)) {
        std::cout << "✅ User added successfully!" << std::endl;
    } else {
        std::cout << "❌ Failed to add user! Username may already exist." << std::endl;
    }
}

void deactivateUser(AuthManager& auth) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "\n=== Deactivate User ===" << std::endl;
    auth.displayAll();
    
    int id;
    std::cout << "\nEnter user ID to deactivate: ";
    std::cin >> id;
    
    if (auth.deactivateUser(id)) {
        std::cout << "✅ User deactivated successfully!" << std::endl;
    } else {
        std::cout << "❌ User not found!" << std::endl;
    }
}

// Admin Dashboard
void adminDashboard(AuthManager& auth, CustomerBST& customers) {
    bool running = true;
    std::string username = auth.currentName();
    
    while (running) {
        displayAdminDashboard(username);
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: // View All Users
                std::cout << "\n=== All Users ===" << std::endl;
                auth.displayAll();
                break;
                
            case 2: // View All Customers
                std::cout << "\n=== All Customers ===" << std::endl;
                customers.displayAll();
                break;
                
            case 3: // Add Customer
                addCustomer(customers);
                break;
                
            case 4: // Search Customer
                searchCustomer(customers);
                break;
                
            case 5: // Add New User
                addUser(auth);
                break;
                
            case 6: // Deactivate User
                deactivateUser(auth);
                break;
                
            case 7: // Logout
                std::cout << "\nLogging out..." << std::endl;
                auth.logout();
                running = false;
                break;
                
            default:
                std::cout << "\n❌ Invalid choice! Please try again." << std::endl;
                break;
        }
    }
}

// User Dashboard
void userDashboard(AuthManager& auth, CustomerBST& customers) {
    bool running = true;
    std::string username = auth.currentName();
    
    while (running) {
        displayUserDashboard(username);
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: // View All Customers
                std::cout << "\n=== All Customers ===" << std::endl;
                customers.displayAll();
                break;
                
            case 2: // Search Customer
                searchCustomer(customers);
                break;
                
            case 3: // Logout
                std::cout << "\nLogging out..." << std::endl;
                auth.logout();
                running = false;
                break;
                
            default:
                std::cout << "\n❌ Invalid choice! Please try again." << std::endl;
                break;
        }
    }
}

int main() {
    AuthManager auth;
    CustomerBST customers;
    
    std::cout << "========================================" << std::endl;
    std::cout << "  USER & CUSTOMER MANAGEMENT SYSTEM" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Load data from files
    std::cout << "\nLoading data..." << std::endl;
    
    // Load users
    auth.loadFromFile("data/users.csv");
    if (auth.getUserCount() == 0) {
        std::cout << "No users found. Creating default admin..." << std::endl;
        auth.seedAdmin();
    }
    std::cout << "Loaded " << auth.getUserCount() << " users" << std::endl;
    
    // Load customers
    customers.loadFromFile("data/customers.csv");
    std::cout << "Loaded customers" << std::endl;
    
    // Main program loop
    bool programRunning = true;
    
    while (programRunning) {
        displayLoginMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: // Login
                if (login(auth) == 0) {
                    // Check if user is admin
                    if (auth.isAdmin()) {
                        std::cout << "\n🔐 Welcome Admin!" << std::endl;
                        adminDashboard(auth, customers);
                    } else {
                        std::cout << "\n👤 Welcome User!" << std::endl;
                        userDashboard(auth, customers);
                    }
                }
                break;
                
            case 2: // Exit
                std::cout << "\nGoodbye!" << std::endl;
                programRunning = false;
                break;
                
            default:
                std::cout << "\n❌ Invalid choice! Please try again." << std::endl;
                break;
        }
    }
    
    // Save data before exit
    std::cout << "\nSaving data..." << std::endl;
    auth.saveToFile("data/users.csv");
    customers.saveToFile("data/customers.csv");
    std::cout << "Data saved successfully!" << std::endl;
    
    return 0;
}