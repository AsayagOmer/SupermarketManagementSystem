/*
 * vtable.c — Virtual Table implementation for Product type polymorphism.
 *
 * Provides per-ProductType implementations of:
 *   - print        (formatted product output)
 *   - calcDiscount (bulk-quantity discount logic)
 *   - isExpired    (date-based expiry check)
 *
 * The global array `productVTables` is indexed by ProductType and used
 * for polymorphic dispatch throughout the supermarket system.
 */

#include <stdio.h>
#include "product.h"   /* Brings in Product, Date, vtable.h, date.h */

/* ------------------------------------------------------------------ */
/*                      Number of product types                       */
/* ------------------------------------------------------------------ */
#define VTABLE_COUNT 4  /* Must match NofTypes (excluding the sentinel) */

/* ================================================================== */
/*                       Print functions                              */
/* ================================================================== */

/*
 * Common column layout (mirrors the existing print_product format):
 *   Name (21)  Barcode (11)  Type (22)  Price  Amount  Expiry
 */

static void printFruitVegetable(const Product* p)
{
    if (!p) return;
    printf("%-21s%-11s%-22s%-16.2f%-16d",
           p->name, p->barcode, "Fruit & Vegetable",
           p->price, p->amount);
    printf("%02d/%02d/%04d",
           p->expiration_date.day,
           p->expiration_date.month,
           p->expiration_date.year);
    printf("  [Fresh]\n");
}

static void printFridge(const Product* p)
{
    if (!p) return;
    printf("%-21s%-11s%-22s%-16.2f%-16d",
           p->name, p->barcode, "Fridge",
           p->price, p->amount);
    printf("%02d/%02d/%04d",
           p->expiration_date.day,
           p->expiration_date.month,
           p->expiration_date.year);
    printf("  [Cold Storage]\n");
}

static void printFrozen(const Product* p)
{
    if (!p) return;
    printf("%-21s%-11s%-22s%-16.2f%-16d",
           p->name, p->barcode, "Frozen",
           p->price, p->amount);
    printf("%02d/%02d/%04d",
           p->expiration_date.day,
           p->expiration_date.month,
           p->expiration_date.year);
    printf("  [Frozen Storage]\n");
}

static void printShelf(const Product* p)
{
    if (!p) return;
    printf("%-21s%-11s%-22s%-16.2f%-16d",
           p->name, p->barcode, "Shelf",
           p->price, p->amount);
    printf("%02d/%02d/%04d",
           p->expiration_date.day,
           p->expiration_date.month,
           p->expiration_date.year);
    printf("\n");
}

/* ================================================================== */
/*                     Discount calculators                           */
/* ================================================================== */

/*
 * Each function returns the effective price after applying a type-specific
 * bulk discount.  If the quantity threshold is not met, the original price
 * is returned unchanged.
 *
 *   FruitVegetable — 15 % off when amount > 10
 *   Fridge         — 10 % off when amount > 20
 *   Frozen         — 20 % off when amount > 30
 *   Shelf          —  5 % off when amount > 50
 */

static float discountFruitVegetable(const Product* p)
{
    if (!p) return 0.0f;
    if (p->amount > 10)
        return p->price * 0.85f;   /* 15 % discount */
    return p->price;
}

static float discountFridge(const Product* p)
{
    if (!p) return 0.0f;
    if (p->amount > 20)
        return p->price * 0.90f;   /* 10 % discount */
    return p->price;
}

static float discountFrozen(const Product* p)
{
    if (!p) return 0.0f;
    if (p->amount > 30)
        return p->price * 0.80f;   /* 20 % discount */
    return p->price;
}

static float discountShelf(const Product* p)
{
    if (!p) return 0.0f;
    if (p->amount > 50)
        return p->price * 0.95f;   /* 5 % discount */
    return p->price;
}

/* ================================================================== */
/*                      Expiry validators                             */
/* ================================================================== */

/*
 * All four type validators share the same calendar comparison logic:
 *   1. expiry year  < current year  → expired
 *   2. same year  and expiry month < current month → expired
 *   3. same year+month and expiry day < current day → expired
 *
 * Returns 1 if expired, 0 otherwise.
 */

static int isExpiredFruitVegetable(const Product* p,
                                   int d, int m, int y)
{
    if (!p) return 1;
    if (p->expiration_date.year < y) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month < m) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month == m
        && p->expiration_date.day < d) return 1;
    return 0;
}

static int isExpiredFridge(const Product* p,
                           int d, int m, int y)
{
    if (!p) return 1;
    if (p->expiration_date.year < y) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month < m) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month == m
        && p->expiration_date.day < d) return 1;
    return 0;
}

static int isExpiredFrozen(const Product* p,
                           int d, int m, int y)
{
    if (!p) return 1;
    if (p->expiration_date.year < y) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month < m) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month == m
        && p->expiration_date.day < d) return 1;
    return 0;
}

static int isExpiredShelf(const Product* p,
                          int d, int m, int y)
{
    if (!p) return 1;
    if (p->expiration_date.year < y) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month < m) return 1;
    if (p->expiration_date.year == y && p->expiration_date.month == m
        && p->expiration_date.day < d) return 1;
    return 0;
}

/* ================================================================== */
/*                   Global V-Table array                             */
/* ================================================================== */

/*
 * Indexed by ProductType enum:
 *   [0] FruitVegetable
 *   [1] Fridge
 *   [2] Frozen
 *   [3] Shelf
 */
const ProductVTable productVTables[VTABLE_COUNT] = {
    { printFruitVegetable, discountFruitVegetable, isExpiredFruitVegetable, "Fruit & Vegetable" },
    { printFridge,         discountFridge,         isExpiredFridge,         "Fridge"             },
    { printFrozen,         discountFrozen,         isExpiredFrozen,        "Frozen"             },
    { printShelf,          discountShelf,          isExpiredShelf,         "Shelf"              }
};

/* ================================================================== */
/*                    V-Table accessor                                */
/* ================================================================== */

const ProductVTable* getVTableForType(int type)
{
    if (type < 0 || type >= VTABLE_COUNT) {
        fprintf(stderr, "Error: invalid ProductType %d (valid range 0..%d)\n",
                type, VTABLE_COUNT - 1);
        return NULL;
    }
    return &productVTables[type];
}
