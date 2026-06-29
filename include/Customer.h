#pragma once
#include <string>
#include <fstream>

struct Customer {
    int id;
    std::string name;
    std::string email;
    std::string phone;

    Customer();
    void display() const;
    std::string toCSV() const;
    static Customer fromCSV(const std::string& line);
};

struct CustomerNode {
    Customer data;
    CustomerNode* left;
    CustomerNode* right;
};

class CustomerBST {
private:
    CustomerNode* root;
    int count;
    int nextId;

    CustomerNode* insertHelper(CustomerNode* node, Customer c);
    void inOrder(CustomerNode* node) const;
    void saveInOrder(CustomerNode* node, std::ofstream& file) const;
    void destroy(CustomerNode* node);

public:
    CustomerBST();
    ~CustomerBST();

    void insert(Customer c);
    CustomerNode* search(const std::string& name) const;
    void displayAll() const;
    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;
};