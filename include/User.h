#ifndef USER_H
#define USER_H

#include <string>

// ---------------------------------------------------------
// User — plain data only
// ---------------------------------------------------------
struct User {
    int    id;
    std::string username;
    std::string passwordHash;
    std::string role;        // "Admin" or "Staff"
    bool   isActive;
};

void        initUser(User& u);                                   // reset to defaults
std::string hashPassword(const std::string& password);            // djb2-style hash
bool        verifyUserPassword(const User& u, const std::string& inputPassword);
std::string userToCSV(const User& u);
User        userFromCSV(const std::string& line);

#endif // USER_H