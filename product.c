//
// Created by Tom on 29.12.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "utils.h"

const char* typeTitle[NofTypes] = {"Fruit Vegetable", "Fridge", "Frozen", "Shelf"};
const char* shortTypeTitle[NofTypes] = {"FV", "FR", "FZ", "SH"};

int init_product(Product* product) {
    memset(product, 0, sizeof(Product));

    if (!myGets("enter product name up to 20 chars", product->name, NAME_LEN)) {
        printf("Error: Failed to get input!\n");
        return 0;
    }

    product->type = getTypeFromUser();
    generateBarcode(product);
    init_date(&product->expiration_date);
    product->price = getNonNegativeFloatFromUser("Enter product price");
    product->amount = (int) getNonNegativeFloatFromUser("Enter product number of items");

    return 1;
}

void generateBarcode(Product* product) {
    strcpy(product->barcode, shortTypeTitle[product->type]);

    for (int i = (int) strlen(product->barcode); i < BARCODE_LEN; i++) {
        const int randint = rand() % 10;
        const char digit = '0' + randint;
        product->barcode[i] = digit;
    }
}

ProductType getTypeFromUser() {
    printf("\n");
    int t;
    do {
        for (int i = 0; i < NofTypes; i++) {
            printf("%d for %s\n", i, typeTitle[i]);
        }
        scanf("%d", &t);
    } while (t < 0 || t >= NofTypes);
    printf("\n");
    return (ProductType) t;
}

int productEquals(const void* p1, const void* p2) {
    const Product *product1 = *(Product**) p1;
    const Product *product2 = *(Product**) p2;
    return strcmp(product1->barcode, product2->barcode) == 0;
}

int productBarcodeEquals(const void* p, const void* val) {
    const Product* product = *(const Product**) p;
    const char* barcode = (const char*) val;
    return strcmp(product->barcode, barcode) == 0;
}

void print_product(const void* p){
    const Product *product = *(Product**) p;
    printf("%-21s%-11s%-22s%.2f            %d              ", product->name, product->barcode, typeTitle[product->type],
           product->price, product->amount);
    print_date(&product->expiration_date);
    printf("\n");
}