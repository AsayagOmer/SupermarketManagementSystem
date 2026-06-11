
#include <stdlib.h>
#include "shopping_cart.h"

#include <stdio.h>
#include <string.h>

ShoppingCart* initShoppingCart() {
    ShoppingCart* shopping_cart = (ShoppingCart*) malloc(sizeof(ShoppingCart));
    if (!shopping_cart) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    shopping_cart->head = NULL;
    shopping_cart->productCount = 0;
    return shopping_cart;
}

float computeShoppingCartPrice(const ShoppingCart* shopping_cart) {
    float price = 0.0f;
    const ShoppingItem* current = shopping_cart->head;
    while (current) {
        price += current->price * current->amount;
        current = current->next;
    }
    return price;
}

ShoppingItem* findItemInCart(const ShoppingCart* cart, const char* barcode) {
    if (!cart || !barcode) return NULL;

    ShoppingItem* current = cart->head;
    while (current) {
        if (strcmp(current->barcode, barcode) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int addItemToShoppingCart(ShoppingCart* shopping_cart, ShoppingItem* item) {
    if (!shopping_cart || !item) {
        return 0;
    }

    // Search for an existing item with the same barcode
    ShoppingItem* existing = findItemInCart(shopping_cart, item->barcode);

    if (existing) {
        // If the item already exists, just increment its amount
        existing->amount += item->amount;
        // Free the new item since we merged it
        free(item);
        return 1;
    }

    // Insert new item at the head of the linked list
    item->next = shopping_cart->head;
    shopping_cart->head = item;
    shopping_cart->productCount++;
    return 1;
}

int removeItemFromCart(ShoppingCart* cart, const char* barcode) {
    if (!cart || !barcode || !cart->head) {
        return 0;
    }

    ShoppingItem* current = cart->head;
    ShoppingItem* previous = NULL;

    while (current) {
        if (strcmp(current->barcode, barcode) == 0) {
            // Unlink the node
            if (previous) {
                previous->next = current->next;
            } else {
                cart->head = current->next;
            }
            free(current);
            cart->productCount--;
            return 1;
        }
        previous = current;
        current = current->next;
    }

    printf("Item with barcode %s not found in cart\n", barcode);
    return 0;
}

void freeShoppingCart(ShoppingCart* cart) {
    if (!cart)
        return;

    // Traverse the linked list and free each node
    ShoppingItem* current = cart->head;
    while (current) {
        ShoppingItem* next = current->next;
        free(current);
        current = next;
    }

    // Reset the cart
    cart->head = NULL;
    cart->productCount = 0;
}
