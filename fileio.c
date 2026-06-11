#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

#include "fileio.h"
#include "vtable.h"
#include "utils.h"

/* ------------------------------------------------------------------ */
/*  Helper: strip trailing newline / carriage return from fgets buffer */
/* ------------------------------------------------------------------ */
static void stripNewline(char* buf)
{
    if (buf)
        buf[strcspn(buf, "\n\r")] = '\0';
}

/* ------------------------------------------------------------------ */
/*  ensureDataDirectory – create the "data/" folder if it doesn't exist */
/* ------------------------------------------------------------------ */
int ensureDataDirectory(void)
{
    int result = MKDIR(DATA_DIR);
    if (result != 0 && errno != EEXIST) {
        printf("Error: could not create directory '%s'\n", DATA_DIR);
        return 0;
    }
    return 1;
}

/* ================================================================== */
/*                          P R O D U C T S                           */
/* ================================================================== */

/*
 * File format (text, one product per block, blocks separated by ---):
 *
 *   ProductName
 *   Barcode
 *   Type(int)
 *   Price
 *   Amount
 *   DD/MM/YYYY
 *   ---
 */

int saveProducts(const char* filename, Product** products, int count)
{
    if (!filename || !products)
        return 0;

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not open '%s' for writing\n", filename);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        const Product* p = products[i];
        if (!p)
            continue;

        fprintf(fp, "%s\n", p->name);
        fprintf(fp, "%s\n", p->barcode);
        fprintf(fp, "%d\n", (int)p->type);
        fprintf(fp, "%.2f\n", p->price);
        fprintf(fp, "%d\n", p->amount);
        fprintf(fp, "%02d/%02d/%04d\n",
                p->expiration_date.day,
                p->expiration_date.month,
                p->expiration_date.year);
        fprintf(fp, "---\n");
    }

    fclose(fp);
    return 1;
}

int loadProducts(const char* filename, Product*** products, int* count)
{
    if (!filename || !products || !count)
        return 0;

    *products = NULL;
    *count    = 0;

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        /* File doesn't exist yet – not an error, just no saved data. */
        return 1;
    }

    char line[256];

    while (1) {
        /* --- Read product name --- */
        if (!fgets(line, sizeof(line), fp))
            break;  /* EOF – done */
        stripNewline(line);

        /* Skip blank lines between blocks (defensive) */
        if (line[0] == '\0')
            continue;

        Product* p = (Product*)malloc(sizeof(Product));
        if (!p) {
            printf("Error: memory allocation failed while loading products\n");
            fclose(fp);
            return 0;
        }
        memset(p, 0, sizeof(Product));

        /* Name */
        strncpy(p->name, line, NAME_LEN - 1);
        p->name[NAME_LEN - 1] = '\0';

        /* Barcode */
        if (!fgets(line, sizeof(line), fp)) { free(p); break; }
        stripNewline(line);
        strncpy(p->barcode, line, BARCODE_LEN - 1);
        p->barcode[BARCODE_LEN - 1] = '\0';

        /* Type */
        int typeVal = 0;
        if (fscanf(fp, "%d", &typeVal) != 1) { free(p); break; }
        p->type = (ProductType)typeVal;

        /* Price */
        if (fscanf(fp, "%f", &p->price) != 1) { free(p); break; }

        /* Amount */
        if (fscanf(fp, "%d", &p->amount) != 1) { free(p); break; }

        /* Expiration date (DD/MM/YYYY) */
        if (fscanf(fp, "%d/%d/%d",
                   &p->expiration_date.day,
                   &p->expiration_date.month,
                   &p->expiration_date.year) != 3) {
            free(p);
            break;
        }

        /* Assign vtable based on product type */
        p->vtable = getVTableForType((int)p->type);

        /* Consume the rest of the date line (trailing newline) */
        if (fgets(line, sizeof(line), fp)) { /* discard */ }

        /* Read and discard the separator line "---" */
        if (fgets(line, sizeof(line), fp)) { /* discard */ }

        /* Grow the products array */
        (*count)++;
        Product** tmp = (Product**)safeRealloc(*products,
                                               sizeof(Product*) * (*count));
        if (!tmp) {
            printf("Error: memory allocation failed while loading products\n");
            free(p);
            *products = NULL;
            *count    = 0;
            fclose(fp);
            return 0;
        }
        *products = tmp;
        (*products)[*count - 1] = p;
    }

    fclose(fp);
    return 1;
}

