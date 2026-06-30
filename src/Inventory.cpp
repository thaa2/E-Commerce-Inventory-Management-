#include "../include/Inventory.h"
#include "../include/FileManager.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

// ══════════════════════════════════════════════════════════════════════════════
// Constructor / Destructor
// ══════════════════════════════════════════════════════════════════════════════

Inventory::Inventory() {
  head   = nullptr;
  count  = 0;
  nextId = 1;
  for (int i = 0; i < HASH_SIZE; i++) {
    hashTable[i] = nullptr;
  }
}

Inventory::~Inventory() {
  // Free linked list
  ProductNode *curr = head;
  while (curr != nullptr) {
    ProductNode *nextNode = curr->next;
    delete curr;
    curr = nextNode;
  }
  // Free hash-table chains
  for (int i = 0; i < HASH_SIZE; i++) {
    HashNode *hCurr = hashTable[i];
    while (hCurr != nullptr) {
      HashNode *hNext = hCurr->next;
      delete hCurr;
      hCurr = hNext;
    }
  }
}

// ══════════════════════════════════════════════════════════════════════════════
// Private: hash-table helpers
// ══════════════════════════════════════════════════════════════════════════════

int Inventory::hashFunction(const std::string &key) const {
  int hash = 0;
  for (char c : key) {
    hash = (hash * 31 + c) % HASH_SIZE;
  }
  if (hash < 0)
    hash += HASH_SIZE;
  return hash;
}

void Inventory::insertHash(const std::string &key, int val) {
  int index = hashFunction(key);
  HashNode *n = new HashNode{key, val, hashTable[index]};
  hashTable[index] = n;
}

