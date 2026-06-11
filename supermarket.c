
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "supermarket.h"

#include <ctype.h>

#include "utils.h"
#include "fileio.h"



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

    // Load persisted data from files
    ensureDataDirectory();
    if (!loadProducts(PRODUCTS_FILE, &super_market->products, &super_market->productCount)) {
        printf("Warning: Could not load products from file\n");
    } else if (super_market->productCount > 0) {
        printf("Loaded %d products from file\n", super_market->productCount);
    }

    if (!loadCustomers(CUSTOMERS_FILE, &super_market->customers, &super_market->customerAmount)) {
        printf("Warning: Could not load customers from file\n");
    } else if (super_market->customerAmount > 0) {
        printf("Loaded %d customers from file\n", super_market->customerAmount);
    }

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
        free(newProduct);
        return 0;
    }

    const size_t newSize = ++super_market->productCount * sizeof(Product*);
    super_market->products = (Product**) safeRealloc(super_market->products, newSize);

    if (!super_market->products) {
        printf("Memory allocation failed!\n");
        super_market->productCount = 0;
        free(newProduct);
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
        freeCustomer(newCustomer);
        free(newCustomer);
        return addCustomerToSuperMarket(superMarket);
    }

    const int indexName = arraySearch(superMarket->customers, superMarket->customerAmount, sizeof(Customer), newCustomer,
                            customerEqualsByName);
    if (indexName != -1) {
        printf("This customer already in market\n");
        printf("Error adding customer\n");
        freeCustomer(newCustomer);
        free(newCustomer);
        return 0;
    }

    const size_t new_size = ++superMarket->customerAmount * sizeof(Customer);
    superMarket->customers = (Customer*) safeRealloc(superMarket->customers, new_size);
    if (!superMarket->customers) {
        printf("Error allocating memory\n");
        freeCustomer(newCustomer);
        free(newCustomer);
        superMarket->customerAmount = 0;
        return 0;
    }
    superMarket->customers[superMarket->customerAmount - 1] = *newCustomer;
    free(newCustomer);
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
    free(toSearch.name);
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
        if (productIndex == -1) {
            printf("Product not found, try again\n");
            continue;
        }
        Product* selectedProduct = superMarket->products[productIndex];

        int amount;
        do {
            printf("How many items do you want? max %d\n", selectedProduct->amount);
            scanf("%d", &amount);
            while (getchar()!='\n');
        } while (amount <= 0 || amount > selectedProduct->amount);
        selectedProduct->amount -= amount;
        ShoppingItem* item = initShoppingItem(selectedProduct, amount);
        if (item) {
            addItemToShoppingCart(&superMarket->customers[index].shopping_cart, item);
        }
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

    // Traverse linked list to return each item to store
    ShoppingItem* current = customer->shopping_cart.head;
    while (current) {
        int productIndex = arraySearch(
            superMarket->products,
            superMarket->productCount,
            sizeof(Product*),
            current->barcode,
            productBarcodeEquals
        );
        if (productIndex == -1) {
            printf("[Warning] Product not found for barcode %s\n", current->barcode);
        } else {
            superMarket->products[productIndex]->amount += current->amount;
        }
        current = current->next;
    }

    // Now that we've returned everything, just free the cart
    freeShoppingCart(&customer->shopping_cart);

    printf("!!! --- Purchase was canceled!!!! ---\n");
}

void processPayment(Customer* customer) {
    printf("---------- Cart info and bill for %s ----------\n", customer->name);
    printCustomerShoppingCart(customer);

    // Save purchase record before freeing the cart
    const float total = computeShoppingCartPrice(&customer->shopping_cart);
    savePurchaseRecord(PURCHASE_HISTORY_FILE, customer->id, customer->name,
                       &customer->shopping_cart, total);

    freeShoppingCart(&customer->shopping_cart);
    printf("!!! --- Payment was received!!!! ---\n");
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
        printf("There are no product of type %s found in market %s\n", typeTitle[type], superMarket->name);
    }
}

