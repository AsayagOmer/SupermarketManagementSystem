
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analytics.h"


void analyticsMenu(void) {
    int choice;
    do {
        printf("\n--- Data Extraction & Analytics ---\n");
        printf("1 - Customer Purchase History\n");
        printf("2 - Top Customers by Spending\n");
        printf("3 - Top Products by Units Sold\n");
        printf("4 - Revenue Report\n");
        printf("0 - Back to Main Menu\n");

        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: showCustomerPurchaseHistory(); break;
            case 2: showTopCustomers(); break;
            case 3: showTopProducts(); break;
            case 4: showRevenueReport(); break;
            case 0: break;
            default: printf("Wrong option\n");
        }
    } while (choice != 0);
}

void showCustomerPurchaseHistory(void) {
    char searchId[10];
    printf("Enter customer ID (9 digits): ");
    scanf("%9s", searchId);
    while (getchar() != '\n');

    FILE* fp = fopen(PURCHASE_HISTORY_PATH, "r");
    if (!fp) {
        printf("No purchase history available.\n");
        return;
    }

    char line[512];
    int purchaseCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n\r")] = '\0';

        if (strcmp(line, "=== PURCHASE ===") == 0) {
            // Read CustomerID line
            char customerId[10] = {0};
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';
            sscanf(line, "CustomerID: %9s", customerId);

            // Read CustomerName line (skip it for display matching)
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';

            // Read "Items:" line
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';

            // Collect item lines and total
            char itemLines[50][512];
            int itemCount = 0;
            float total = 0.0f;

            while (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n\r")] = '\0';

                if (strncmp(line, "Total:", 6) == 0) {
                    sscanf(line, "Total: %f", &total);
                    // Read closing separator
                    if (fgets(line, sizeof(line), fp)) {
                        // consume "================"
                    }
                    break;
                }

                if (itemCount < 50) {
                    strcpy(itemLines[itemCount], line);
                    itemCount++;
                }
            }

            // Check if this purchase belongs to the searched customer
            if (strcmp(customerId, searchId) == 0) {
                purchaseCount++;
                printf("\nPurchase #%d:\n", purchaseCount);
                printf("  Items:\n");
                for (int i = 0; i < itemCount; i++) {
                    printf("  %s\n", itemLines[i]);
                }
                printf("  Total: %.2f\n", total);
            }
        }
    }

    fclose(fp);

    if (purchaseCount == 0) {
        printf("No purchases found for customer ID: %s\n", searchId);
    } else {
        printf("\nTotal purchases found: %d\n", purchaseCount);
    }
}

void showTopCustomers(void) {
    FILE* fp = fopen(PURCHASE_HISTORY_PATH, "r");
    if (!fp) {
        printf("No purchase history available.\n");
        return;
    }

    CustomerStats stats[MAX_ANALYTICS_ENTRIES];
    int statsCount = 0;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n\r")] = '\0';

        if (strcmp(line, "=== PURCHASE ===") == 0) {
            // Read CustomerID line
            char customerId[10] = {0};
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';
            sscanf(line, "CustomerID: %9s", customerId);

            // Read CustomerName line
            char customerName[256] = {0};
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';
            const char* nameStart = strstr(line, "CustomerName: ");
            if (nameStart) {
                strncpy(customerName, nameStart + 14, sizeof(customerName) - 1);
                customerName[sizeof(customerName) - 1] = '\0';
            }

            // Read "Items:" line
            if (!fgets(line, sizeof(line), fp)) break;
            line[strcspn(line, "\n\r")] = '\0';

            // Skip item lines until Total
            float total = 0.0f;
            while (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n\r")] = '\0';
                if (strncmp(line, "Total:", 6) == 0) {
                    sscanf(line, "Total: %f", &total);
                    // Read closing separator
                    if (fgets(line, sizeof(line), fp)) {
                        // consume "================"
                    }
                    break;
                }
            }

            // Find or create CustomerStats entry
            int found = -1;
            for (int i = 0; i < statsCount; i++) {
                if (strcmp(stats[i].customerId, customerId) == 0) {
                    found = i;
                    break;
                }
            }

            if (found != -1) {
                stats[found].totalPurchases++;
                stats[found].totalSpent += total;
            } else if (statsCount < MAX_ANALYTICS_ENTRIES) {
                strncpy(stats[statsCount].customerId, customerId, sizeof(stats[statsCount].customerId) - 1);
                stats[statsCount].customerId[sizeof(stats[statsCount].customerId) - 1] = '\0';
                strncpy(stats[statsCount].customerName, customerName, sizeof(stats[statsCount].customerName) - 1);
                stats[statsCount].customerName[sizeof(stats[statsCount].customerName) - 1] = '\0';
                stats[statsCount].totalPurchases = 1;
                stats[statsCount].totalSpent = total;
                statsCount++;
            }
        }
    }

    fclose(fp);

    if (statsCount == 0) {
        printf("No customer data found.\n");
        return;
    }

    // Selection sort by totalSpent descending
    for (int i = 0; i < statsCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < statsCount; j++) {
            if (stats[j].totalSpent > stats[maxIdx].totalSpent) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            CustomerStats temp = stats[i];
            stats[i] = stats[maxIdx];
            stats[maxIdx] = temp;
        }
    }

    int displayCount = statsCount < 10 ? statsCount : 10;

    printf("\n--- Top Customers by Spending ---\n");
    printf("%-6s%-21s%-13s%-12s%s\n", "Rank", "Name", "ID", "Purchases", "Total Spent");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < displayCount; i++) {
        printf("%-6d%-21s%-13s%-12d$%.2f\n",
            i + 1,
            stats[i].customerName,
            stats[i].customerId,
            stats[i].totalPurchases,
            stats[i].totalSpent);
    }
}

