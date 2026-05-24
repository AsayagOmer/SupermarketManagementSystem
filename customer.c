//
// Created by Tom on 29.12.2024.
//



#include <string.h>
#include "customer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int initCustomer(Customer* customer) {
    memset(customer, 0, sizeof(Customer));
    initCustomerName(customer);
    if (!customer->name || !getCustomerIdFromUser(customer))
        return 0;
}

int getCustomerIdFromUser(Customer* customer) {
    char* id;
    do {
        printf("ID should be 9 digits\n");
        id = getUserInputString("For example: 123456789");
        if (!id) {
            return 0;
        }
    } while (strlen(id) != 9 || !containsOnlyDigits(id));
    strcpy(customer->id, id);
    free(id);
    return 1;
}

int containsOnlyDigits(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int initCustomerName(Customer* customer) {
    // Get first name
    char* firstName = getCustomerNameFromUser("Enter customer first name");
    if (!firstName) {
        return 0;
    }

    // Get last name
    char* lastName = getCustomerNameFromUser("Enter customer last name");
    if (!lastName) {
        free(firstName);
        return 0;
    }

    // Allocate space for the full name
    const size_t fullNameLen = strlen(firstName) + strlen(lastName) + 4; // " - " + null terminator
    customer->name = malloc(fullNameLen);
    if (!customer->name) {
        printf("Memory allocation failed.\n");
        free(firstName);
        free(lastName);
        return 0;
    }

    strcpy(customer->name, firstName);
    strcat(customer->name, " - ");
    strcat(customer->name, lastName);

    free(firstName);
    free(lastName);

    return 1;
}

char* getCustomerNameFromUser(const char* prompt) {
    char* name = getUserInputString(prompt);
    if (!name) {
        return NULL;
    }

    if (!containsOnlyLetters(name)) {
        printf("Name should contain only letters\n");
        free(name);
        return NULL;
    }

    stringToLowercase(name);

    char* back = (char*) malloc(strlen(name) + 1);
    if (!back) {
        printf("Error allocating memory\n");
        free(name);
        return NULL;
    }

    const char* words = strtok(name, " ");

    while (words != NULL) {
        strcat(back, words);
        strcat(back, " ");
        words = strtok(NULL, " ");
    }

    free(name);
    back[0] = toupper(back[0]);
    back[strlen(back) - 1] = '\0';
    return back;
}

void stringToLowercase(char* str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

int containsOnlyLetters(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

int customerEqualsById(const void* c1, const void* c2) {
    const Customer* customer1 = (Customer*) c1;
    const Customer* customer2 = (Customer*) c2;
    const int idEquals = strcmp(customer1->id, customer2->id) == 0;
    return idEquals;
}

int customerEqualsByName(const void* c1, const void* c2) {
    const Customer* customer1 = (Customer*) c1;
    const Customer* customer2 = (Customer*) c2;
    const int nameEquals = strcmp(customer1->name, customer2->name) == 0;
    return nameEquals;
}

int customerEquals(const void* c1, const void* c2) {
    const Customer* customer1 = (Customer*) c1;
    const Customer* customer2 = (Customer*) c2;
    const int nameEquals = strcmp(customer1->name, customer2->name) == 0;
    const int idEquals = strcmp(customer1->id, customer2->id) == 0;
    return nameEquals || idEquals;
}

void printCustomer(const void* c) {
    const Customer* customer = (Customer*) c;
    printf("Name: %s\n", customer->name);
    printf("ID: %s\n", customer->id);
    if (customer->shopping_cart.productCount == 0) {
        printf("Shopping cart is empty!\n");
    } else {
        printf("Doing shopping now!!\n");

    }
}

void printCustomerShoppingCart(const Customer* customer) {
    if (customer->shopping_cart.productCount == 0) {
        printf("Customer cart is empty\n");
        return;
    }

    for (int i = 0; i < customer->shopping_cart.productCount; i++) {
        const ShoppingItem* item = customer->shopping_cart.shopping_items[i];
        printf("Item %s count %d price per item %.2f\n", item->barcode, item->amount, item->price);
    }

    const float total = computeShoppingCartPrice(&customer->shopping_cart);
    printf("Total bill to pay: %.2f\n", total);
}

void freeCustomer(Customer* customer) {
    if (!customer) return;

    // Free the dynamically-allocated name
    free(customer->name);
    customer->name = NULL;

    // Free the shopping cart
    freeShoppingCart(&customer->shopping_cart);
}