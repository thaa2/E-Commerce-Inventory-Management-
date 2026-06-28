#include "../include/AuthManager.h"
#include "../include/FileManager.h"
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
    ensureFile(path);
    std::string lines[MAX_USERS + 10];
    int lineCount = readLines(path, lines, MAX_USERS + 10);
    if (lineCount <= 1) return;

    for (int i = 1; i < lineCount && userCount < MAX_USERS; i++) {
        if (lines[i].empty()) continue;
        users[userCount] = User::fromCSV(lines[i]);
        if (users[userCount].id >= nextId) nextId = users[userCount].id + 1;
        userCount++;
    }
}

void AuthManager::saveToFile(const std::string& path) const {
    std::string lines[MAX_USERS + 1];
    int lineCount = 0;
    lines[lineCount++] = "id,username,passwordHash,role,isActive";
    for (int i = 0; i < userCount; i++) {
        lines[lineCount++] = users[i].toCSV();
    }
    writeLines(path, lines, lineCount);
}