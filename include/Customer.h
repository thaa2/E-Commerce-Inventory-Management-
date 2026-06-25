#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <fstream>

struct Customer {
    int id;
    std::string name;
    std::string email;
    std::string phone;

    void display() const;
};

struct CustomerNode {
    Customer data;
    CustomerNode* left;
    CustomerNode* right;
};

class CustomerBST {
private:
    CustomerNode* root;
    int nextId;

    CustomerNode* insertHelper(CustomerNode* node, const Customer& c);
    CustomerNode* searchHelper(CustomerNode* node, const std::string& name);
    void displayAllHelper(CustomerNode* node) const;
    void saveToFileHelper(CustomerNode* node, std::ofstream& file) const;
    void destroyTree(CustomerNode* node);

public:
    CustomerBST();
    ~CustomerBST();

    void insert(Customer c);
    CustomerNode* search(const std::string& name);
    void displayAll() const;
    void saveToFile(const std::string& filepath) const;
    void loadFromFile(const std::string& filepath);
};

#endif // CUSTOMER_H
