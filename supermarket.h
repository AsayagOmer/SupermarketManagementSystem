
#ifndef SUPERMARKET_H
#define SUPERMARKET_H

#include "customer.h"
#include "product.h"


typedef struct {
    char* name;
    Customer* customers;
    int customerAmount;
    Product** products;
    int productCount;
} SuperMarket;

int initSuperMarket(SuperMarket* super_market);
int addToExistingProduct(const SuperMarket* super_market);
int addNewProductToSuperMarket(SuperMarket * super_market);
void displaySuperMarket(const SuperMarket* super_market);
int addProductToSuperMarket(SuperMarket* super_market);
int addCustomerToSuperMarket(SuperMarket* superMarket);
void displayAllCustomers(const SuperMarket* superMarket);
void displayAllProducts(const SuperMarket* superMarket);
int shopping(const SuperMarket* superMarket);
int searchProductByBarcode(const SuperMarket* superMarket);
int canShop(const SuperMarket* superMarket);
int selectCustomer(const SuperMarket* superMarket);
int confirmShopping(void);
void printShoppingCart(const SuperMarket* super_market);
int shoppingCartManagement(const SuperMarket* superMarket);
void returnProductsToStore(const SuperMarket* superMarket, Customer* customer);
void processPayment(Customer* customer);
void printProductsByType(const SuperMarket* superMarket);
void finalizeSuperMarket(SuperMarket* superMarket);
void freeSuperMarket(SuperMarket* superMarket);

// Enhanced product filtering
void filterProductsByName(const SuperMarket* superMarket);
void filterProductsByPriceRange(const SuperMarket* superMarket);
void filterProductsByExpiry(const SuperMarket* superMarket);
void sortProductsByPrice(const SuperMarket* superMarket);
void sortProductsByName(const SuperMarket* superMarket);
void productFilterMenu(const SuperMarket* superMarket);

// Data persistence
void saveAllData(const SuperMarket* superMarket);


#endif //SUPERMARKET_H
