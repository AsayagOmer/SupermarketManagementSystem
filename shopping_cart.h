
#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include "shoppingitem.h"

typedef struct {
    ShoppingItem** shopping_items;
    int productCount;
} ShoppingCart;

ShoppingCart* initShoppingCart();
float computeShoppingCartPrice(const ShoppingCart* shopping_cart);
int addItemToShoppingCart(ShoppingCart* shopping_cart, ShoppingItem* item);
void freeShoppingCart(ShoppingCart* cart);

#endif //SHOPPING_CART_H
