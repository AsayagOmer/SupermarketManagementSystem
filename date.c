//
// Created by Tom on 29.12.2024.
//

#include "date.h"

#include <stdio.h>
#include <stdlib.h>

// Sort of Constructor
int init_date(Date* date) {
    // If Allocation is Failed.
    if (!date)
        return 0;

    do{} while (!get_date_from_user(date) || !is_valid_date(date));

    return 1;
}
