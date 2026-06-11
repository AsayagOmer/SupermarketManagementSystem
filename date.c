
#include "date.h"

#include <stdio.h>
#include <stdlib.h>

// Days per month lookup (index 0 unused, 1=Jan..12=Dec)
static const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Sort of Constructor
int init_date(Date* date) {
    // If Allocation is Failed.
    if (!date)
        return 0;

    do{} while (!get_date_from_user(date) || !is_valid_date(date));

    return 1;
}

int get_date_from_user(Date* date) {
    if (!date)
        return 0;

    printf("Enter expiration date (DD/MM/YYYY): ");
    if (scanf("%d/%d/%d", &date->day, &date->month, &date->year) != 3) {
        printf("Invalid date format! Use DD/MM/YYYY\n");
        // Clear input buffer
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');
    return 1;
}

int is_valid_date(const Date* date) {
    if (!date)
        return 0;

    // Validate year range
    if (date->year < FIRST_VALID_YEAR || date->year > LAST_VALID_YEAR) {
        printf("Year must be between %d and %d\n", FIRST_VALID_YEAR, LAST_VALID_YEAR);
        return 0;
    }

    // Validate month
    if (date->month < FIRST_MONTH_OF_YEAR || date->month > MONTHS_IN_YEAR) {
        printf("Month must be between 1 and 12\n");
        return 0;
    }

    // Calculate max days for the given month
    int maxDays = daysInMonth[date->month];
    if (date->month == 2 && isLeapYear(date->year)) {
        maxDays = 29;
    }

    // Validate day
    if (date->day < 1 || date->day > maxDays) {
        printf("Day must be between 1 and %d for month %d\n", maxDays, date->month);
        return 0;
    }

    return 1;
}

void print_date(const Date* date) {
    if (!date) {
        printf("N/A");
        return;
    }
    printf("%02d/%02d/%04d", date->day, date->month, date->year);
}
