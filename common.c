#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "stdbool.h"
#include "structs.h"
#include "common.h"

#define END_OF_LINE  "\r\n"
int ConvertNumber(char* str)
{
	int num = 0;
	if (str == NULL)
	{
		return -1;
	}

	while (*str)
	{
		if (*str < '0' || *str > '9')
		{
			return num;
		}
		num += *str - 48;
		if (str[1] && is_number(str[1]))
			num *= 10;
		str++;
	}
	return num;
}

bool is_ascii(char ch)
{
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == ' ';
}

bool is_number(char ch)
{
	return ch >= '0' && ch <= '9';
}

void remove_non_ascii(char* str) {
	int write_index = 0;
	int read_index = 0;

	while (str[read_index] != '\0') {
		if (is_ascii(str[read_index])) {
			str[write_index++] = str[read_index];
		}
		read_index++;
	}

	str[write_index] = '\0';
}

void displayError(char* error)
{
	fprintf(stderr, "%s", error);
}

char Allergies(char* str)
{
	char all = 0, * token;
	token = strtok(str, ",");
	do
	{
		if (strncmp("none", token, strlen("none")) == 0)
			all += NONE;
		if (strncmp("Penicillin", token, strlen("Penicillin")) == 0)
			all += PENICILLIN;
		if (strncmp("Sulfa", token, strlen("Sulfa")) == 0)
			all += SULFA;
		if (strncmp("Opioids", token, strlen("Opioids")) == 0)
			all += OPIOIDS;
		if (strncmp("Anesthetics", token, strlen("Anesthetics")) == 0)
			all += ANESTHETICS;
		if (strncmp("Eggs", token, strlen("Eggs")) == 0)
			all += EGGS;
		if (strncmp("Latex", token, strlen("Latex")) == 0)
			all += LATEX;
		if (strncmp("Preservatives", token, strlen("Preservatives")) == 0)
			all += PRESERVATIVES;
		token = strtok(NULL, ",");
	} while (token != NULL);
	return all;
}

char* AllergiesNumberToString(char* str)
{
	char* result = (char*)malloc(255);
	result[0] = '\0';

	char* token = strtok(str, ",");
	while (token != NULL) {
		int code = atoi(token);
		switch (code) {
		case 0:
			strcat(result, "none,");
			break;
		case 1:
			strcat(result, "Penicillin,");
			break;
		case 2:
			strcat(result, "Sulfa,");
			break;
		case 3:
			strcat(result, "Opioids,");
			break;
		case 4:
			strcat(result, "Anesthetics,");
			break;
		case 5:
			strcat(result, "Eggs,");
			break;
		case 6:
			strcat(result, "Latex,");
			break;
		case 7:
			strcat(result, "Preservatives,");
			break;
		}
		token = strtok(NULL, ",");
	}

	// remove the last comma and space
	int len = strlen(result);
	if (len >= 1) {
		result[len - 1] = '\0';
	}

	return result;
}

// a function that translates the allergy bitmap to string
char* AllergiesToString(char all)
{
	char* result = (char*)malloc(100);
	result[0] = '\0';

	if (all & NONE)
		strcat(result, "none, ");
	if (all & PENICILLIN)
		strcat(result, "Penicillin, ");
	if (all & SULFA)
		strcat(result, "Sulfa, ");
	if (all & OPIOIDS)
		strcat(result, "Opioids, ");
	if (all & ANESTHETICS)
		strcat(result, "Anesthetics, ");
	if (all & EGGS)
		strcat(result, "Eggs, ");
	if (all & LATEX)
		strcat(result, "Latex, ");
	if (all & PRESERVATIVES)
		strcat(result, "Preservatives, ");

	// remove the last comma and space
	int len = strlen(result);
	if (len >= 2)
	{
		result[len - 2] = '\0';
	}

	return result;

}

// compare date, return 1 if date1 > date2, 0 if date1 == date2 and -1 if date1 < date2
int CompareDates(Date* date1, Date* date2)
{
	if (date1->Year > date2->Year)
		return 1;
	else if (date1->Year < date2->Year)
		return -1;
	else if (date1->Month > date2->Month)
		return 1;
	else if (date1->Month < date2->Month)
		return -1;
	else if (date1->Day > date2->Day)
		return 1;
	else if (date1->Day < date2->Day)
		return -1;
	else if (date1->Hour > date2->Hour)
		return 1;
	else if (date1->Hour < date2->Hour)
		return -1;
	else if (date1->Min > date2->Min)
		return 1;
	else if (date1->Min < date2->Min)
		return -1;
	return 0;  // dates are equal
}

