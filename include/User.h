#ifndef USER_H
#define USER_H

#include <string>

const int MAX_USERS = 100;

enum Role { NONE = -1, ADMIN = 0, STAFF = 1, CUSTOMER = 2 };

struct UserRecord {
    std::string id;
    std::string name;
    std::string passwordHash;
    Role role;
};

std::string hashPassword(const std::string& password);
void UserCSV();
void loadCSVToArray(UserRecord userArray[], int& userCount);
int  signUp(UserRecord userArray[], int& userCount,
            const std::string& new_id, const std::string& new_name,
            const std::string& new_password);
UserRecord verUsr(UserRecord userArray[], int userCount,
                  const std::string& in_id, const std::string& in_password);

#endif // USER_H