/* ================================================================== */
/*                        C U S T O M E R S                           */
/* ================================================================== */

/*
 * File format (text, one customer per block, blocks separated by ---):
 *
 *   CustomerName
 *   CustomerID
 *   ---
 */

int saveCustomers(const char* filename, const Customer* customers, int count)
{
    if (!filename || !customers)
        return 0;

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not open '%s' for writing\n", filename);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        const Customer* c = &customers[i];
        fprintf(fp, "%s\n", c->name ? c->name : "");
        fprintf(fp, "%s\n", c->id);
        fprintf(fp, "---\n");
    }

    fclose(fp);
    return 1;
}

int loadCustomers(const char* filename, Customer** customers, int* count)
{
    if (!filename || !customers || !count)
        return 0;

    *customers = NULL;
    *count     = 0;

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        /* File doesn't exist yet – not an error, just no saved data. */
        return 1;
    }

    char line[256];

    while (1) {
        /* --- Read customer name --- */
        if (!fgets(line, sizeof(line), fp))
            break;  /* EOF – done */
        stripNewline(line);

        /* Skip blank lines between blocks (defensive) */
        if (line[0] == '\0')
            continue;

        /* Grow the customers array */
        (*count)++;
        Customer* tmp = (Customer*)safeRealloc(*customers,
                                                sizeof(Customer) * (*count));
        if (!tmp) {
            printf("Error: memory allocation failed while loading customers\n");
            *customers = NULL;
            *count     = 0;
            fclose(fp);
            return 0;
        }
        *customers = tmp;

        Customer* c = &(*customers)[*count - 1];
        memset(c, 0, sizeof(Customer));

        /* Name – malloc and copy */
        size_t nameLen = strlen(line);
        c->name = (char*)malloc(nameLen + 1);
        if (!c->name) {
            printf("Error: memory allocation failed for customer name\n");
            fclose(fp);
            return 0;
        }
        strcpy(c->name, line);

        /* ID */
        if (!fgets(line, sizeof(line), fp)) break;
        stripNewline(line);
        strncpy(c->id, line, ID_LEN - 1);
        c->id[ID_LEN - 1] = '\0';

        /* Shopping cart – zero-initialised (don't load cart state) */
        c->shopping_cart.head         = NULL;
        c->shopping_cart.productCount = 0;

        /* Read and discard the separator line "---" */
        if (fgets(line, sizeof(line), fp)) { /* discard */ }
    }

    fclose(fp);
    return 1;
}

/* ================================================================== */
/*                  P U R C H A S E   R E C O R D S                   */
/* ================================================================== */

/*
 * Append format:
 *
 *   === PURCHASE ===
 *   CustomerID: <id>
 *   CustomerName: <name>
 *   Items:
 *     <barcode> x<amount> @ <price>
 *     ...
 *   Total: <total>
 *   ================
 */

int savePurchaseRecord(const char* filename, const char* customerId,
                       const char* customerName, const ShoppingCart* cart,
                       float total)
{
    if (!filename || !customerId || !customerName || !cart)
        return 0;

    FILE* fp = fopen(filename, "a");
    if (!fp) {
        printf("Error: could not open '%s' for appending\n", filename);
        return 0;
    }

    fprintf(fp, "=== PURCHASE ===\n");
    fprintf(fp, "CustomerID: %s\n", customerId);
    fprintf(fp, "CustomerName: %s\n", customerName);
    fprintf(fp, "Items:\n");

    /* Iterate over the linked list of ShoppingItem nodes */
    ShoppingItem* current = cart->head;
    while (current) {
        fprintf(fp, "  %s x%d @ %.2f\n",
                current->barcode, current->amount, current->price);
        current = current->next;
    }

    fprintf(fp, "Total: %.2f\n", total);
    fprintf(fp, "================\n");

    fclose(fp);
    return 1;
}
