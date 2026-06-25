#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

struct Product {
  int id;
  std::string sku; // e.g. "SHOE-001"
  std::string name;
  std::string category;
  double price;
  int quantity;
  int reorderThreshold;
  int reorderLevel; // For compatibility with MenuManager.cpp

  bool isLowStock() const;
  void display() const;
  std::string toCSV() const;
  static Product fromCSV(const std::string &line);
};

#endif // PRODUCT_H
