//
// Created by Tom on 10.01.2025.
//
#include <string.h>
#include "shoppingitem.h"

#include <stdlib.h>

int shoppingItemEquals(const void* i1, const void* i2) {
    const ShoppingItem* item1 = *(ShoppingItem**) i1;
    const ShoppingItem* item2 = *(ShoppingItem**) i2;
    return strcmp(item1->barcode, item2->barcode) == 0;
}

ShoppingItem* initShoppingItem(const Product *p1, const int amount) {
    ShoppingItem* back = (ShoppingItem*) malloc(sizeof(ShoppingItem));
    strcpy(back->barcode, p1->barcode);
    back->price = p1->price;
    back->amount = amount;
    return back;
}