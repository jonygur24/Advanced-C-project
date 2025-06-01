#define _CRT_SECURE_NO_WARNINGS
#include "pline.h"
#include "common.h"
#include "patient.h"
#include "structs.h"

// load line from file
pLine* loadLine(pTree* tree)
{
    char buffer[256];
    pLine* line = NULL;
    if (tree == NULL)
        return NULL;
    FILE* file = fopen("Line.txt", "r");
    if (file == NULL)
    {
        displayError("Error opening file");
        return NULL;
    }
    line = (pLine*)malloc(sizeof(pLine));
    if (line == NULL)
    {
        displayError("allocation failed");
        fclose(file);
        return NULL;
    }
    line->head = NULL;

    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file))
    {
        char* token = strtok(buffer, "\r\n");
        // token = strtok(NULL, ".\r\n");
        pInTree* p = searchPatient(tree->root, token + 2);
        line = PushToEndOfLine(line, &p->tpatient);
    }
    fclose(file);
    return line;
}


// Pushes a new patient to the end of a linked list representing a line.
pLine* PushToEndOfLine(pLine* root, Patient* p)
{
    pInLine* newNode = (pInLine*)malloc(sizeof(pInLine));
    if (newNode == NULL)
    {
        displayError("allocation failed\n");
        return root;
    }
    newNode->lpatient = p;
    newNode->next = NULL;
    if (root->head == NULL)
    {
        root->head = newNode;
        return root;
    }
    pInLine* temp = root->head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newNode;
    return root;
}

// Searches for a patient in a linked list representing a line based on their ID.
Patient* searchLine(pLine* line, char* patientID) {
    pInLine* current = line->head;
    while (current != NULL) {
        if (strcmp(current->lpatient->ID, patientID) == 0) {
            return current->lpatient; // Found the matching patient
        }
        current = current->next;
    }
    return NULL; // No matching patient found
}

// Displays the details of each patient in the given line.
void displayPatientsInLine(pInLine* line) {
    char arrivalBuffer[20];
    if (line == NULL)
        return;

    pInLine* currentPatient = line;
    while (currentPatient != NULL) {
        Visit* visit = currentPatient->lpatient->Visits ? currentPatient->lpatient->Visits->visit : NULL;
        if (visit) {
            formatDate(arrivalBuffer, visit->tArrival);
            printf("Patient ID: %s\n", currentPatient->lpatient->ID);
            printf("Name: %s\n", currentPatient->lpatient->Name);
            printf("Arrival: %s\n", arrivalBuffer);
            printf("Doctor: %s\n", visit->Doctor ? visit->Doctor->Name : "");
            printf("Summary: %s\n", visit->vSummary ? visit->vSummary : "None");
        }
        else {
            printf("No visit information available for patient ID: %s\n", currentPatient->lpatient->ID);
        }

        currentPatient = currentPatient->next;
    }
}

void movePatientToFrontOfLine(pLine* line) {
    char patientID[10];
    char buffer[256];
    // Print all the patients in line
    displayPatientsInLine(line->head);

    // Prompt the patient for their ID
    printf("Enter patient ID: ");
    getValidID(patientID);

    // Edge case: If the line is empty
    if (line->head == NULL) {
        printf("No patients in line.\n");
        return;
    }

    // Special case: If the patient is already at the front
    if (strcmp(line->head->lpatient->ID, patientID) == 0) {
        printf("Patient is already at the front of the line.\n");
        return;
    }

    pInLine* current = line->head;
    pInLine* previous = NULL;

    // Traverse the list to find the patient
    while (current != NULL && strcmp(current->lpatient->ID, patientID) != 0) {
        previous = current;
        current = current->next;
    }

    // If patient is found
    if (current != NULL) {
        // Re-link the previous node to skip the current node
        if (previous != NULL) {
            previous->next = current->next;
        }
        // Move the current node to the front of the list
        current->next = line->head;
        line->head = current;
        printf("Patient %s moved to the front of the line.\n", patientID);
    }
    else {
        printf("Patient with ID %s not found in line.\n", patientID);
    }
    printf("\n");
    displayPatientsInLine(line->head);
}

// Initializes a new linked list representing a line.
pLine* initializeLine() {
    pLine* line = (pLine*)malloc(sizeof(pLine));
    if (line == NULL) {
        displayError("Memory allocation failed\n");
        exit(1);
    }
    line->head = NULL;
    return line;
}

// Frees the memory used by a linked list representing a line.
void freeLine(pInLine* node) {
    while (node != NULL) {
        pInLine* temp = node;
        node = node->next;
        free(temp);
    }
}