char* strip_non_printable(char* str) {
	int len = strlen(str);
	char buffer[256];  // Create a buffer on the stack

	char* src = str, * dst = buffer;
	while (*src) {
		if (is_ascii(*src)) {
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';

	strcpy(str, buffer);  // Copy the cleaned string back to the original string
	return str;
}

char* safePromptForStringPtr(char* prompt)
{
	char* buffer = malloc(255 * sizeof(char));
	int buffer_size = 255 * sizeof(char);
	int bytes_read = 0;

	while (1) {
		printf("%s", prompt);

		fgets(buffer, buffer_size, stdin);
		// bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1) {
			/* End of file or error */
			free(buffer);
			return NULL;
		}


		if (strlen(strip_non_printable(buffer)) > 0) {
			/* Non-empty string with only ASCII characters */
			return buffer;
		}
		else {
			printf("Input cannot be empty. Please try again.\n");
		}
	}
	return NULL;
}

void getValidString(char* prompt, char* buffer, int size) { // Function to get valid string input from the user
	while (1) {
		printf("%s", prompt);
		fgets(buffer, sizeof(buffer), stdin);
		if (fgets(buffer, size, stdin) != NULL && strchr(buffer, '\n') != NULL) {
			buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character if present
			if (strlen(buffer) > 0) {
				break;
			}
			else {
				printf("Input cannot be empty. Please try again.\n");
			}
		}
		else {

			printf("Input error. Please try again.\n");
		}
	}
}

char* safePromptForString(char* prompt, char* buffer)
{
	int buffer_size = 255 * sizeof(char);
	int bytes_read = 0;

	while (1) {
		printf("%s", prompt);
		fgets(buffer, buffer_size, stdin);
		if (strlen(buffer) < 0) {
			/* End of file or error */
			free(buffer);
			return NULL;
		}

		if (strlen(strip_non_printable(buffer)) > 0) {
			/* Non-empty string with only ASCII characters */
			return buffer;
		}
		else {
			*buffer = 0;
			displayError("Input cannot be empty. Please try again.\n");
		}
	}

	*buffer = 0;
}

char* safePromptForStringWSpecial(char* prompt, char* buffer)
{
	int buffer_size = 255 * sizeof(char);
	int bytes_read = 0;

	while (1) {
		printf("%s", prompt);
		fgets(buffer, buffer_size, stdin);
		if (strlen(buffer) < 0) {
			/* End of file or error */
			free(buffer);
			return NULL;
		}

		if (strlen(buffer) > 0) {
			/* Non-empty string with only ASCII characters */
			return buffer;
		}
		else {
			*buffer = 0;
			displayError("Input cannot be empty. Please try again.\n");
		}
	}

	*buffer = 0;
}

int isValidDate(Date* D) { // Function to check if the date and time input is valid
	if (D->Day < 1 || D->Day > 31) return 0;
	if (D->Month < 1 || D->Month > 12) return 0;
	if (D->Hour < 0 || D->Hour > 23) return 0;
	if (D->Min < 0 || D->Min > 59) return 0;
	return 1;
}

int getValidDate(char* prompt, Date* D) {
	char buffer[120] = { 0 }; // Adjust buffer size if needed
	if (safePromptForStringWSpecial(prompt, buffer) != NULL && strlen(buffer) > 0) {
		ParseDate(buffer, D);
		if (isValidDate(D)) {
			return 0;
		}
		else {
			displayError("Invalid date or time. Please try again.\n");
			return -1;
		}
	}
	else {
		displayError("Input error. Please try again.\n");
		return -1;
	}

	return 0;
}

void ParseDate(char* str, Date* D)
{
	if (str != NULL && str != END_OF_LINE)
	{
		str = strtok(str, "/");
		D->Day = ConvertNumber(str);
		str = strtok(NULL, "/");
		D->Month = ConvertNumber(str);
		str = strtok(NULL, " ");
		D->Year = ConvertNumber(str);
		str = strtok(NULL, ":");
		D->Hour = ConvertNumber(str);
		str = strtok(NULL, "\n");
		D->Min = ConvertNumber(str);
	}
	else
	{
		D->Day = -1;
		D->Month = -1;
		D->Year = -1;
		D->Hour = -1;
		D->Min = -1;
	}
}

float ParseDuration(char* str)
{
	float res = 0;
	char* token = strtok(str, ":");
	res += ConvertNumber(str) * 100;
	str = strtok(NULL, "\n");
	res += ConvertNumber(str);
	res /= 100;
	return res;
}

pInTree* findMin(pInTree* node) {
	while (node && node->left != NULL) {
		node = node->left;
	}
	return node;
}

void formatDuration(char* buffer, float duration) {
	int hours = (int)duration;
	int minutes = (int)((duration - hours) * 60);
	sprintf(buffer, "%d:%02d", hours, minutes);
}

void formatDate(char* buffer, Date date) {
	sprintf(buffer, "%02d/%02d/%04d %02d:%02d", date.Day, date.Month, date.Year, date.Hour, date.Min);
}

void clearBuffer()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

void getValidID(char* id) {
	char buffer[256];
	while (1) {
		printf("Please enter patient's ID: ");
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer, "%s", buffer);

		// Check if the input is exactly 9 digits long and only contains numbers
		if (strlen(buffer) == 9 && strspn(buffer, "0123456789") == 9) {
			strcpy(id, buffer);
			break;
		}
		else {
			printf("Input error: ID must be exactly 9 digits long and contain only numbers, please try again.\n");
		}
	}
}
void capitalizeFirstLetter(char* str) {
	int capitalize = 1; // Flag to determine if the next character should be capitalized

	while (*str) {
		if (isspace(*str)) {
			capitalize = 1; 
		}
		else if (capitalize && isalpha(*str)) {
			*str = toupper(*str);
			capitalize = 0;
		}
		else {
			*str = tolower(*str); 
		}
		str++;
	}
}
