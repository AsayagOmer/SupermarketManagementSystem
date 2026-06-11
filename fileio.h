#ifndef FILEIO_H
#define FILEIO_H

#include "product.h"
#include "customer.h"

#define DATA_DIR "data"
#define PRODUCTS_FILE "data/products.txt"
#define CUSTOMERS_FILE "data/customers.txt"
#define PURCHASE_HISTORY_FILE "data/purchase_history.txt"

// Save/Load products
int saveProducts(const char* filename, Product** products, int count);
int loadProducts(const char* filename, Product*** products, int* count);

// Save/Load customers (saves name and ID only, not cart)
int saveCustomers(const char* filename, const Customer* customers, int count);
int loadCustomers(const char* filename, Customer** customers, int* count);

// Append a purchase record
int savePurchaseRecord(const char* filename, const char* customerId,
                       const char* customerName, const ShoppingCart* cart, float total);

// Ensure data directory exists
int ensureDataDirectory(void);

#endif // FILEIO_H