void Inventory::removeHash(const std::string &key) {
  int index = hashFunction(key);
  HashNode *curr = hashTable[index];
  HashNode *prev = nullptr;
  while (curr != nullptr) {
    if (curr->key == key) {
      if (prev == nullptr)
        hashTable[index] = curr->next;
      else
        prev->next = curr->next;
      delete curr;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: hash search
// ══════════════════════════════════════════════════════════════════════════════

int Inventory::hashSearch(const std::string &sku) {
  int index = hashFunction(sku);
  HashNode *curr = hashTable[index];
  while (curr != nullptr) {
    if (curr->key == sku)
      return curr->value;
    curr = curr->next;
  }
  return -1; // not found
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: SKU helpers
// ══════════════════════════════════════════════════════════════════════════════

bool Inventory::skuExists(const std::string &sku) {
  return hashSearch(sku) != -1;
}

Product *Inventory::findBySku(const std::string &sku) {
  int id = hashSearch(sku);
  if (id == -1)
    return nullptr;
  return findById(id);
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Add a brand-new product
//   - Returns false (and prints an error) if the SKU already exists.
//   - Returns true on success.
// ══════════════════════════════════════════════════════════════════════════════

bool Inventory::addProduct(Product p) {
  // ── Duplicate-SKU guard ───────────────────────────────────────────────────
  if (skuExists(p.sku)) {
    std::cout << " [Error] SKU '" << p.sku
              << "' already exists. Use 'Add Stock by SKU' to restock it.\n";
    return false;
  }

  // ── Assign auto-ID if none provided ──────────────────────────────────────
  ProductNode *n = new ProductNode();
  n->data = p;
  if (n->data.id <= 0) {
    n->data.id = nextId++;
  } else {
    if (n->data.id >= nextId)
      nextId = n->data.id + 1;
  }
  n->next = nullptr;

  // ── Append to end of linked list ─────────────────────────────────────────
  if (head == nullptr) {
    head = n;
  } else {
    ProductNode *temp = head;
    while (temp->next != nullptr)
      temp = temp->next;
    temp->next = n;
  }
  count++;
  insertHash(n->data.sku, n->data.id);
  return true;
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Add stock to an existing product (identified by SKU)
//   - Returns false (and prints an error) if the SKU is not found.
//   - Returns true on success.
// ══════════════════════════════════════════════════════════════════════════════

bool Inventory::addStockBySku(const std::string &sku, int amount) {
  Product *p = findBySku(sku);
  if (p == nullptr) {
    std::cout << " [Error] SKU '" << sku << "' not found in inventory.\n";
    return false;
  }
  if (amount <= 0) {
    std::cout << " [Error] Amount must be greater than zero.\n";
    return false;
  }
  p->quantity += amount;
  std::cout << " [OK] Added " << amount << " unit(s) to '" << p->name
            << "'. New quantity: " << p->quantity << "\n";
  return true;
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Remove / find
// ══════════════════════════════════════════════════════════════════════════════

bool Inventory::removeProduct(int id) {
  if (head == nullptr)
    return false;

  if (head->data.id == id) {
    ProductNode *oldHead = head;
    removeHash(oldHead->data.sku);
    head = head->next;
    delete oldHead;
    count--;
    return true;
  }

  ProductNode *prev = head;
  while (prev->next != nullptr) {
    if (prev->next->data.id == id) {
      ProductNode *target = prev->next;
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

Product *Inventory::findById(int id) {
  ProductNode *curr = head;
  while (curr != nullptr) {
    if (curr->data.id == id)
      return &curr->data;
    curr = curr->next;
  }
  return nullptr;
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Display
// ══════════════════════════════════════════════════════════════════════════════

static void printInventoryHeader() {
  std::cout << std::left << std::setw(6) << "ID" << std::setw(15) << "SKU"
            << std::setw(25) << "Name" << std::setw(15) << "Category"
            << std::setw(10) << "Price" << std::setw(10) << "Quantity"
            << std::setw(10) << "Reorder" << "\n";
  std::cout << std::string(91, '-') << "\n";
}

void Inventory::displayAll() {
  printInventoryHeader();
  ProductNode *curr = head;
  while (curr != nullptr) {
    curr->data.display();
    curr = curr->next;
  }
  if (count == 0)
    std::cout << " (no products)\n";
}

void Inventory::displayLowStock() {
  printInventoryHeader();
  bool any = false;
  ProductNode *curr = head;
  while (curr != nullptr) {
    if (curr->data.isLowStock()) {
      curr->data.display();
      any = true;
    }
    curr = curr->next;
  }
  if (!any)
    std::cout << " (all products are sufficiently stocked)\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Sorting
// ══════════════════════════════════════════════════════════════════════════════

int Inventory::copyToArray(Product arr[], int maxLimit) {
  int i = 0;
  ProductNode *curr = head;
  while (curr != nullptr && i < maxLimit) {
    arr[i++] = curr->data;
    curr = curr->next;
  }
  return i;
}

// Selection-sort by price.
//   ascending = true  → small to big  (low price first)
//   ascending = false → big to small  (high price first)
void Inventory::sortByPrice(Product arr[], int n, bool ascending) {
  for (int i = 0; i < n - 1; i++) {
    int pick = i;
    for (int j = i + 1; j < n; j++) {
      bool better = ascending ? (arr[j].price < arr[pick].price)
                              : (arr[j].price > arr[pick].price);
      if (better)
        pick = j;
    }
    Product tmp = arr[i];
    arr[i]    = arr[pick];
    arr[pick] = tmp;
  }
}

// Selection-sort by name (alphabetical).
//   ascending = true  → A to Z
//   ascending = false → Z to A
void Inventory::sortByName(Product arr[], int n, bool ascending) {
  for (int i = 0; i < n - 1; i++) {
    int pick = i;
    for (int j = i + 1; j < n; j++) {
      bool better = ascending ? (arr[j].name < arr[pick].name)
                              : (arr[j].name > arr[pick].name);
      if (better)
        pick = j;
    }
    Product tmp = arr[i];
    arr[i]    = arr[pick];
    arr[pick] = tmp;
  }
}

// Selection-sort by quantity.
//   ascending = true  → small to big
//   ascending = false → big to small
void Inventory::sortByQuantity(Product arr[], int n, bool ascending) {
  for (int i = 0; i < n - 1; i++) {
    int pick = i;
    for (int j = i + 1; j < n; j++) {
      bool better = ascending ? (arr[j].quantity < arr[pick].quantity)
                              : (arr[j].quantity > arr[pick].quantity);
      if (better)
        pick = j;
    }
    Product tmp = arr[i];
    arr[i]    = arr[pick];
    arr[pick] = tmp;
  }
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: Stock / value
// ══════════════════════════════════════════════════════════════════════════════

bool Inventory::adjustStock(int id, int amount) {
  Product *p = findById(id);
  if (p != nullptr) {
    p->quantity += amount;
    if (p->quantity < 0)
      p->quantity = 0;
    return true;
  }
  return false;
}

double Inventory::totalValue() {
  double total = 0.0;
  ProductNode *curr = head;
  while (curr != nullptr) {
    total += curr->data.price * curr->data.quantity;
    curr = curr->next;
  }
  return total;
}

// ══════════════════════════════════════════════════════════════════════════════
// Public: File I/O  (uses FileManager helpers, same as the original)
// ══════════════════════════════════════════════════════════════════════════════

void Inventory::loadFromFile(const std::string &filepath) {
  ensureFile(filepath);
  std::string lines[1000];
  int lineCount = readLines(filepath, lines, 1000);
  if (lineCount <= 1)
    return;

  for (int i = 1; i < lineCount; i++) {
    if (lines[i].empty())
      continue;
    Product p = Product::fromCSV(lines[i]);
    if (p.sku.empty() || p.name.empty())
      continue;
    // Bypass duplicate guard when loading our own CSV data
    if (!skuExists(p.sku))
      addProduct(p);
  }
}

void Inventory::saveToFile(const std::string &filepath) const {
  std::string lines[1000];
  int lineCount = 0;
  lines[lineCount++] = "id,sku,name,category,price,quantity,reorderThreshold";

  ProductNode *curr = head;
  while (curr != nullptr) {
    lines[lineCount++] = curr->data.toCSV();
    curr = curr->next;
  }
  writeLines(filepath, lines, lineCount);
}

void Inventory::saveCategories(const std::string &filepath) const {
  std::string lines[1000];
  int lineCount = 0;
  ProductNode *curr = head;
  while (curr != nullptr && lineCount < 1000) {
    if (!curr->data.category.empty()) {
      lines[lineCount++] = curr->data.category;
    }
    curr = curr->next;
  }
  writeLines(filepath, lines, lineCount);
}
