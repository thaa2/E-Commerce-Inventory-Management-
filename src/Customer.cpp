#include "../include/Customer.h"
#include <iostream>
#include <sstream>
#include <vector>

void Customer::display() const {
    std::cout << "ID: " << id << " | Name: " << name << " | Email: " << email << " | Phone: " << phone << "\n";
}

CustomerBST::CustomerBST() {
    root = nullptr;
    nextId = 1;
}

CustomerBST::~CustomerBST() {
    destroyTree(root);
}

void CustomerBST::destroyTree(CustomerNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

CustomerNode* CustomerBST::insertHelper(CustomerNode* node, const Customer& c) {
    if (node == nullptr) {
        CustomerNode* n = new CustomerNode{c, nullptr, nullptr};
        if (n->data.id <= 0) {
            n->data.id = nextId++;
        } else {
            if (n->data.id >= nextId) {
                nextId = n->data.id + 1;
            }
        }
        return n;
    }
    if (c.name < node->data.name) {
        node->left = insertHelper(node->left, c);
    } else {
        node->right = insertHelper(node->right, c);
    }
    return node;
}

void CustomerBST::insert(Customer c) {
    root = insertHelper(root, c);
}

CustomerNode* CustomerBST::searchHelper(CustomerNode* node, const std::string& name) {
    if (node == nullptr || node->data.name == name) {
        return node;
    }
    if (name < node->data.name) {
        return searchHelper(node->left, name);
    }
    return searchHelper(node->right, name);
}

CustomerNode* CustomerBST::search(const std::string& name) {
    return searchHelper(root, name);
}

void CustomerBST::displayAllHelper(CustomerNode* node) const {
    if (node != nullptr) {
        displayAllHelper(node->left);
        std::cout << "ID: " << node->data.id 
                  << " | Name: " << node->data.name 
                  << " | Email: " << node->data.email 
                  << " | Phone: " << node->data.phone << "\n";
        displayAllHelper(node->right);
    }
}

void CustomerBST::displayAll() const {
    if (root == nullptr) {
        std::cout << "No customers registered.\n";
        return;
    }
    displayAllHelper(root);
}

void CustomerBST::saveToFileHelper(CustomerNode* node, std::ofstream& file) const {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->data.id << "," << node->data.name << "," 
             << node->data.email << "," << node->data.phone << "\n";
        saveToFileHelper(node->right, file);
    }
}

void CustomerBST::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) return;
    file << "id,name,email,phone\n";
    saveToFileHelper(root, file);
    file.close();
}

void CustomerBST::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string fId, fName, fEmail, fPhone;
        
        std::getline(ss, fId, ',');
        std::getline(ss, fName, ',');
        std::getline(ss, fEmail, ',');
        std::getline(ss, fPhone, ',');

        if (!fName.empty()) {
            Customer c;
            try {
                c.id = std::stoi(fId);
            } catch (...) {
                c.id = 0;
            }
            c.name = fName;
            c.email = fEmail;
            c.phone = fPhone;
            insert(c);
        }
    }
    file.close();
}
