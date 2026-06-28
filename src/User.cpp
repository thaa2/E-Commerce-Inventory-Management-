#include "../include/User.h"
#include "../include/FileManager.h"
#include <sstream>
#include <string>
#include <cctype>

User::User() { id = 0; isActive = true; }

std::string User::hashPassword(const std::string& pass) {
    unsigned long h = 5381;
    for (char c : pass)
        h = h * 33 + (unsigned char)c;
    return std::to_string(h);
}

bool User::verifyPassword(const std::string& pass) const {
    return passwordHash == hashPassword(pass);
}

std::string User::toCSV() const {
    return std::to_string(id)   + "," +
           username        + "," +
           passwordHash    + "," +
           role            + "," +
           (isActive ? "1" : "0");
}

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

User User::fromCSV(const std::string& line) {
    User u;
    std::stringstream ss(line);
    std::string token;
    int col = 0;
    
    while (std::getline(ss, token, ',')) {
        token = trim(token);
        
        if (col == 0) {
            try { u.id = std::stoi(token); } 
            catch(...) { u.id = 0; }
        }
        else if (col == 1) {
            u.username = token;
        }
        else if (col == 2) {
            u.passwordHash = token;
        }
        else if (col == 3) {
            // Handle both number and string role
            if (token == "Admin" || token == "0") {
                u.role = "Admin";
            } else if (token == "Staff" || token == "1") {
                u.role = "Staff";
            } else if (token == "Manager" || token == "2") {
                u.role = "Manager";
            } else {
                u.role = "User";
            }
        }
        else if (col == 4) {
            u.isActive = (token == "1");
        }
        col++;
    }
    return u;
}