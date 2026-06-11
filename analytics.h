
#ifndef ANALYTICS_H
#define ANALYTICS_H

#define MAX_ANALYTICS_ENTRIES 500
#define PURCHASE_HISTORY_PATH "data/purchase_history.txt"

typedef struct {
    char customerId[10];    // 9 digits + null
    char customerName[256];
    int totalPurchases;
    float totalSpent;
} CustomerStats;

typedef struct {
    char barcode[8];
    int totalUnitsSold;
    float totalRevenue;
} ProductStats;

// Display the analytics sub-menu and handle user choices
void analyticsMenu(void);

// Individual analytics functions
void showCustomerPurchaseHistory(void);
void showTopCustomers(void);
void showTopProducts(void);
void showRevenueReport(void);

#endif // ANALYTICS_H
