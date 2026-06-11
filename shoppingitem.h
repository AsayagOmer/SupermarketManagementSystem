
#ifndef SHOPPINGITEM_H
#define SHOPPINGITEM_H
#include "product.h"

typedef struct ShoppingItem {
    char barcode[BARCODE_LEN];
    float price;
    int amount;
    struct ShoppingItem* next;  // Linked list pointer
} ShoppingItem;

int shoppingItemEquals(const void* i1, const void* i2);
ShoppingItem* initShoppingItem(const Product *p1, int amount);

#endif //SHOPPINGITEM_H
