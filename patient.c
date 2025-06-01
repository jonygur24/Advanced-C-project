#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptree.h"
#include "common.h"
#include "doctors.h"
#include "patient.h"
#include "pline.h"

pTree* loadPatients(DocNode* head) {
    char buffer[256];
    pTree* tree = (pTree*)malloc(sizeof(pTree));
    if (tree == NULL) {
        displayError("Allocation failed\n");
        return NULL;
    }

    FILE* file = fopen("Patients.txt", "r");
    if (file == NULL) {
        displayError("Error opening file");
        free(tree);
        return NULL;
    }

    tree->root = NULL;
    // Skip first two lines
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file)) {
        if (buffer[0] != '=' && is_ascii(buffer[0])) {
            Patient p = { 0 };  // Initialize Patient structure
            parsePatientInfo(buffer, &p);

            while (fgets(buffer, sizeof(buffer), file) == NULL || buffer[0] != '=') {
                // Check for the end of the patient section
                processVisit(tree, file, &p, head);
            }
            // update the visits here
            insertToTree(tree, p);
        }
    }

    fclose(file);
    return tree;
}

pInTree* addPatientNode(pInTree* node, Patient* patient) {
    if (node == NULL) {
        pInTree* newNode = (pInTree*)malloc(sizeof(pInTree));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        newNode->tpatient = *patient;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (strcmp(patient->ID, node->tpatient.ID) < 0) {
        node->left = addPatientNode(node->left, patient);
    }
    else if (strcmp(patient->ID, node->tpatient.ID) > 0) {
        node->right = addPatientNode(node->right, patient);
    }
    return node;
}


void processVisit(pTree* tree, FILE* file, Patient* p, DocNode* head) {
    char buffer[256];
    Visit* v = (Visit*)malloc(sizeof(Visit));
    if (v == NULL) {
        displayError("Allocation failed\n");
        free(p->Name);
        exit(1);
    }

    v->Duration = 0;  // Default value
    v->vSummary = NULL;

    // fgets(buffer, sizeof(buffer), file);  // Read empty line

    fgets(buffer, sizeof(buffer), file);  // Read Arrival
    ParseDate(strchr(buffer, ':') + 1, &v->tArrival);

    fgets(buffer, sizeof(buffer), file);  // Read Dismissed
    ParseDate(strchr(buffer, ':') + 1, &v->tDismissed);

    fgets(buffer, sizeof(buffer), file);  // Read Duration
    char* token = strchr(buffer, ':');
    if (token != NULL && is_number(token[1])) {
        v->Duration = ParseDuration(token + 1);
    }

    fgets(buffer, sizeof(buffer), file);  // Read Doctor
    token = strchr(buffer, ':');
    if (token != NULL) {
        remove_non_ascii(token + 1);
        DocNode* D = SearchDoc(token + 1, head);
        v->Doctor = &D->doctor;
    }

    fgets(buffer, sizeof(buffer), file);  // Read Summary
    token = strchr(buffer, ':');
    if (token != NULL) {
        v->vSummary = _strdup(token + 1);
    }

    p->Visits = pushVisit(p->Visits, v);
}

void parsePatientInfo(char* buffer, Patient* p) {
    char* token = strtok(buffer, ";");
    p->Name = _strdup(token + 2);
    token = strtok(NULL, ";");
    strcpy(p->ID, token);
    token = strtok(NULL, "\n");
    p->Allergies = Allergies(token);
    p->Visits = NULL;
    p->nVisits = 0;
}

void handleAllocationFailure(void* ptr, char* message) {
    if (ptr == NULL) {
        displayError(message);
        exit(1);
    }
}

pInTree* searchPatient(pInTree* root, char* id) {
    // Check if root or id is NULL
    if (root == NULL) {
        return NULL;
    }
    if (id == NULL) {
        printf("Error: Provided id is NULL.\n");
        return NULL;
    }
    if (root->tpatient.ID == NULL) {
        printf("Error: Patient ID in tree node is NULL.\n");
        return NULL;
    }

    // Compare the given ID with the current node's ID
    int cmp = strcmp(root->tpatient.ID, id);
    if (cmp == 0) {
        // ID matches
        return root;
    }
    else if (cmp > 0) {
        // ID is less than current node's ID, search left subtree
        return searchPatient(root->left, id);
    }
    else {
        // ID is greater than current node's ID, search right subtree
        return searchPatient(root->right, id);
    }
}

void reverseVisitStack(Visit_Stack** head) {
    Visit_Stack* prev = NULL;
    Visit_Stack* current = *head;
    Visit_Stack* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

Visit_Stack* pushVisit(Visit_Stack* visits, Visit* v)
{
    Visit_Stack* vs = (Visit_Stack*)malloc(sizeof(Visit_Stack));
    if (vs == NULL)
    {
        displayError("allocation failed\n");
        return visits;
    }
    vs->visit = v;
    vs->next = visits;
    return vs;
}

Visit_Stack* popVisit(Visit_Stack* visits, Visit** v) {
    if (visits == NULL) {
        *v = NULL;
        return NULL;
    }

    Visit_Stack* temp = visits;
    *v = visits->visit;
    visits = visits->next;
    free(temp);

    return visits;
}

// displaying all patients 
void displayAllPatients(pInTree* root)
{
    if (root == NULL)
        return;
    displayAllPatients(root->left);
    printf("ID: %s, Name: %s \n", root->tpatient.ID, root->tpatient.Name);
    printf("\n");
    displayAllPatients(root->right);
}

// arguments - visit stack
// sort visit stack of a patient
// use recursion
// use the most efficient sorting
// return sorted visit stack
Visit_Stack* sortVisitStack(Visit_Stack* visits) {
    Visit_Stack* reversed = NULL;
    while (visits != NULL) {
        Visit* visit;
        visits = popVisit(visits, &visit);
        reversed = pushVisit(reversed, visit);
    }
    return reversed;
}


void addPatient(pTree* tree, Patient* patient) {
    tree->root = addPatientNode(tree->root, patient);
}

void removePatientFromTree(pTree* tree, char* ID) {
    tree->root = removePatientNode(tree->root, ID);
}

pInTree* removePatientNode(pInTree* root, char* ID) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(ID, root->tpatient.ID) < 0) {
        root->left = removePatientNode(root->left, ID);
    }
    else if (strcmp(ID, root->tpatient.ID) > 0) {
        root->right = removePatientNode(root->right, ID);
    }
    else {
        if (root->left == NULL) {
            pInTree* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            pInTree* temp = root->left;
            free(root);
            return temp;
        }

        pInTree* temp = findMin(root->right);
        root->tpatient = temp->tpatient;
        root->right = removePatientNode(root->right, temp->tpatient.ID);
    }
    return root;
}