// ===================== Enhanced Product Filtering =====================

void filterProductsByName(const SuperMarket* superMarket) {
    char* searchTerm = getUserInputString("Enter product name to search for (partial match)");
    if (!searchTerm) return;

    // Convert search term to lowercase for case-insensitive matching
    for (int i = 0; searchTerm[i]; i++) {
        searchTerm[i] = tolower(searchTerm[i]);
    }

    printf("\n--- Products matching \"%s\" ---\n", searchTerm);
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    int foundAny = 0;
    for (int i = 0; i < superMarket->productCount; i++) {
        // Make a lowercase copy of the product name for comparison
        char nameLower[NAME_LEN];
        strncpy(nameLower, superMarket->products[i]->name, NAME_LEN - 1);
        nameLower[NAME_LEN - 1] = '\0';
        for (int j = 0; nameLower[j]; j++) {
            nameLower[j] = tolower(nameLower[j]);
        }

        if (strstr(nameLower, searchTerm)) {
            print_product(&superMarket->products[i]);
            foundAny = 1;
        }
    }

    if (!foundAny) {
        printf("No products found matching \"%s\"\n", searchTerm);
    }
    free(searchTerm);
}

void filterProductsByPriceRange(const SuperMarket* superMarket) {
    float minPrice, maxPrice;

    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    while (getchar() != '\n');

    if (minPrice > maxPrice) {
        float temp = minPrice;
        minPrice = maxPrice;
        maxPrice = temp;
    }

    printf("\n--- Products priced between %.2f and %.2f ---\n", minPrice, maxPrice);
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    int foundAny = 0;
    for (int i = 0; i < superMarket->productCount; i++) {
        const Product* p = superMarket->products[i];
        if (p->price >= minPrice && p->price <= maxPrice) {
            print_product(&superMarket->products[i]);
            foundAny = 1;
        }
    }

    if (!foundAny) {
        printf("No products found in that price range\n");
    }
}

void filterProductsByExpiry(const SuperMarket* superMarket) {
    Date deadline;
    printf("Show products expiring before:\n");
    init_date(&deadline);

    printf("\n--- Products expiring before %02d/%02d/%04d ---\n",
           deadline.day, deadline.month, deadline.year);
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    int foundAny = 0;
    for (int i = 0; i < superMarket->productCount; i++) {
        const Product* p = superMarket->products[i];
        const Date* ed = &p->expiration_date;

        // Check if product expires before the deadline
        int expiresBefore = 0;
        if (ed->year < deadline.year) {
            expiresBefore = 1;
        } else if (ed->year == deadline.year && ed->month < deadline.month) {
            expiresBefore = 1;
        } else if (ed->year == deadline.year && ed->month == deadline.month && ed->day < deadline.day) {
            expiresBefore = 1;
        }

        if (expiresBefore) {
            print_product(&superMarket->products[i]);
            foundAny = 1;
        }
    }

    if (!foundAny) {
        printf("No products found expiring before that date\n");
    }
}

void sortProductsByPrice(const SuperMarket* superMarket) {
    if (superMarket->productCount <= 1) {
        displayAllProducts(superMarket);
        return;
    }

    printf("Sort by price: 1 for ascending, 2 for descending: ");
    int order;
    scanf("%d", &order);
    while (getchar() != '\n');

    // Create a temporary copy of the pointers array for sorting
    Product** sorted = (Product**) malloc(superMarket->productCount * sizeof(Product*));
    if (!sorted) {
        printf("Memory allocation failed\n");
        return;
    }
    memcpy(sorted, superMarket->products, superMarket->productCount * sizeof(Product*));

    // Selection sort
    for (int i = 0; i < superMarket->productCount - 1; i++) {
        int targetIdx = i;
        for (int j = i + 1; j < superMarket->productCount; j++) {
            if (order == 1) {
                if (sorted[j]->price < sorted[targetIdx]->price)
                    targetIdx = j;
            } else {
                if (sorted[j]->price > sorted[targetIdx]->price)
                    targetIdx = j;
            }
        }
        if (targetIdx != i) {
            Product* temp = sorted[i];
            sorted[i] = sorted[targetIdx];
            sorted[targetIdx] = temp;
        }
    }

    printf("\n--- Products sorted by price (%s) ---\n", order == 1 ? "ascending" : "descending");
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printArray(sorted, superMarket->productCount, sizeof(Product*), print_product);

    free(sorted);
}

