#include "User.h"
#include <iostream>
#include <fstream>

using namespace std;

static const string USERS_CSV_PATH = "data/users.csv";

string hashPassword(const string& password) {
    unsigned int hashValue = 0;
    for (char c : password) {
        hashValue = (hashValue * 31 + c) % 10007;
    }
    return to_string(hashValue);
}

void UserCSV() {
    bool needCreate = true;
    ifstream check(USERS_CSV_PATH, ios::binary);
    if (check.is_open()) {
        check.seekg(0, ios::end);
        if (check.tellg() > 0) needCreate = false;
        check.close();
    }

    if (!needCreate) return;

    ofstream w(USERS_CSV_PATH);
    if (!w.is_open()) return;

    w << "ID,Name,PasswordHash,RoleNum\n"; 
    w << "001,phadeth," << hashPassword("deth123")  << ",0\n";
    w << "002,sokhour," << hashPassword("hour123") << ",0\n";
    w << "003,setha,"   << hashPassword("setha123") << ",1\n"; 
    w << "004,sela,"    << hashPassword("sela123")  << ",2\n";
    w.close();
}

void loadCSVToArray(UserRecord userArray[], int& userCount) {
    ifstream r(USERS_CSV_PATH);
    if (!r.is_open()) return;

    string line;
    userCount = 0;
    getline(r, line);

    while (getline(r, line) && userCount < MAX_USERS) {
        int Fcomma = line.find(',');
        if (Fcomma == string::npos) continue;
        string Id = line.substr(0, Fcomma);

        int Scomma = line.find(',', Fcomma + 1);
        if (Scomma == string::npos) continue;
        string name = line.substr(Fcomma + 1, Scomma - (Fcomma + 1)); 

        int Tcomma = line.find(',', Scomma + 1);
        if (Tcomma == string::npos) continue;
        string hashpass = line.substr(Scomma + 1, Tcomma - (Scomma + 1));
        
        string roleStr = line.substr(Tcomma + 1); 

        Role rowRole = NONE;
        if (roleStr == "0")      rowRole = ADMIN;
        else if (roleStr == "1") rowRole = STAFF;
        else if (roleStr == "2") rowRole = CUSTOMER;

        userArray[userCount] = {Id, name, hashpass, rowRole};
        userCount++;
    }
    r.close();
}

int signUp(UserRecord userArray[], int& userCount, const string& new_id, const string& new_name, const string& new_password) {
    if (userCount >= MAX_USERS) {
        cout << "System local array capacity full!" << endl;
        return 0;
    }

    for (int i = 0; i < userCount; i++) {
        if (userArray[i].id == new_id) {
            cout << "User ID '" << new_id << "' already exists!" << endl;
            return 0;
        }
    }

    ofstream nw(USERS_CSV_PATH, ios::app); 
    if (!nw.is_open()) return 0;

    string newHash = hashPassword(new_password);
    nw << new_id << "," << new_name << "," << newHash << ",2\n";
    nw.close();

    userArray[userCount] = {new_id, new_name, newHash, CUSTOMER};
    userCount++;

    return 1;
}

UserRecord verUsr(UserRecord userArray[], int userCount, const string& in_id, const string& in_password) {
    string in_hash = hashPassword(in_password);
    int idFoundIndex = -1; 

    for (int i = 0; i < userCount; i++) {
        if (userArray[i].id == in_id) {
            idFoundIndex = i; 
            break; 
        }
    }

        if (idFoundIndex == -1) {
        cout << "The ID '" << in_id << "' does not exist in our database!" << endl;
        return {"", "", "", NONE};
    }

    if (userArray[idFoundIndex].passwordHash != in_hash) {
        cout << "Incorrect password for user profile '" << userArray[idFoundIndex].name << "'!" << endl;
        return {"", "", "", NONE};
    }

    cout << "Login Successful! Welcome, " << userArray[idFoundIndex].name << "." << endl;
    return userArray[idFoundIndex];
}