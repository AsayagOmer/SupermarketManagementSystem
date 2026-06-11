
#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include "shoppingitem.h"

typedef struct {
    ShoppingItem* head;     // Head of linked list
    int productCount;
} ShoppingCart;

ShoppingCart* initShoppingCart();
float computeShoppingCartPrice(const ShoppingCart* shopping_cart);
int addItemToShoppingCart(ShoppingCart* shopping_cart, ShoppingItem* item);
ShoppingItem* findItemInCart(const ShoppingCart* cart, const char* barcode);
int removeItemFromCart(ShoppingCart* cart, const char* barcode);
void freeShoppingCart(ShoppingCart* cart);

#endif //SHOPPING_CART_H