// Display all patient's admissions, meaning all it's visits
// get from the user the ID of the patient, then display all the visits
// of that patient in order from the latest to the earliest.
// arguments - head of patients tree
void displayAllPatientsAdmissions(pInTree* root) {
    char id[10];
    char buffer[256];
    printf("Enter the ID of the patient: ");
    getValidID(id);

    pInTree* node = searchPatient(root, id);
    if (node == NULL) {
        printf("Patient not found.\n");
        return;
    }

    Visit_Stack* stack = node->tpatient.Visits;

    int visitCount = 0;
    while (stack != NULL) {
        Visit* visit = stack->visit;

        char arrivalBuffer[20];
        char dismissedBuffer[20] = "";
        char durationBuffer[10] = "";

        formatDate(arrivalBuffer, visit->tArrival);
        if (isValidDate(&visit->tDismissed)) {
            formatDate(dismissedBuffer, visit->tDismissed);
            formatDuration(durationBuffer, visit->Duration);
        }
        else {
            // Leave duration empty if no dismissal date
            durationBuffer[0] = '\0';
        }

        if (visitCount > 0) {
            printf("\n");
        }
        printf("Arrival: %s\n", arrivalBuffer);
        printf("Dismissed: %s\n", dismissedBuffer);
        printf("Duration: %s\n", durationBuffer);
        printf("Doctor: %s\n", visit->Doctor ? visit->Doctor->Name : "");
        printf("Summary: %s\n", visit->vSummary ? visit->vSummary : "None");

        stack = stack->next;
        visitCount++;
    }
}


// return empty object when the patient has no visits
Visit* getLastVisit(Patient* patient)
{
    if (patient == NULL)
        return NULL;

    Visit_Stack* stack = patient->Visits;
    Visit* lastVisit = stack->visit;
    Visit* currentVisit = NULL;

    while (stack != NULL)
    {
        currentVisit = stack->visit;
        if (CompareDates(&currentVisit->tArrival, &lastVisit->tArrival) > 0)
            lastVisit = currentVisit;
        stack = stack->next;
    }

    return lastVisit;
}

