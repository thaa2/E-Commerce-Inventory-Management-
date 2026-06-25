#include "../include/AuthManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>

AuthManager::AuthManager() {
    UserCSV();
    loadCSVToArray(users, userCount);
    currentUser = {"", "", "", NONE};
}

bool AuthManager::login(const std::string& username, const std::string& password) {
    std::string in_hash = hashPassword(password);
    for (int i = 0; i < userCount; i++) {
        if (users[i].name == username && users[i].passwordHash == in_hash) {
            currentUser = users[i];
            return true;
        }
    }
    return false;
}

void AuthManager::logout() {
    currentUser = {"", "", "", NONE};
}

std::string AuthManager::currentName() const {
    return currentUser.name;
}

bool AuthManager::isAdmin() const {
    return currentUser.role == ADMIN;
}

bool AuthManager::isAuthenticated() const {
    return currentUser.role != NONE;
}

void AuthManager::displayAll() const {
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(20) << "Username"
              << "Role\n";
    std::cout << std::string(40, '-') << "\n";
    for (int i = 0; i < userCount; i++) {
        std::string roleName = "Customer";
        if (users[i].role == ADMIN) roleName = "Admin";
        else if (users[i].role == STAFF) roleName = "Staff";

        std::cout << std::left
                  << std::setw(10) << users[i].id
                  << std::setw(20) << users[i].name
                  << roleName << "\n";
    }
}

bool AuthManager::addUser(const UserRecord& u) {
    if (userCount >= MAX_USERS) return false;
    for (int i = 0; i < userCount; i++) {
        if (users[i].name == u.name || users[i].id == u.id) {
            return false;
        }
    }

    users[userCount] = u;
    userCount++;

    // Write all to CSV
    std::ofstream nw("data/users.csv");
    if (!nw.is_open()) return false;
    nw << "ID,Name,PasswordHash,RoleNum\n";
    for (int i = 0; i < userCount; i++) {
        int rNum = 2; // CUSTOMER
        if (users[i].role == ADMIN) rNum = 0;
        else if (users[i].role == STAFF) rNum = 1;
        nw << users[i].id << "," << users[i].name << "," << users[i].passwordHash << "," << rNum << "\n";
    }
    nw.close();
    return true;
}

bool AuthManager::deactivateUser(int id) {
    std::string idStr = std::to_string(id);
    if (id < 10) idStr = "00" + idStr;
    else if (id < 100) idStr = "0" + idStr;

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == idStr) {
            // Delete/deactivate user by removing from array or changing role
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;

            // Save back to CSV
            std::ofstream nw("data/users.csv");
            if (nw.is_open()) {
                nw << "ID,Name,PasswordHash,RoleNum\n";
                for (int k = 0; k < userCount; k++) {
                    int rNum = 2;
                    if (users[k].role == ADMIN) rNum = 0;
                    else if (users[k].role == STAFF) rNum = 1;
                    nw << users[k].id << "," << users[k].name << "," << users[k].passwordHash << "," << rNum << "\n";
                }
                nw.close();
            }
            return true;
        }
    }
    return false;
}
