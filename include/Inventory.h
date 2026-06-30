#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <string>

struct ProductNode {
  Product data;
  ProductNode *next;
};

struct HashNode {
  std::string key; // sku
  int value;       // product id
  HashNode *next;
};

class Inventory {
private:
  static const int HASH_SIZE = 100;
  HashNode *hashTable[HASH_SIZE];

  int  hashFunction(const std::string &key) const;
  void insertHash(const std::string &key, int val);
  void removeHash(const std::string &key);

public:
  ProductNode *head;
  int count;
  int nextId;

  Inventory();
  ~Inventory();

  // ── Core CRUD ──────────────────────────────────────────────────────────────
  // Adds a brand-new product. Returns false (with message) if SKU exists.
  bool addProduct(Product p);

  // Adds stock to an existing product found by SKU. Returns false if not found.
  bool addStockBySku(const std::string &sku, int amount);

  bool     removeProduct(int id);
  Product *findById(int id);
  Product *findBySku(const std::string &sku);

  // Returns true if the SKU is already in the inventory.
  bool skuExists(const std::string &sku);

  void displayAll();
  void displayLowStock();

  // ── Hash search ────────────────────────────────────────────────────────────
  int hashSearch(const std::string &sku);

  // ── Sorting (ascending = small→big, descending = big→small) ───────────────
  int  copyToArray(Product arr[], int maxLimit);
  void sortByPrice   (Product arr[], int n, bool ascending = true);
  void sortByName    (Product arr[], int n, bool ascending = true);
  void sortByQuantity(Product arr[], int n, bool ascending = true);

  // ── Misc ───────────────────────────────────────────────────────────────────
  bool   adjustStock(int id, int amount);
  double totalValue();

  // ── File I/O ───────────────────────────────────────────────────────────────
  void loadFromFile(const std::string &filepath);
  void saveToFile  (const std::string &filepath) const;
};

#endif // INVENTORY_H
