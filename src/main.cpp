#include "User.h"
#include "MenuManager.h"
#include <iostream>
#include <limits>

int main() {
    std::cout << "==================================================\n";
    std::cout << "        E-COMMERCE MANAGEMENT SYSTEM CLI          \n";
    std::cout << "==================================================\n";

    // Initialise the users CSV with default accounts if it doesn't exist
    UserCSV();

    UserRecord userArray[MAX_USERS];
    int userCount = 0;
    loadCSVToArray(userArray, userCount);

    // ── Login loop ──────────────────────────────────
    UserRecord loggedIn = {"", "", "", NONE};
    while (loggedIn.role == NONE) {
        std::cout << "\n--- Login ---\n";
        std::cout << "User ID  : ";
        std::string id;
        std::getline(std::cin, id);

        std::cout << "Password : ";
        std::string password;
        std::getline(std::cin, password);

        loggedIn = verUsr(userArray, userCount, id, password);
    }

    // Show role
    std::cout << "\nWelcome, " << loggedIn.name << "!  Role: ";
    switch (loggedIn.role) {
        case ADMIN:    std::cout << "Admin\n";    break;
        case STAFF:    std::cout << "Staff\n";    break;
        case CUSTOMER: std::cout << "Customer\n"; break;
        default:       std::cout << "Unknown\n";  break;
    }

    // ── Main application ─────────────────────────────
    runMainMenu();

    return 0;
}