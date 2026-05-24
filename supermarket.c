//
// Created by Tom on 10.01.2025.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "supermarket.h"

#include <ctype.h>

#include "utils.h"



int initSuperMarket(SuperMarket* super_market) {
    // init all fields with zero or NULL
    memset(super_market, 0, sizeof(SuperMarket));

    char* name = getUserInputString("Enter market name");

    if (!name) {
        return 0;
    }

    super_market->name = malloc(strlen(name) + 1);
    if (super_market->name == NULL) {
        printf("Error allocating memory for market name\n");
        free(name);
        return 0;
    }

    strcpy(super_market->name, name);
    free(name);
    return 1;
}

void displaySuperMarket(const SuperMarket* super_market) {
    printf("Super Market Name: %s\n", super_market->name);
    printf("There are %d products\n", super_market->productCount);
    displayAllProducts(super_market);
    displayAllCustomers(super_market);
}

void displayAllProducts(const SuperMarket* superMarket) {
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printArray(superMarket->products, superMarket->productCount, sizeof(Product*), print_product);
}

void displayAllCustomers(const SuperMarket* superMarket) {
    printf("There are %d listed customers\n", superMarket->customerAmount);
    printArray(superMarket->customers, superMarket->customerAmount, sizeof(Customer), printCustomer);
}

int addNewProductToSuperMarket(SuperMarket * super_market) {
    Product* newProduct = (Product*) malloc(sizeof(Product));
    if (!newProduct) {
        printf("Memory allocation for new product failed!\n");
        return 0;
    }

    const int productInitialized = init_product(newProduct);
    if (!productInitialized) {
        printf("Adding product failed!\n");
        return 0;
    }

    const size_t newSize = ++super_market->productCount * sizeof(Product*);
    super_market->products = (Product**) safeRealloc(super_market->products, newSize);

    if (!super_market->products) {
        printf("Memory allocation failed!\n");
        super_market->productCount = 0;
        return 0;
    }

    super_market->products[super_market->productCount - 1] = newProduct;
    return 1;
}

int searchProductByBarcode(const SuperMarket* superMarket) {
    Product temp;
    while (1) {
        printf("Code should be of 7 length exactly\n");
        printf("Must have 2 type prefix letters following by a 5 digits number\n");
        printf("For example: FR20301\n");
        scanf("%s", temp.barcode);
        const char prefix[] = {temp.barcode[0], temp.barcode[1], '\0'};
        if (strlen(temp.barcode) != 7)
            printf("Invalid barcode length\n");
        else if (arraySearch(shortTypeTitle, NofTypes, sizeof(char*), prefix, stringEquals) == -1) {
            printf("Invalid type prefix\n");
        } else if (!containsOnlyDigits(temp.barcode + 2)) {
            printf("Incorrect number of digits\n");
        } else {
            break;
        }
    }
    Product* toSearch = &temp;
    const int index = arraySearch(superMarket->products, superMarket->productCount, sizeof(Product*), &toSearch, productEquals);

    if (index == -1) {
        printf("No such product barcode\n");
    }
    return index;
}

int addToExistingProduct(const SuperMarket* super_market) {
    const int index = searchProductByBarcode(super_market);
    if (index == -1) {
        return 0;
    }

    int amountToAdd;
    do {
        printf("How many items to add to stock? ");
        scanf("%d", &amountToAdd);
    } while (amountToAdd <= 0);

    super_market->products[index]->amount += amountToAdd;

    return 1;
}


int addProductToSuperMarket(SuperMarket* super_market) {
    printf("Adding new product? y/Y:");
    char userChoice;
    scanf(" %c", &userChoice);
    // Clear the input buffer to avoid issues with subsequent fgets
    while (getchar() != '\n');

    if (tolower(userChoice) == 'y') {
        return addNewProductToSuperMarket(super_market);
    }

    return addToExistingProduct(super_market);
}

int addCustomerToSuperMarket(SuperMarket* superMarket) {
    Customer* newCustomer = (Customer*) malloc(sizeof(Customer));
    if (!newCustomer) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    if (!initCustomer(newCustomer)) {
        printf("Error adding customer\n");
        free(newCustomer);
        return 0;
    }

    const int indexId = arraySearch(superMarket->customers, superMarket->customerAmount, sizeof(Customer), newCustomer,
                            customerEqualsById);
    if (indexId != -1) {
        printf("ID %s is not unique\n", newCustomer->id);
        return addCustomerToSuperMarket(superMarket);
    }

    const int indexName = arraySearch(superMarket->customers, superMarket->customerAmount, sizeof(Customer), newCustomer,
                            customerEqualsByName);
    if (indexName != -1) {
        printf("This customer already in market\n");
        printf("Error adding customer\n");
        return 0;
    }

    const size_t new_size = ++superMarket->customerAmount * sizeof(Customer);
    superMarket->customers = (Customer*) safeRealloc(superMarket->customers, new_size);
    if (!superMarket->customers) {
        printf("Error allocating memory\n");
        free(newCustomer);
        superMarket->customerAmount = 0;
        return 0;
    }
    superMarket->customers[superMarket->customerAmount - 1] = *newCustomer;
    return 1;
}

int canShop(const SuperMarket* superMarket) {
    if (superMarket->productCount == 0) {
        printf("No products in market - cannot shop\n");
        return 0;
    }

    if (superMarket->customerAmount == 0) {
        printf("No customers listed to market\n");
        return 0;
    }
    return 1;
}

