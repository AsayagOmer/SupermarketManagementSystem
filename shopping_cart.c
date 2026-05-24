//
// Created by Tom on 29.12.2024.
//

#include <stdlib.h>
#include "shopping_cart.h"

#include <stdio.h>
#include <string.h>

#include "utils.h"

ShoppingCart* initShoppingCart() {
    ShoppingCart* shopping_cart = (ShoppingCart*) malloc(sizeof(ShoppingCart));
    if (!shopping_cart) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    shopping_cart->shopping_items = NULL;
    shopping_cart->productCount = 0;
    return shopping_cart;
}

float computeShoppingCartPrice(const ShoppingCart* shopping_cart) {
    float price = 0.0f;
    for (int i = 0; i < shopping_cart->productCount; i++) {
        const ShoppingItem* item = shopping_cart->shopping_items[i];
        price += item->price * item->amount;
    }
    return price;
}

int addItemToShoppingCart(ShoppingCart* shopping_cart, ShoppingItem* item) {
    if (!shopping_cart || !item) {
        return 0;
    }

    // Search for an item that has the same barcode
    int index = arraySearch(
        shopping_cart->shopping_items,
        shopping_cart->productCount,
        sizeof(ShoppingItem*),
        &item,
        shoppingItemEquals
    );

    // If the item doesn't exist in the cart yet, add a new pointer
    if (index == -1) {
        shopping_cart->productCount++;
        size_t new_size = sizeof(ShoppingItem*) * shopping_cart->productCount;
        shopping_cart->shopping_items =
            (ShoppingItem**) safeRealloc(shopping_cart->shopping_items, new_size);

        if (!shopping_cart->shopping_items) {
            printf("Memory allocation failed!\n");
            shopping_cart->productCount = 0; // or revert to old count if you prefer
            return 0;
        }

        // Insert at the last valid index: productCount - 1
        shopping_cart->shopping_items[shopping_cart->productCount - 1] = item;
        return 1;
    }

    // If the item already exists, just increment its amount
    shopping_cart->shopping_items[index]->amount += item->amount;
    return 1;
}

void freeShoppingCart(ShoppingCart* cart) {
    if (!cart || !cart->shopping_items)
        return;

    // Free each ShoppingItem pointer
    for (int i = 0; i < cart->productCount; i++) {
        free((void*) cart->shopping_items[i]);
    }

    // Free the array of pointers
    free(cart->shopping_items);

    // Null and reset
    cart->shopping_items = NULL;
    cart->productCount = 0;
}
