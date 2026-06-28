#include "../include/Product.h"
#include "../include/FileManager.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

bool Product::isLowStock() const { return quantity <= reorderThreshold; }

void Product::display() const {
  std::cout << std::left << std::setw(6) << id << std::setw(15) << sku
            << std::setw(25) << name << std::setw(15) << category
            << std::setw(10) << std::fixed << std::setprecision(2) << price
            << std::setw(10) << quantity << std::setw(10) << reorderThreshold
            << "\n";
}

std::string Product::toCSV() const {
  std::stringstream ss;
  ss << id << "," << sku << "," << name << "," << category << "," << price
     << "," << quantity << "," << reorderThreshold;
  return ss.str();
}

Product Product::fromCSV(const std::string &line) {
  Product p;
  p.id = 0;
  p.price = 0.0;
  p.quantity = 0;
  p.reorderThreshold = 0;
  p.reorderLevel = 0;

  std::vector<std::string> fields;
  std::stringstream ss(line);
  std::string field;
  while (std::getline(ss, field, ',')) {
    fields.push_back(field);
  }

  if (fields.size() == 7) {
    try {
      p.id = std::stoi(fields[0]);
      p.sku = fields[1];
      p.name = fields[2];
      p.category = fields[3];
      p.price = std::stod(fields[4]);
      p.quantity = std::stoi(fields[5]);
      p.reorderThreshold = std::stoi(fields[6]);
      p.reorderLevel = p.reorderThreshold; // sync
    } catch (...) {
      // handle parse error if any
    }
  } else if (fields.size() == 5) {
    // Support legacy/alternate format: sku, name, price, quantity, low stock threshold
    p.sku = fields[0];
    p.name = fields[1];
    p.category = "";
    try {
      p.price = std::stod(fields[2]);
    } catch (...) {
      p.price = 0.0;
    }
    try {
      p.quantity = std::stoi(fields[3]);
    } catch (...) {
      p.quantity = 0;
    }
    try {
      p.reorderThreshold = std::stoi(fields[4]);
    } catch (...) {
      p.reorderThreshold = 0;
    }
    p.reorderLevel = p.reorderThreshold;
  }

  return p;
}
