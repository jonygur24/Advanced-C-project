#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "doctors.h"
#include "common.h"

// Loads a list of doctors from a file and returns a linked list of doctor nodes.
DocNode* loadDoctors()
{
	DocNode* head = NULL;
	char buffer[256];
	FILE* file = fopen("Doctors.txt", "r");
	if (file == NULL)
	{
		displayError("Error opening file");
		return NULL;
	}
	fgets(buffer, sizeof(buffer), file);
	fgets(buffer, sizeof(buffer), file);
	while (fgets(buffer, sizeof(buffer), file))
	{
		char* token = strtok(buffer, "\n");
		DocNode* newNode = ParseDoctor(token);
		head = PushToEndOfNode(head, newNode);
	}
	return head;
}

// Function to remove a node by Doc
void removeNodeByDocPtr(DocNode** head, Doc* doc) {
	DocNode* temp = *head;
	DocNode* prev = NULL;

	while (temp != NULL) {
		if (strcmp(temp->doctor.Name, doc->Name) == 0 &&
			strncmp(temp->doctor.nLicense, doc->nLicense, 6) == 0 &&
			temp->doctor.nPatients == doc->nPatients) {
			if (prev == NULL) {
				*head = temp->next;
			}
			else {
				prev->next = temp->next;
			}

			free(temp->doctor.Name);
			free(temp);
			return;
		}
		prev = temp;
		temp = temp->next;
	}

	printf("No matching doctor found\n");
}

// Parses a string representation of a doctor and creates a new `DocNode` object.
DocNode* ParseDoctor(char* str)
{
	char* token = NULL;
	DocNode* node = (DocNode*)malloc(sizeof(DocNode));
	if (node == NULL)
	{
		displayError("allocation failed\n");
		return NULL;
	}
	token = strtok(str, ";");
	node->doctor.Name = _strdup(token);
	token = strtok(NULL, ";");
	strcpy(node->doctor.nLicense, token + 1);
	token = strtok(NULL, "\n");
	node->doctor.nPatients = ConvertNumber(token + 1);
	node->next = NULL;
	return node;
}

//  Searches for a doctor with the given name in the linked list.
DocNode* SearchDoc(char* str, DocNode* head)
{
	while (head != NULL)
	{
		if (strcmp(head->doctor.Name, str) == 0)
			break;
		head = head->next;
	}
	return head;
}

// Adds a new node to the end of the linked list of DocNodes.
DocNode* PushToEndOfNode(DocNode* head, DocNode* newNode)
{
	if (head == NULL)
		head = newNode;
	else
	{
		DocNode* temp = head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newNode;
	}
	return head;
}


// This function gets the doctor with the least number of patients.
// If there are multiple doctors with the same number of patients,
// it returns the first one in the list.
DocNode* getMostAvailableDoc(DocNode* head)
{
	DocNode* minPatients = head;
	DocNode* current = head->next;

	while (current != NULL)
	{
		if (minPatients->doctor.nPatients > current->doctor.nPatients)
		{
			minPatients = current;
		}
		current = current->next;
	}

	return minPatients;
}


// get from the user the name of the doctor and display all the patients of that doctor
// (name, ID, arrival)
// Make sure that the arrival date comes from a visit for the same doctor
void assignDoctor2case(DocNode* head, Visit* newVisit) {

	DocNode* current = getMostAvailableDoc(head);
	newVisit->Doctor = &current->doctor;
	current->doctor.nPatients++;
}

// Adds a new node to the end of the linked list of DocNodes.
void addDocNode(DocNode** head, char* name, char* nLicense, int nPatients) {
	DocNode* newNode = initializeNode(name, nLicense, nPatients);
	if (*head == NULL) {
		*head = newNode;
	}
	else {
		DocNode* temp = *head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}

// Function to initialize a new DocNode
DocNode* initializeNode(char* name, char* nLicense, int nPatients) {
	DocNode* newNode = (DocNode*)malloc(sizeof(DocNode));
	if (newNode == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	newNode->doctor.Name = _strdup(name);
	strncpy(newNode->doctor.nLicense, nLicense, 6);
	newNode->doctor.nLicense[6] = '\0';  // Ensure null-termination
	newNode->doctor.nPatients = nPatients;
	newNode->next = NULL;
	return newNode;
}

// Frees a linked list of DocNode structures, including the dynamically allocated 
// memory for each doctor's name. The function takes a pointer to the head of the 
// list as a parameter and does not return a value.
void freeDocList(DocNode* node) {
	while (node != NULL) {
		DocNode* temp = node;
		node = node->next;
		free(temp->doctor.Name); // Free dynamically allocated Name
		free(temp); // Free the DocNode
	}
}

// Display list of doctors and the number of patients
void displayAllDocs(DocNode* head)
{
	DocNode* current = head;
	while (current != NULL)
	{
		printf("Doctor: %s\n", current->doctor.Name);
		printf("Number of Patients: %d\n", current->doctor.nPatients);
		printf("\n");
		current = current->next;
	}
}