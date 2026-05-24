//
// Created by Tom on 29.12.2024.
//

#ifndef INPUT_H
#define INPUT_H

#define MAX_STR_INPUT 255
char* getUserInputString(const char* prompt);
void* safeRealloc(void* base, size_t new_size);
int arraySearch(const void* arr, size_t len, size_t elem_size, const void* val, int (*equals)(const void*, const void*));
int stringEquals(const void* s1, const void* s2);
void printArray(const void* arr, size_t len, size_t elem_size, void (*print) (const void*));
float getNonNegativeFloatFromUser(const char* prompt);
char* myGets(const char* prompt, char* buffer, int bufferSize);
#endif //INPUT_H
