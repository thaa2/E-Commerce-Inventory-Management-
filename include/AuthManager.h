#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "User.h"
#include <string>

class AuthManager {
private:
    UserRecord users[MAX_USERS];
    int userCount;
    UserRecord currentUser;

public:
    AuthManager();

    bool login(const std::string& username, const std::string& password);
    void logout();

    std::string currentName() const;
    bool isAdmin() const;
    bool isAuthenticated() const;

    void displayAll() const;
    bool addUser(const UserRecord& u);
    bool deactivateUser(int id);
};

#endif // AUTHMANAGER_H
