//
// Created by Tom on 29.12.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int arraySearch(const void* arr, const size_t len, const size_t elem_size, const void* val, int (*equals)(const void*, const void*)) {
    for (int i = 0; i < len; i++) {
        const void* element = (char*) arr + i * elem_size;
        if (equals(element, val))
            return i;
    }
    return -1;
}

int stringEquals(const void* s1, const void* s2) {
    // s1 is a pointer to a pointer-to-char (element from shortTypeTitle)
    const char* str1 = *(const char**) s1;
    // s2 is just a pointer-to-char (your prefix)
    const char* str2 = (const char*) s2;
    return strcmp(str1, str2) == 0;
}


void* safeRealloc(void* base, const size_t new_size) {
    void* temp = realloc(base, new_size);
    if (!temp) {
        free(base);
    }
    return temp;
}

char* myGets(const char* prompt, char* buffer, const int bufferSize) {
    printf("%s\n", prompt);
    if (fgets(buffer, bufferSize, stdin) == NULL) {
        printf("Error getting input\n");
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

char* getUserInputString(const char* prompt) {
    char buffer[MAX_STR_INPUT];
    myGets(prompt, buffer, sizeof(buffer));
    const size_t len = strlen(buffer);

    char* back = malloc((len+1) * sizeof(char));
    if (back == NULL) {
        printf("Error allocating memory for input string\n");
        return NULL;
    }

    strcpy(back, buffer);
    return back;
}

void printArray(const void* arr, const size_t len, const size_t elem_size, void (*print) (const void*)) {
    for (int i = 0; i < len; i++) {
        const void* element = (char*) arr + i  * elem_size;
        print(element);
    }
}

float getNonNegativeFloatFromUser(const char* prompt) {
    float back;
    do {
        printf("%s\n",prompt);
        scanf("%f", &back);
    } while (back < 0);
    return back;
}