int selectCustomer(const SuperMarket* superMarket) {
    Customer toSearch;
    toSearch.name = getUserInputString("Who is shopping? Enter customer name or id");
    strcpy(toSearch.id, toSearch.name);

    const int foundIndex = arraySearch(superMarket->customers, superMarket->customerAmount, sizeof(Customer), &toSearch, customerEquals);
    if (foundIndex == -1) {
        printf("this customer not listed\n");
        return -1;
    }
    return foundIndex;
}

int confirmShopping(void) {
    printf("Do you want to shop for a product? y/Y, anything else to exit!!");
    char choice;
    scanf(" %c", &choice);
    while (getchar()!='\n');
    if (tolower(choice) != 'y') {
        return 0;
    }
    return 1;
}

int shopping(const SuperMarket* superMarket) {
    if (!canShop(superMarket)) {
        return 0;
    }

    displayAllCustomers(superMarket);
    const int index = selectCustomer(superMarket);
    if (index == -1) return 0;
    displayAllProducts(superMarket);
    while (confirmShopping()) {
        const int productIndex = searchProductByBarcode(superMarket);
        Product* selectedProduct = superMarket->products[productIndex];

        int amount;
        do {
            printf("How many items do you want? max %d\n", selectedProduct->amount);
            scanf("%d", &amount);
            while (getchar()!='\n');
        } while (amount <= 0 || amount > selectedProduct->amount);
        selectedProduct->amount -= amount;
        const ShoppingItem* item = initShoppingItem(selectedProduct, amount);
        addItemToShoppingCart(&superMarket->customers[index].shopping_cart, item);
    }
    return 1;
}

void printShoppingCart(const SuperMarket* super_market) {
    displayAllCustomers(super_market);
    const int index = selectCustomer(super_market);
    if (index != -1)
        printCustomerShoppingCart(&super_market->customers[index]);
}

void returnProductsToStore(const SuperMarket* superMarket, Customer* customer) {
    if (!superMarket || !customer) {
        printf("Invalid supermarket or customer pointer!\n");
        return;
    }

    // For each item in the cart, return it to store
    for (int i = 0; i < customer->shopping_cart.productCount; i++) {
        const ShoppingItem* item = customer->shopping_cart.shopping_items[i];
        if (!item) continue;

        int productIndex = arraySearch(
            superMarket->products,
            superMarket->productCount,
            sizeof(Product*),
            item->barcode,       /* 'val' */
            productBarcodeEquals /* comparator */
        );
        if (productIndex == -1) {
            // Ideally shouldn't happen if guaranteed to exist
            printf("[Warning] Product not found for barcode %s\n", item->barcode);
            continue;
        }
        superMarket->products[productIndex]->amount += item->amount;
    }

    // Now that we've returned everything, just free the cart
    freeShoppingCart(&customer->shopping_cart);

    printf("!!! --- Purchase was canceled!!!! ---\n");
}

void processPayment(Customer* customer) {
    printf("---------- Cart info and bill for %s ----------\n", customer->name);
    printCustomerShoppingCart(customer);
    freeShoppingCart(&customer->shopping_cart);
    printf("!!! --- Payment was recived!!!! ---\n");
}

int shoppingCartManagement(const SuperMarket* superMarket) {
    displayAllCustomers(superMarket);
    const int index = selectCustomer(superMarket);
    if (index == -1) return 0;

    Customer* selectedCustomer = &superMarket->customers[index];
    if (selectedCustomer->shopping_cart.productCount == 0) {
        printf("Shopping cart is empty!\n");
        return 0;
    }

    printCustomerShoppingCart(selectedCustomer);
    printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!");
    char choice;
    scanf(" %c", &choice);
    while (getchar() != '\n');

    if (tolower(choice) != 'y') {
        returnProductsToStore(superMarket, selectedCustomer);
    } else {
        processPayment(selectedCustomer);
    }

    return 1;
}

void printProductsByType(const SuperMarket* superMarket) {
    const ProductType type = getTypeFromUser();

    if (!superMarket) {
        printf("Invalid supermarket pointer!\n");
        return;
    }

    int foundAny = 0;
    for (int i = 0; i < superMarket->productCount; i++) {
        const Product* p = superMarket->products[i];
        if (p->type == type) {
            print_product(&superMarket->products[i]);
            foundAny = 1;
        }
    }

    if (!foundAny) {
        printf("There are no product of type %s found in market %s", typeTitle[type], superMarket->name);
    }
}

void finalizeSuperMarket(SuperMarket* superMarket) {
    for (int i = 0; i < superMarket->customerAmount; i++) {
        Customer* customer = &superMarket->customers[i];
        if (customer->shopping_cart.productCount != 0) {
            processPayment(customer);
        }
    }
    freeSuperMarket(superMarket);
}

void freeSuperMarket(SuperMarket* superMarket) {
    if (!superMarket) return;

    // 1) Free the market name
    free(superMarket->name);
    superMarket->name = NULL;

    // 2) Free the array of customers
    //    (But first free each customer’s internals)
    for (int i = 0; i < superMarket->customerAmount; i++) {
        freeCustomer(&superMarket->customers[i]);
    }
    free(superMarket->customers);
    superMarket->customers = NULL;
    superMarket->customerAmount = 0;

    // 3) Free the array of product pointers
    //    (Each product was allocated with 'malloc')
    for (int i = 0; i < superMarket->productCount; i++) {
        free(superMarket->products[i]); // free the Product* itself
    }
    free(superMarket->products);
    superMarket->products = NULL;
    superMarket->productCount = 0;
}