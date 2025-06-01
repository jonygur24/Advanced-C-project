#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "stdbool.h"
#include "structs.h"

int ConvertNumber(char* str);
bool is_number(char ch);
bool is_ascii(char ch);
void remove_non_ascii(char* str);
void displayError(char* error);
char Allergies(char* str);
char* AllergiesToString(char all);
int CompareDates(Date* date1, Date* date2);
char* strip_non_printable(char* str);
char* safePromptForString(char* prompt, char* buffer);
char* safePromptForStringPtr(char* prompt);
int isValidDate(Date* D);
int getValidDate(char* prompt, Date* D);
void ParseDate(char* str, Date* D);
float ParseDuration(char* str);
char* AllergiesNumberToString(char* str);
pInTree* findMin(pInTree* node);
void formatDuration(char* buffer, float duration);
void formatDate(char* buffer, Date date);
char* safePromptForStringWSpecial(char* prompt, char* buffer);
void getValidID(char* id);
void capitalizeFirstLetter(char* str);