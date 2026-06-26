#include "../include/AuthManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>

AuthManager::AuthManager() {
    userCount = 0;
    nextId = 1;
    currentIndex = -1;
}

void AuthManager::seedAdmin() {
    if (userCount > 0) return;
    User u;
    u.id = nextId++;
    u.username = "admin";
    u.passwordHash = User::hashPassword("admin123");
    u.role = "Admin";
    u.isActive = true;
    users[userCount++] = u;
    saveToFile("data/users.csv");
    std::cout << "  Default admin created. user: admin  pass: admin123\n";
}

bool AuthManager::login(const std::string& username, const std::string& password) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username && users[i].isActive) {
            if (users[i].verifyPassword(password)) {
                currentIndex = i;
                return true;
            }
        }
    }
    return false;
}

void AuthManager::logout() { currentIndex = -1; }
bool AuthManager::isLoggedIn() const { return currentIndex != -1; }
bool AuthManager::isAdmin() const {
    return isLoggedIn() && users[currentIndex].role == "Admin";
}
std::string AuthManager::currentName() const {
    if (!isLoggedIn()) return "";
    return users[currentIndex].username;
}

bool AuthManager::addUser(User u) {
    if (userCount >= MAX_USERS) return false;
    for (int i = 0; i < userCount; i++)
        if (users[i].username == u.username) return false;
    u.id = nextId++;
    users[userCount++] = u;
    saveToFile("data/users.csv");
    return true;
}

bool AuthManager::deactivateUser(int id) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            users[i].isActive = false;
            saveToFile("data/users.csv");
            return true;
        }
    }
    return false;
}

void AuthManager::displayAll() const {
    std::cout << std::left << std::setw(5) << "ID" << std::setw(15) << "Username"
              << std::setw(10) << "Role" << "Active\n";
    std::cout << std::string(40, '-') << "\n";
    for (int i = 0; i < userCount; i++) {
        std::cout << std::setw(5)  << users[i].id
                  << std::setw(15) << users[i].username
                  << std::setw(10) << users[i].role
                  << (users[i].isActive ? "Yes" : "No") << "\n";
    }
}

void AuthManager::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open " << path << std::endl;
        return;
    }
    
    std::string line;
    // Skip header
    if (std::getline(file, line)) {
        // Header line - do nothing
    }
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        users[userCount] = User::fromCSV(line);
        if (users[userCount].id >= nextId) nextId = users[userCount].id + 1;
        userCount++;
    }
    file.close();
}

void AuthManager::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    file << "id,username,passwordHash,role,isActive\n";
    for (int i = 0; i < userCount; i++)
        file << users[i].toCSV() << "\n";
}