void displayLinePatientsLastVisitDetails(pInLine* line)
{
    if (line == NULL)
        return;

    pInLine* currentPatient = line;
    while (currentPatient != NULL)
    {
        printf("Patient ID: %s\n", currentPatient->lpatient->ID);
        printf("Name: %s\n\n", currentPatient->lpatient->Name);
        printf("Last Visit Details:\n");
        Visit_Stack* stack = currentPatient->lpatient->Visits;

        if (stack != NULL)
        {
            Visit* lastVisit = getLastVisit(currentPatient->lpatient);
            printf("\tDate of arrival: %d/%d/%d\n", lastVisit->tArrival.Day, lastVisit->tArrival.Month, lastVisit->tArrival.Year);
            printf("\tDate of dismissed: %d/%d/%d\n", lastVisit->tDismissed.Day, lastVisit->tDismissed.Month, lastVisit->tDismissed.Year);
            printf("\tDuration: %f\n", lastVisit->Duration);
            printf("\tDoctor: %s\n", lastVisit->Doctor->Name);
            printf("\tSummary: %s\n", lastVisit->vSummary == NULL ? "None" : lastVisit->vSummary);
            printf("\n");
        }

        currentPatient = currentPatient->next;
    }

}

// show all the patients in line (name, ID) - name and ID can be found through tree of patients
// get from the user the ID of the patient and show the list of allergies of that patient
// allergies can be found through tree of patients
// display allergies for a patient by ID
void checkForAllergies(pInLine* line)
{
    char buffer[256];
    if (line == NULL)
        return;

    pInLine* node = line;

    printf("Patients in line:\n");
    while (node != NULL)
    {
        printf("ID: %s, Name: %s\n", node->lpatient->ID, node->lpatient->Name);
        node = node->next;
    }


    char id[10];
    printf("Enter the ID of the patient to display allergies: ");
    getValidID(id);

    node = line;

    while (node != NULL)
    {
        if (strcmp(node->lpatient->ID, id) == 0)
        {
            printf("Allergies of patient with ID %s:\n", id);
            printf("%s\n", AllergiesToString(node->lpatient->Allergies));
            return;
        }
        node = node->next;
    }
}


