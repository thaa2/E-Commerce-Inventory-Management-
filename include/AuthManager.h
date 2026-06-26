#pragma once
#include "User.h"
#include <string>

const int MAX_USERS = 50;

class AuthManager {
private:
    User users[MAX_USERS];
    int userCount;
    int nextId;
    int currentIndex;

public:
    AuthManager();

    void seedAdmin();
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool isLoggedIn() const;
    bool isAdmin() const;
    std::string currentName() const;

    bool addUser(User u);
    bool deactivateUser(int id);
    void displayAll() const;

    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;

    // Getter methods - defined inline in header
    int getUserCount() const { return userCount; }
};