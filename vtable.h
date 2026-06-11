#ifndef VTABLE_H
#define VTABLE_H

/*
 * vtable.h — Virtual Table definitions for Product type polymorphism.
 *
 * Each ProductType (FruitVegetable, Fridge, Frozen, Shelf) has its own
 * vtable entry containing type-specific function pointers for printing,
 * discount calculation, and expiry validation.
 *
 * To avoid circular includes (product.h <-> vtable.h), Product is
 * forward-declared here. product.h includes vtable.h and provides
 * the full struct definition.
 */

/* Forward declare Product to avoid circular dependency with product.h */
typedef struct Product Product;

/*
 * ProductVTable — Virtual function table for Product types.
 *
 * Members:
 *   print        — Type-specific formatted print of the product.
 *   calcDiscount — Returns the (possibly discounted) price for the product.
 *   isExpired    — Returns 1 if the product is expired relative to the
 *                  given current date, 0 otherwise.
 *   typeName     — Human-readable name for the product type.
 */
typedef struct ProductVTable {
    void  (*print)(const Product* product);
    float (*calcDiscount)(const Product* product);
    int   (*isExpired)(const Product* product,
                       int currentDay, int currentMonth, int currentYear);
    const char* typeName;
} ProductVTable;

/*
 * Global V-Table array — one entry per ProductType.
 * Indexed by the ProductType enum (0..NofTypes-1).
 * Defined in vtable.c.
 */
extern const ProductVTable productVTables[4];

/*
 * getVTableForType — Safely retrieve the vtable for a given ProductType.
 *
 * Parameters:
 *   type — integer value of the ProductType enum.
 *
 * Returns:
 *   Pointer to the matching ProductVTable, or NULL if type is out of range.
 */
const ProductVTable* getVTableForType(int type);

#endif /* VTABLE_H */