void sortProductsByName(const SuperMarket* superMarket) {
    if (superMarket->productCount <= 1) {
        displayAllProducts(superMarket);
        return;
    }

    // Create a temporary copy of the pointers array for sorting
    Product** sorted = (Product**) malloc(superMarket->productCount * sizeof(Product*));
    if (!sorted) {
        printf("Memory allocation failed\n");
        return;
    }
    memcpy(sorted, superMarket->products, superMarket->productCount * sizeof(Product*));

    // Selection sort alphabetically
    for (int i = 0; i < superMarket->productCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < superMarket->productCount; j++) {
            if (strcmp(sorted[j]->name, sorted[minIdx]->name) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Product* temp = sorted[i];
            sorted[i] = sorted[minIdx];
            sorted[minIdx] = temp;
        }
    }

    printf("\n--- Products sorted alphabetically ---\n");
    printf("Name                 Barcode    Type                 Price      Count In Stock       Expiry Date\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printArray(sorted, superMarket->productCount, sizeof(Product*), print_product);

    free(sorted);
}

void productFilterMenu(const SuperMarket* superMarket) {
    if (superMarket->productCount == 0) {
        printf("No products in store to filter\n");
        return;
    }

    int choice;
    do {
        printf("\n--- Product Filter & Sort ---\n");
        printf("1 - Filter by type\n");
        printf("2 - Filter by name (search)\n");
        printf("3 - Filter by price range\n");
        printf("4 - Filter by expiry date\n");
        printf("5 - Sort by price\n");
        printf("6 - Sort by name\n");
        printf("0 - Back to main menu\n");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: printProductsByType(superMarket); break;
            case 2: filterProductsByName(superMarket); break;
            case 3: filterProductsByPriceRange(superMarket); break;
            case 4: filterProductsByExpiry(superMarket); break;
            case 5: sortProductsByPrice(superMarket); break;
            case 6: sortProductsByName(superMarket); break;
            case 0: break;
            default: printf("Invalid option\n"); break;
        }
    } while (choice != 0);
}

// ===================== Data Persistence =====================

void saveAllData(const SuperMarket* superMarket) {
    ensureDataDirectory();
    if (saveProducts(PRODUCTS_FILE, superMarket->products, superMarket->productCount)) {
        printf("Products saved successfully (%d products)\n", superMarket->productCount);
    } else {
        printf("Error saving products\n");
    }

    if (saveCustomers(CUSTOMERS_FILE, superMarket->customers, superMarket->customerAmount)) {
        printf("Customers saved successfully (%d customers)\n", superMarket->customerAmount);
    } else {
        printf("Error saving customers\n");
    }
}

// ===================== Finalization =====================

void finalizeSuperMarket(SuperMarket* superMarket) {
    for (int i = 0; i < superMarket->customerAmount; i++) {
        Customer* customer = &superMarket->customers[i];
        if (customer->shopping_cart.productCount != 0) {
            processPayment(customer);
        }
    }

    // Save data before freeing
    saveAllData(superMarket);

    freeSuperMarket(superMarket);
}

void freeSuperMarket(SuperMarket* superMarket) {
    if (!superMarket) return;

    // 1) Free the market name
    free(superMarket->name);
    superMarket->name = NULL;

    // 2) Free the array of customers
    //    (But first free each customer's internals)
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