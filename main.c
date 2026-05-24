#include <stdio.h>

#include "supermarket.h"


int displayMainMenu() {
    printf("\nPlease choose one of the following options\n");
    printf("0 - Show Supermarket\n");
    printf("1 - Add Product\n");
    printf("2 - Add Customer\n");
    printf("3 - Customer Shopping\n");
    printf("4 - Print Shopping Cart\n");
    printf("5 - Customer Shopping Cart Management\n");
    printf("6 - Print product by type\n");
    printf("-1 - Quit\n");

    int userChoice;
    scanf("%d", &userChoice);
    while (getchar() != '\n');
    return userChoice;
}

int runFunction(SuperMarket* superMarket, const int choice) {
    switch (choice) {
        case 0: displaySuperMarket(superMarket); break;
        case 1: addProductToSuperMarket(superMarket); break;
        case 2: addCustomerToSuperMarket(superMarket); break;
        case 3:
            if (!shopping(superMarket)) {
                printf("Error in shopping\n");
            } else {
                printf("----------Shopping ended----------\n");
            }
            break;
        case 4: printShoppingCart(superMarket); break;
        case 5:
            if (!shoppingCartManagement(superMarket)) {
                printf("Error in shopping cart management\n");
            }
            break;
        case 6: printProductsByType(superMarket); break;
        case -1:
            printf("Bye bye\n");
            finalizeSuperMarket(superMarket);
            return 0;
        default:
            printf("Wrong option\n");
    }
    return 1;
}


int main() {
    SuperMarket superMarket;
    if (!initSuperMarket(&superMarket)) {
        return 1;
    }

    int choice;

    do {
        choice = displayMainMenu();
    } while (runFunction(&superMarket, choice));


    // Customer c;
    // initCustomer(&c);
    // printCustomer(&c);

}
