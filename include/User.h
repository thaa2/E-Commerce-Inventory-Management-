#pragma once
#include <string>

struct User{
    int id;
    std::string username;
    std::string passwordHash;
    std::string role;
    bool isActive;

    User();
    static std::string hashPassword(const std::string& pass);
    bool verifyPassword(const std::string& pass) const;
    std::string toCSV() const;
    static User fromCSV(const std::string& line);
};
