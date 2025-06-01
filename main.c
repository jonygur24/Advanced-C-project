#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "pline.h"
#include "doctors.h"
#include "patient.h"
#include "ptree.h"

int menu()
{
    int choice;
    char buffer[256];
    printf("1 - Admit patient\n"
        "2 - Check for patient's allergies\n"
        "3 - Display all patients\n"
        "4 - Display all patient's admissions\n"
        "5 - Display all patients in line\n"
        "6 - Advance patient in line\n"
        "7 - Display list of doctors\n"
        "8 - Display all patients assigned to a doctor\n"
        "9 - Discharge patient\n"
        "10 - Remove visit\n"
        "11 - Remove patient\n"
        "12 - Close the hospital\n"
        "0 - Exit Program\n");
    printf("Please Choose: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &choice);


    if (choice < 0 || choice > 12)
    {
        printf("you can insert only 0-12.\n");
        return -1;
    }
    return choice;
}

// Frees all memory used by the binary search tree, the line, and the Doc linked list.
void closeTheHospital(pTree* tree, pLine* line, DocNode* Docs_Head) {
    // Free all memory used by the binary search tree
    freeTree(tree->root);
    free(tree);

    // Free all memory used by the line
    freeLine(line->head);
    free(line);

    // Free all memory used by the Doc linked list
    freeDocList(Docs_Head);

    printf("All resources have been freed. Closing the hospital.\n");

    // Exit the system
    exit(0);
}

void writePatient(FILE* file, pInTree* node, int* num) {
    if (node != NULL) {

        // Write patient basic information
        fprintf(file, "%d.%s;%s;", ++(*num), node->tpatient.Name, node->tpatient.ID);

        // Write patient allergies
        if (node->tpatient.Allergies == NONE) {
            fprintf(file, "none\n");
        }
        else {
            int allergies = node->tpatient.Allergies;
            int first = 1; // To handle comma placement
            if (allergies & 1) {
                fprintf(file, "Penicillin");
                first = 0;
            }
            if (allergies & 2) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Sulfa");
                first = 0;
            }
            if (allergies & 4) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Opioids");
                first = 0;
            }
            if (allergies & 8) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Anesthetics");
                first = 0;
            }
            if (allergies & 16) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Eggs");
                first = 0;
            }
            if (allergies & 32) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Latex");
                first = 0;
            }
            if (allergies & 64) {
                if (!first) fprintf(file, ",");
                fprintf(file, "Preservatives");
                first = 0;
            }
            fprintf(file, "\n");
        }

        fprintf(file, "\n");

        // Reverse the visit stack and write visit details
        Visit_Stack* visitStack = node->tpatient.Visits;
        Visit_Stack* reversedStack = NULL;
        Visit_Stack* temp;

        // Reverse the stack
        while (visitStack != NULL) {
            temp = visitStack;
            visitStack = visitStack->next;
            temp->next = reversedStack;
            reversedStack = temp;
        }

        // Write visits in reversed order
        int visitCount = 0;
        while (reversedStack != NULL) {
            Visit* visit = reversedStack->visit;

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
                fprintf(file, "\n");
            }
            fprintf(file, "Arrival:%s\n", arrivalBuffer);
            fprintf(file, "Dismissed:%s\n", dismissedBuffer);
            fprintf(file, "Duration:%s\n", durationBuffer);
            fprintf(file, "Doctor:%s\n", visit->Doctor ? visit->Doctor->Name : "");
            fprintf(file, "Summary:%s", visit->vSummary ? visit->vSummary : "");

            reversedStack = reversedStack->next;
            visitCount++;
        }

        if (visitCount == 0) {
            fprintf(file, "\n");
        }

        // Print separator line
        fprintf(file, "========================\n");

        writePatient(file, node->right, num);
        writePatient(file, node->left, num);
    }
}

void WritePatients(FILE* file, pInTree* root, int* num)
{
    if (root == NULL)
        return;
    writePatient(file, root, num);
    WritePatients(file, root->left, num);
    WritePatients(file, root->right, num);
}

void updateFiles(pTree* tree, pLine* line, DocNode* head)
{
    int cnt = 0;
    FILE* file = fopen("Patients.txt", "w");
    if (file == NULL)
    {
        displayError("Error opening file");
        return;
    }
    fprintf(file, "Name; ID; Allergies\n");
    fflush(file);
    fprintf(file, "========================\n");
    fflush(file);
    writePatient(file, tree->root, &cnt);
    printf("Patient.txt has been updated successfully\n");
    fclose(file);
    file = fopen("Doctors.txt", "w");
    if (file == NULL)
    {
        displayError("Error opening file");
        return;
    }
    fprintf(file, "Full Name; License Number; Number of Patients\n");
    fprintf(file, "=================================================\n");
    fflush(file);
    DocNode* doc = head;
    while (doc != NULL)
    {
        fprintf(file, "%s; %s; %d\n", doc->doctor.Name, doc->doctor.nLicense, doc->doctor.nPatients);
        doc = doc->next;
    }
    printf("Doctors.txt has been updated successfully\n");
    fclose(file);
    file = fopen("Line.txt", "w");
    if (file == NULL)
    {
        displayError("Error opening file");
        return;
    }
    fprintf(file, "Patients' IDs in line\n");
    fprintf(file, "=====================\n");
    fflush(file);
    int i = 1;
    for (pInLine* curr = line->head; curr; curr = curr->next)
    {
        fprintf(file, "%d.%s\n", i++, curr->lpatient->ID);
        fflush(file);
    }
    fclose(file);
    printf("Line.txt has been updated successfully\n");
    closeTheHospital(tree, line, head); // free all memory used by the 3 databases
}


int main()
{
    DocNode* Docs_Head = loadDoctors();
    pTree* Tree = loadPatients(Docs_Head);
    pLine* Line = loadLine(Tree);
    int choice = 0;

    while (1)
    {
        choice = menu();
        switch (choice) {
        case 0:
            printf("System shuting down\n");
            updateFiles(Tree, Line, Docs_Head);
            return 0;
        case 1:
            Line = admitPatient(Tree, Docs_Head, Line);
            break;
        case 2:
            checkForAllergies(Line->head);
            break;
        case 3:
            displayAllPatients(Tree->root);
            printf("\n");
            break;
        case 4:
            displayAllPatientsAdmissions(Tree->root);
            break;
        case 5:
            displayPatientsInLine(Line->head);
            printf("\n");
            break;
        case 6:
            movePatientToFrontOfLine(Line);
            break;
        case 7:
            displayAllDocs(Docs_Head);
            break;
        case 8:
            displayAllPatientsAssignedToADoctor(Tree, Docs_Head);
            break;
        case 9:
            dischargePatient(Docs_Head, Tree, Line);
            break;
        case 10:
            removeVisit(Tree->root);
            break;
        case 11:
            removePatient(Tree, Line, Docs_Head);
            break;
        case 12:
            closeTheHospital(Tree, Line, Docs_Head);
            break;
        }

    }
    return choice;
}
