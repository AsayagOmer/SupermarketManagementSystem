//
// Created by Tom on 10.01.2025.
//

#ifndef SHOPPINGITEM_H
#define SHOPPINGITEM_H
#include "product.h"

typedef struct {
    char barcode[7]; // TODO NOTE TO SELF: consider making barcode a struct
    float price;
    int amount;
} ShoppingItem;

int shoppingItemEquals(const void* i1, const void* i2);
ShoppingItem* initShoppingItem(const Product *p1, int amount);

#endif //SHOPPINGITEM_H
