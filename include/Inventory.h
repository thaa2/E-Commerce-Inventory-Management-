#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <string>

struct ProductNode {
    Product data;
    ProductNode* next;
};

struct HashNode {
    std::string key; // sku
    int value; // product id
    HashNode* next;
};

class Inventory {
private:
    static const int HASH_SIZE = 100;
    HashNode* hashTable[HASH_SIZE];

    int hashFunction(const std::string& key) const;
    void insertHash(const std::string& key, int val);
    void removeHash(const std::string& key);

public:
    ProductNode* head;
    int count;
    int nextId;

    Inventory();
    ~Inventory();

    void addProduct(Product p);
    bool removeProduct(int id);
    Product* findById(int id);
    void displayAll();
    void displayLowStock();

    // Additional methods needed by MenuManager
    int hashSearch(const std::string& sku);
    int copyToArray(Product arr[], int maxLimit);
    void sortByPrice(Product arr[], int n);
    void sortByName(Product arr[], int n);
    void sortByQuantity(Product arr[], int n);
    bool adjustStock(int id, int amount);
    double totalValue();

    // File handling
    void loadFromFile(const std::string& filepath);
    void saveToFile(const std::string& filepath) const;
};

#endif // INVENTORY_H