void showTopProducts(void) {
    FILE* fp = fopen(PURCHASE_HISTORY_PATH, "r");
    if (!fp) {
        printf("No purchase history available.\n");
        return;
    }

    ProductStats stats[MAX_ANALYTICS_ENTRIES];
    int statsCount = 0;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n\r")] = '\0';

        if (strcmp(line, "=== PURCHASE ===") == 0) {
            // Skip CustomerID line
            if (!fgets(line, sizeof(line), fp)) break;
            // Skip CustomerName line
            if (!fgets(line, sizeof(line), fp)) break;
            // Skip "Items:" line
            if (!fgets(line, sizeof(line), fp)) break;

            // Parse item lines until Total
            while (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n\r")] = '\0';

                if (strncmp(line, "Total:", 6) == 0) {
                    // Read closing separator
                    if (fgets(line, sizeof(line), fp)) {
                        // consume "================"
                    }
                    break;
                }

                // Parse item line: "  <barcode> x<amount> @ <price>"
                char barcode[8] = {0};
                int amount = 0;
                float price = 0.0f;

                // Trim leading whitespace and parse
                const char* p = line;
                while (*p == ' ') p++;

                if (sscanf(p, "%7s x%d @ %f", barcode, &amount, &price) == 3) {
                    // Find or create ProductStats entry
                    int found = -1;
                    for (int i = 0; i < statsCount; i++) {
                        if (strcmp(stats[i].barcode, barcode) == 0) {
                            found = i;
                            break;
                        }
                    }

                    if (found != -1) {
                        stats[found].totalUnitsSold += amount;
                        stats[found].totalRevenue += amount * price;
                    } else if (statsCount < MAX_ANALYTICS_ENTRIES) {
                        strncpy(stats[statsCount].barcode, barcode, sizeof(stats[statsCount].barcode) - 1);
                        stats[statsCount].barcode[sizeof(stats[statsCount].barcode) - 1] = '\0';
                        stats[statsCount].totalUnitsSold = amount;
                        stats[statsCount].totalRevenue = amount * price;
                        statsCount++;
                    }
                }
            }
        }
    }

    fclose(fp);

    if (statsCount == 0) {
        printf("No product data found.\n");
        return;
    }

    // Selection sort by totalUnitsSold descending
    for (int i = 0; i < statsCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < statsCount; j++) {
            if (stats[j].totalUnitsSold > stats[maxIdx].totalUnitsSold) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            ProductStats temp = stats[i];
            stats[i] = stats[maxIdx];
            stats[maxIdx] = temp;
        }
    }

    int displayCount = statsCount < 10 ? statsCount : 10;

    printf("\n--- Top Products by Units Sold ---\n");
    printf("%-6s%-11s%-13s%s\n", "Rank", "Barcode", "Units Sold", "Revenue");
    printf("------------------------------------------------\n");

    for (int i = 0; i < displayCount; i++) {
        printf("%-6d%-11s%-13d$%.2f\n",
            i + 1,
            stats[i].barcode,
            stats[i].totalUnitsSold,
            stats[i].totalRevenue);
    }
}

void showRevenueReport(void) {
    FILE* fp = fopen(PURCHASE_HISTORY_PATH, "r");
    if (!fp) {
        printf("No purchase history available.\n");
        return;
    }

    int totalTransactions = 0;
    float totalRevenue = 0.0f;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n\r")] = '\0';

        if (strcmp(line, "=== PURCHASE ===") == 0) {
            totalTransactions++;

            // Skip CustomerID, CustomerName, Items lines
            if (!fgets(line, sizeof(line), fp)) break;
            if (!fgets(line, sizeof(line), fp)) break;
            if (!fgets(line, sizeof(line), fp)) break;

            // Skip item lines until Total
            while (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n\r")] = '\0';
                if (strncmp(line, "Total:", 6) == 0) {
                    float total = 0.0f;
                    sscanf(line, "Total: %f", &total);
                    totalRevenue += total;
                    // Read closing separator
                    if (fgets(line, sizeof(line), fp)) {
                        // consume "================"
                    }
                    break;
                }
            }
        }
    }

    fclose(fp);

    if (totalTransactions == 0) {
        printf("No transactions found.\n");
        return;
    }

    float avgTransaction = totalRevenue / totalTransactions;

    printf("\n--- Revenue Report ---\n");
    printf("Total Transactions:       %d\n", totalTransactions);
    printf("Total Revenue:            $%.2f\n", totalRevenue);
    printf("Average Transaction:      $%.2f\n", avgTransaction);
}
