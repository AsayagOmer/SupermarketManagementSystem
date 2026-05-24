//
// Created by Tom on 29.12.2024.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "shopping_cart.h"

#define ID_LEN 9

typedef struct {
    char id[ID_LEN], *name;
    ShoppingCart shopping_cart;
} Customer;

int initCustomer(Customer* customer);
char* getCustomerNameFromUser(const char* prompt);
void stringToLowercase(char* str);
void printCustomer(const void* c);
int initCustomerName(Customer* customer);
int containsOnlyLetters(const char* str);
int containsOnlyDigits(const char* str);
int getCustomerIdFromUser(Customer* customer);
int customerEqualsById(const void* c1, const void* c2);
int customerEqualsByName(const void* c1, const void* c2);
int customerEquals(const void* c1, const void* c2);
void printCustomerShoppingCart(const Customer* customer);
void freeCustomer(Customer* customer);

#endif //CUSTOMER_H
