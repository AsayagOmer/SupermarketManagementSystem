//
// Created by Tom on 29.12.2024.
//

#ifndef PRODUCT_H
#define PRODUCT_H

#include "date.h"

#define NAME_LEN 20
#define BARCODE_LEN 7

typedef enum {FruitVegetable, Fridge, Frozen, Shelf, NofTypes} ProductType;
extern const char* typeTitle[NofTypes];
extern const char* shortTypeTitle[NofTypes];

typedef struct {

  char name[NAME_LEN];
  char barcode[BARCODE_LEN];
  ProductType type;
  float price;
  int amount;
  Date expiration_date;

} Product;

int init_product(Product* product);
void print_product(const void* p);
ProductType getTypeFromUser();
void typeToString(ProductType type);
void generateBarcode(Product* product);
int productEquals(const void* p1, const void* p2);
int productBarcodeEquals(const void* p, const void* val);
// NO NEED TO FREE IN PRODUCT : NO STARTING MALLOC && ENDING.


#endif //PRODUCT_H