// Removes a visit from the patient's visit list based on the arrival date provided by the user
void removeVisit(pInTree* root)
{
    char id[10];
    char buffer[256];
    printf("Enter the ID of the patient: ");
    getValidID(id);

    pInTree* patient = searchPatient(root, id);
    if (patient == NULL)
    {
        printf("Patient with ID %s does not exist\n", id);
        return;
    }

    Date visitDate;
    printf("Enter the arrival date of the visit to remove (dd/mm/yyyy hh:mm): ");
    fgets(buffer, sizeof(buffer), stdin);
    int res = sscanf(buffer, "%d/%d/%d %d:%d", &visitDate.Day, &visitDate.Month, &visitDate.Year, &visitDate.Hour, &visitDate.Min);
    if (res != 5)
    {
        printf("Invalid input format. Date must be in dd/mm/yyyy hh:mm format.\n");
        return;
    }

    Visit_Stack* current = patient->tpatient.Visits;
    Visit_Stack* previous = NULL;

    while (current != NULL)
    {
        if (CompareDates(&current->visit->tArrival, &visitDate) == 0)
        {
            printf("Date: %d/%d/%d\n", current->visit->tArrival.Day, current->visit->tArrival.Month, current->visit->tArrival.Year);
            if (previous == NULL)
            {
                patient->tpatient.Visits = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            free(current->visit);
            free(current);
            printf("Visit on %d/%d/%d removed\n", visitDate.Day, visitDate.Month, visitDate.Year);
            printf("Visits of patient with ID %s:\n", id);
            printPatientVisits(patient);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Visit on %d/%d/%d not found\n", visitDate.Day, visitDate.Month, visitDate.Year);
}

// Prints all the visits of a patient with the patient struct
void printPatientVisits(pInTree* inpatient)
{
    pInTree* patient = inpatient;
    if (patient == NULL)
    {
        printf("Patient does not exist\n");
        return;
    }

    Visit_Stack* current = patient->tpatient.Visits;

    while (current != NULL)
    {
        printf("\tDate of arrival: %d/%d/%d %d:%d\n", current->visit->tArrival.Day, current->visit->tArrival.Month, current->visit->tArrival.Year, current->visit->tArrival.Hour, current->visit->tArrival.Min);
        printf("\tDate of dismissed: %d/%d/%d %d:%d\n", current->visit->tDismissed.Day, current->visit->tDismissed.Month, current->visit->tDismissed.Year, current->visit->tDismissed.Hour, current->visit->tDismissed.Min);
        printf("\tDuration: %f\n", current->visit->Duration);
        printf("\tDoctor: %s\n", current->visit->Doctor->Name);
        printf("\tSummary: %s\n", current->visit->vSummary == NULL ? "None" : current->visit->vSummary);
        printf("\n");
        current = current->next;
    }
}



/**
 * Adds a new visit to the stack of visits of a patient.
 */
void addVisit(Patient* patient, Date arrival, DocNode* doctor, char* summary) { // Function to create a new visit and push it onto the stack

    Visit* newVisit = (Visit*)malloc(sizeof(Visit));
    if (!newVisit) {
        fprintf(stderr, "Memory allocation failed for new visit\n");
        return;
    }

    newVisit->tArrival = arrival;
    newVisit->vSummary = _strdup(summary);
    assignDoctor2case(doctor, newVisit);

    Visit_Stack* newVisitNode = (Visit_Stack*)malloc(sizeof(Visit_Stack));
    if (!newVisitNode) {
        fprintf(stderr, "Memory allocation failed for new visit node\n");
        free(newVisit->Doctor);
        free(newVisit->vSummary);
        free(newVisit);
        return;
    }

    // Fill in the Visit_Stack node details
    newVisitNode->visit = newVisit;
    newVisitNode->next = patient->Visits;

    // Update the patient's visit stack
    patient->Visits = newVisitNode;
    patient->nVisits++;
}

// prompt for Arrival time, dismissal time using getValidDate
// calculate the duration if both dsimissal and arrival present, otherwise zero
// 
Visit* promptVisit(Patient* p, DocNode* head)
{
    Date arrivalTime;
    char buffer[256];
    while (getValidDate("Enter arrival time (DD/MM/YYYY HH:MM): ", &arrivalTime) != 0) {}

    safePromptForString("Enter a summary of the visit: ", buffer);
    addVisit(p, arrivalTime, head, buffer);
}

// Prints a menu for entering allergy codes separated by commas.
void printAllergyMenu()
{
    printf("Enter your allergy codes separated by commas:\n"
        "0: None\n"
        "1: Penicillin\n"
        "2: Sulfa\n"
        "3: Opioids\n"
        "4: Anesthetics\n"
        "5: Eggs\n"
        "6: Latex\n"
        "7: Preservatives\n"
        "Example input: 1,2,3\n");
}

char* getAllergiesInput()
{
    bool isValid = false;
    char allergiesBuff[255];
    char allergiesBuffCopy[255];

    printAllergyMenu();

    if (allergiesBuff == NULL || allergiesBuffCopy == NULL)
    {
        displayError("Allocation failed\n");
        return NULL;
    }

    while (!isValid) {
        if (fgets(allergiesBuff, sizeof(allergiesBuff), stdin) == NULL) {
            displayError("\nInput error\n");
            continue;
        }

        // Remove newline character at the end if present
        int len = strlen(allergiesBuff);
        if (len > 0 && allergiesBuff[len - 1] == '\n') {
            allergiesBuff[len - 1] = '\0';
        }

        // Make a copy of the input buffer
        strcpy(allergiesBuffCopy, allergiesBuff);

        if ((strstr(allergiesBuff, "0,") != NULL || strstr(allergiesBuff, "0,") != NULL)
            && strlen(allergiesBuff) != 1) {
            displayError("'0' (None) cannot be combined with other allergy codes. Please try again.\n");
            continue;
        }

        if (validateAllergies(allergiesBuffCopy))
        {
            isValid = true;
        }

        if (!isValid) {
            displayError("Invalid input. Please enter valid allergy codes.\n");
            continue;
        }
    }

    return _strdup(allergiesBuff);
}

// Validates a string of allergy codes, returning true if all codes are valid (0-7) and false otherwise.
bool validateAllergies(char* allergiesBuff)
{
    char* token = strtok(allergiesBuff, ",");
    int isValid = 1;

    while (token != NULL) {
        int code = atoi(token);
        if (code < 0 || code > 7) {
            isValid = 0;
            break;
        }
        token = strtok(NULL, ",");
    }

    return isValid;
}

bool hasOpenVisit(Patient* patient)
{
    Visit* lastVisit = getLastVisit(patient);
    if (lastVisit == NULL)
    {
        return false;
    }
    return isValidDate(&lastVisit->tDismissed) == 0;


}

bool patientHasOpenVisit(Patient* patient)
{
    Visit* lastVisit = getLastVisit(patient);
    if (lastVisit == NULL)
    {
        return false;
    }
    return lastVisit->tDismissed.Day == -1 && lastVisit->tDismissed.Month == -1 && lastVisit->tDismissed.Year == -1;
}

// Retrieves the doctor associated with the given patient.
Doc* getPatientDoctor(Patient* patient)
{
    if (patientHasOpenVisit(patient))
    {
        return getLastVisit(patient)->Doctor;
    }
    return NULL;
}

// in every case of failure, free all the allocated memory
// Creates a new patient with the given ID.
Patient* createPatientById(char* id)
{
    Patient* patient = (Patient*)malloc(sizeof(Patient));
    if (patient == NULL)
    {
        displayError("Failed to allocate memory for patient");
        return NULL;
    }

    strncpy(patient->ID, id, 10);
    patient->Visits = NULL;
    patient->nVisits = 0;
    patient->Name = safePromptForStringPtr("Enter patient name: ");    //getValidString(, NULL, 0);
    if (patient->Name == NULL)
    {
        free(patient);
        displayError("Failed to allocate memory for patient name");
        return NULL;
    }
    capitalizeFirstLetter(patient->Name);

    patient->Allergies = Allergies(AllergiesNumberToString(getAllergiesInput()));

    if (patient->Allergies == 0)
    {
        free(patient);
        displayError("Failed to allocate memory for patient allergies");
        return NULL;
    }
    return patient;
}

// Removes a patient from a linked list of patients based on their ID.
void removePatientFromLine(pLine* line, char* ID) {
    pInLine* current = line->head;
    pInLine* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->lpatient->ID, ID) == 0) {
            if (prev == NULL) {
                line->head = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current); // Note: Assuming patient is managed elsewhere
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("No matching patient found\n");
}

// Removes a patient from the tree and line, and decrements the number of patients in the doctor's node.
void removePatient(pTree* tree, pLine* Line, DocNode* Docs_Head) {
    char ID[10];
    char buffer[256];

    getValidID(ID);

    pInTree* p1 = searchPatient(tree->root, ID);
    if (p1 == NULL) {
        printf("Patient not found.\n");
        return;
    }

    Patient* p = &p1->tpatient;

    if (hasOpenVisit(p)) {
        if (p->Visits && p->Visits->visit && p->Visits->visit->Doctor) {
            p->Visits->visit->Doctor->nPatients--;
        }
        removePatientFromLine(Line, ID);
    }

    removePatientFromTree(tree, ID);

    printf("Patient removed successfully.\n");

}

// Displays all patients assigned to a specific doctor.
void displayAllPatientsAssignedToADoctor(pTree* root, DocNode* head) {
    char docName[50];
    DocNode* doc;
    char buffer[256];
    while (1) {
        // Prompt the user for doctor name
        safePromptForString("Enter doctor name:", buffer);
        //fgets(buffer, sizeof(buffer), stdin);
        strncpy(docName, buffer, sizeof(docName));

        // Remove newline character if present
        int len = strlen(docName);
        if (len > 0 && docName[len - 1] == '\n') {
            docName[len - 1] = '\0';
        }

        // Check if input is empty
        if (!is_ascii(docName[0])) {
            printf("No doctor name provided, please try again.\n");
            continue;
        }
        capitalizeFirstLetter(docName);
        // Search for the doctor
        doc = SearchDoc(docName, head);
        if (root == NULL || doc == NULL) {
            printf("Incorecct name, please try again.\n");
            continue;
        }
        printf("\n");
        break;
    }
    // Display patients assigned to the doctor
    displayPatientsAssigned2ADoctor(&doc->doctor, root->root);
}

// Displays all patients assigned to a specific doctor.
void displayPatientsAssigned2ADoctor(Doc* doctor, pInTree* node) {
    if (node == NULL) {
        return;
    }

    // Check if the patient is assigned to the given doctor
    Visit_Stack* visitStack = node->tpatient.Visits;
    if (visitStack != NULL) {
        if (visitStack->visit->Doctor == doctor && isValidDate(&visitStack->visit->tDismissed) == 0) {
            printf("Patient Name: %s\n", node->tpatient.Name);
            printf("Patient ID: %s\n", node->tpatient.ID);
            printf("Arrival Date: %d/%d/%d %d:",
                visitStack->visit->tArrival.Day,
                visitStack->visit->tArrival.Month,
                visitStack->visit->tArrival.Year,
                visitStack->visit->tArrival.Hour);
            if (visitStack->visit->tArrival.Min < 10) {
                printf("0%d\n\n", visitStack->visit->tArrival.Min);
            }
            else
                printf("%d\n\n", visitStack->visit->tArrival.Min);
        }
    }

    // Recursively check left and right subtrees
    displayPatientsAssigned2ADoctor(doctor, node->left);
    displayPatientsAssigned2ADoctor(doctor, node->right);
}

// Discharges the last visit of a patient.
void dischargePatient(DocNode* Docs_Head, pTree* Tree, pLine* Line) {

    float duration = 0;
    char id[10];
    char buffer[256];

    printf("Please enter patient's ID for dismissal:");
    getValidID(id);

    pInTree* p1 = searchPatient(Tree->root, id);
    Patient* p = &p1->tpatient;
    if (p1 == NULL) {
        printf("Patient not found.\n");
        return;
    }
    if (isValidDate(&(p1->tpatient.Visits->visit->tDismissed)) == 1) {
        printf("Patient was dismissed.\n");
        return;
    }

    Date dismissalTime;

    while (duration < 1) { //Loop to check of dismissal time makes sense
        getValidDate("Enter dismissal time (DD/MM/YYYY HH:MM): ", &dismissalTime);
        duration = (dismissalTime.Hour * 60 + dismissalTime.Min) - (p1->tpatient.Visits->visit->tArrival.Hour * 60 + p1->tpatient.Visits->visit->tArrival.Min);

        if (duration < 0 || CompareDates(&dismissalTime, &p1->tpatient.Visits->visit->tArrival)!=1) {
            printf("Error: Dismissal time cannot be earlier than arrival time.\n");
            duration = 0;
        }
    }
    p1->tpatient.Visits->visit->tDismissed = dismissalTime;
    p1->tpatient.Visits->visit->Duration = duration / 60;
    p1->tpatient.Visits->visit->Doctor->nPatients--;
    removePatientFromLine(Line, id);
    printf("ID: %s , Patient %s has been discharged.\n", p1->tpatient.ID, p1->tpatient.Name);
}

// Discharges the last visit of a patient.
void dischargeLastVisit(Visit_Stack* visitStack, Date dismissalDate) {
    if (visitStack == NULL) {
        return;
    }

    Visit_Stack* lastVisit = visitStack;
    while (lastVisit->next != NULL) {
        lastVisit = lastVisit->next;
    }
    lastVisit->visit->tDismissed = dismissalDate;
}

pLine* admitPatient(pTree* root, DocNode* head, pLine* line)
{
    char id[10];
    char buffer[256];
    Patient* p;
    pInTree* p1;
    DocNode* current = getMostAvailableDoc(head);

    if (current->doctor.nPatients != 0 || current == NULL) {
        printf("No available doctors\n");
        return line;
    }

    printf("Enter patient ID:");
    getValidID(id);

    // check with atoi that id is only numbers

    p1 = searchPatient(root->root, id);

    if (p1 == NULL)
    {
        p = createPatientById(id);
        insertToTree(root, *p);
    }
    else
    {
        p = &p1->tpatient;
        if (hasOpenVisit(p))
        {
            displayError("Patient has an open visit, cannot admit patient.\n");
            return NULL;
        }
    }

    promptVisit(p, head);

    if (searchLine(line, id) != NULL) {
        displayError("Patient already in line\n");
        return line;
    }

    PushToEndOfLine(line, p);

    printf("Patient added successfully\n");

    return line;
}