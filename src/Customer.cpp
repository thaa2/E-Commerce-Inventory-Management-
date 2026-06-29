#include "../include/Customer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Customer::Customer() { id = 0; }

void Customer::display() const {
    std::cout << std::left
              << std::setw(5)  << id
              << std::setw(20) << name
              << std::setw(25) << email
              << phone << "\n";
}

std::string Customer::toCSV() const {
    return std::to_string(id) + "," + name + "," + email + "," + phone;
}

std::string trim_customer(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

int safe_stoi_customer(const std::string& str) {
    std::string trimmed = trim_customer(str);
    if (trimmed.empty()) return 0;
    try {
        return std::stoi(trimmed);
    } catch (...) {
        return 0;
    }
}

Customer Customer::fromCSV(const std::string& line) {
    Customer c;
    std::stringstream ss(line);
    std::string token;
    int col = 0;
    
    while (std::getline(ss, token, ',')) {
        token = trim_customer(token);
        
        if (col == 0) c.id = safe_stoi_customer(token);
        else if (col == 1) c.name = token;
        else if (col == 2) c.email = token;
        else if (col == 3) c.phone = token;
        col++;
    }
    return c;
}

CustomerBST::CustomerBST() {
    root   = nullptr;
    count  = 0;
    nextId = 1;
}

CustomerBST::~CustomerBST() { destroy(root); }

void CustomerBST::destroy(CustomerNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void CustomerBST::insert(Customer c) {
    if (search(c.name) !=nullptr){
        std::cout<<"Customer "<<c.name<<" already exist!\n";
        return;
    }
    c.id   = nextId++;
    root   = insertHelper(root, c);
    count++;
}

CustomerNode* CustomerBST::insertHelper(CustomerNode* node, Customer c) {
    if (node == nullptr) {
        CustomerNode* n = new CustomerNode();
        n->data  = c;
        n->left  = nullptr;
        n->right = nullptr;
        return n;
    }
    if (c.name < node->data.name)
        node->left  = insertHelper(node->left,  c);
    else
        node->right = insertHelper(node->right, c);
    return node;
}

CustomerNode* CustomerBST::search(const std::string& name) const {
    CustomerNode* curr = root;
    while (curr != nullptr) {
        if (name == curr->data.name) return curr;
        if (name  < curr->data.name) curr = curr->left;
        else                         curr = curr->right;
    }
    return nullptr;
}

void CustomerBST::displayAll() const {
    if (root == nullptr) {
        std::cout << "  No customers.\n";
        return;
    }
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Name"
              << std::setw(25) << "Email" << "Phone\n";
    std::cout << std::string(55, '-') << "\n";
    inOrder(root);
}

void CustomerBST::inOrder(CustomerNode* node) const {
    if (node == nullptr) return;
    inOrder(node->left);
    node->data.display();
    inOrder(node->right);
}

void CustomerBST::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open " << path << std::endl;
        return;
    }
    
    std::string line;
    // Skip header
    if (std::getline(file, line)) {
        // Header line - do nothing
    }
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Check if line has at least 4 fields
        int commaCount = 0;
        for (char c : line) {
            if (c == ',') commaCount++;
        }
        if (commaCount < 3) continue; // Skip invalid lines
        
        Customer c = Customer::fromCSV(line);
        root = insertHelper(root, c);
        if (c.id >= nextId) nextId = c.id + 1;
        count++;
    }
    file.close();
}

void CustomerBST::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    file << "id,name,email,phone\n";
    saveInOrder(root, file);
}

void CustomerBST::saveInOrder(CustomerNode* node, std::ofstream& file) const {
    if (node == nullptr) return;
    saveInOrder(node->left, file);
    file << node->data.toCSV() << "\n";
    saveInOrder(node->right, file);
}