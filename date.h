//
// Created by Tom on 29.12.2024.
//

#ifndef DATE_H
#define DATE_H

// Those macro can changed by request.
#define LAST_VALID_YEAR 2030
#define FIRST_VALID_YEAR 2024

#define MONTHS_IN_YEAR 12
#define FIRST_MONTH_OF_YEAR 1

typedef struct {
    int day, month, year;
} Date;

int init_date(Date *date);
#endif //DATE_H
