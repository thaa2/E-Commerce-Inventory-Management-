#include "../include/Inventory.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

Inventory::Inventory() {
    head = nullptr;
    count = 0;
    nextId = 1;
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = nullptr;
    }
}

Inventory::~Inventory() {
    ProductNode* curr = head;
    while (curr != nullptr) {
        ProductNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* hCurr = hashTable[i];
        while (hCurr != nullptr) {
            HashNode* hNext = hCurr->next;
            delete hCurr;
            hCurr = hNext;
        }
    }
}

int Inventory::hashFunction(const std::string& key) const {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % HASH_SIZE;
    }
    if (hash < 0) hash += HASH_SIZE;
    return hash;
}

void Inventory::insertHash(const std::string& key, int val) {
    int index = hashFunction(key);
    HashNode* n = new HashNode{key, val, hashTable[index]};
    hashTable[index] = n;
}

void Inventory::removeHash(const std::string& key) {
    int index = hashFunction(key);
    HashNode* curr = hashTable[index];
    HashNode* prev = nullptr;
    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) {
                hashTable[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int Inventory::hashSearch(const std::string& sku) {
    int index = hashFunction(sku);
    HashNode* curr = hashTable[index];
    while (curr != nullptr) {
        if (curr->key == sku) {
            return curr->value;
        }
        curr = curr->next;
    }
    return -1;
}

void Inventory::addProduct(Product p) {
    ProductNode* n = new ProductNode();
    n->data = p;
    if (n->data.id <= 0) {
        n->data.id = nextId++;
    } else {
        if (n->data.id >= nextId) {
            nextId = n->data.id + 1;
        }
    }
    n->next = nullptr;

    if (head == nullptr) {
        head = n;
    } else {
        ProductNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = n;
    }
    count++;
    insertHash(n->data.sku, n->data.id);
}

bool Inventory::removeProduct(int id) {
    if (head == nullptr) return false;

    if (head->data.id == id) {
        ProductNode* oldHead = head;
        removeHash(oldHead->data.sku);
        head = head->next;
        delete oldHead;
        count--;
        return true;
    }

    ProductNode* prev = head;
    while (prev->next != nullptr) {
        if (prev->next->data.id == id) {
            ProductNode* target = prev->next;
            removeHash(target->data.sku);
            prev->next = target->next;
            delete target;
            count--;
            return true;
        }
        prev = prev->next;
    }
    return false;
}

Product* Inventory::findById(int id) {
    ProductNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.id == id) {
            return &curr->data;
        }
        curr = curr->next;
    }
    return nullptr;
}

void Inventory::displayAll() {
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(15) << "SKU"
              << std::setw(25) << "Name"
              << std::setw(15) << "Category"
              << std::setw(10) << "Price"
              << std::setw(10) << "Quantity"
              << std::setw(10) << "Reorder" << "\n";
    std::cout << std::string(91, '-') << "\n";

    ProductNode* curr = head;
    while (curr != nullptr) {
        curr->data.display();
        curr = curr->next;
    }
}

void Inventory::displayLowStock() {
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(15) << "SKU"
              << std::setw(25) << "Name"
              << std::setw(15) << "Category"
              << std::setw(10) << "Price"
              << std::setw(10) << "Quantity"
              << std::setw(10) << "Reorder" << "\n";
    std::cout << std::string(91, '-') << "\n";

    ProductNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.isLowStock()) {
            curr->data.display();
        }
        curr = curr->next;
    }
}

int Inventory::copyToArray(Product arr[], int maxLimit) {
    int i = 0;
    ProductNode* curr = head;
    while (curr != nullptr && i < maxLimit) {
        arr[i++] = curr->data;
        curr = curr->next;
    }
    return i;
}

void Inventory::sortByPrice(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].price < arr[minIdx].price) {
                minIdx = j;
            }
        }
        Product temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }
}

void Inventory::sortByName(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].name < arr[minIdx].name) {
                minIdx = j;
            }
        }
        Product temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }
}

void Inventory::sortByQuantity(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].quantity < arr[minIdx].quantity) {
                minIdx = j;
            }
        }
        Product temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }
}

bool Inventory::adjustStock(int id, int amount) {
    Product* p = findById(id);
    if (p != nullptr) {
        p->quantity += amount;
        if (p->quantity < 0) p->quantity = 0;
        return true;
    }
    return false;
}

double Inventory::totalValue() {
    double total = 0.0;
    ProductNode* curr = head;
    while (curr != nullptr) {
        total += curr->data.price * curr->data.quantity;
        curr = curr->next;
    }
    return total;
}

void Inventory::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Product p = Product::fromCSV(line);
        addProduct(p);
    }
    file.close();
}

void Inventory::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) return;

    file << "id,sku,name,category,price,quantity,reorderThreshold\n";
    ProductNode* curr = head;
    while (curr != nullptr) {
        file << curr->data.toCSV() << "\n";
        curr = curr->next;
    }
    file.close();
}
