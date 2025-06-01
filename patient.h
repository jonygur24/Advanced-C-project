#ifndef PATIENT_H
#define PATIENT_H
#define _CRT_SECURE_NO_WARNINGS
#include "common.h"
#include "structs.h"

pTree* loadPatients(DocNode* head);
pInTree* addPatientNode(pInTree* node, Patient* patient);
void addPatient(pTree* tree, Patient* patient);
pInTree* removePatientNode(pInTree* root, char* ID);
void removePatientFromTree(pTree* tree, char* ID);
void reverseVisitStack(Visit_Stack** head);
void processVisit(pTree* tree, FILE* file, Patient* p, DocNode* head);
void parsePatientInfo(char* buffer, Patient* p);
void handleAllocationFailure(void* ptr, char* message);
pInTree* searchPatient(pInTree* root, char* id);
pLine* admitPatient(pTree* root, DocNode* head, pLine* line);

Visit_Stack* pushVisit(Visit_Stack* visits, Visit* v);
Visit_Stack* popVisit(Visit_Stack* visits, Visit** v);
void displayAllPatients(pInTree* root);
Visit_Stack* sortVisitStack(Visit_Stack* visits);
void displayAllPatientsAdmissions(pInTree* root);
void checkForAllergies(pInLine* line);
void displayAllergiesForPatient(pInLine* line);
void displayAllPatientsAssignedToADoctor(pTree* root, DocNode* head);
void displayLinePatientsLastVisitDetails(pInLine* line);
void removeVisit(pInTree* root);
void printPatientVisits(pInTree* inpatient);
bool validateAllergies(char* allergiesBuff);
void addVisit(Patient* patient, Date arrival, DocNode* doctor, char* summary);
Patient* createPatientById(char* id);
char* getAllergiesInput();
bool hasOpenVisit(Patient* patient);
void removePatientFromLine(pLine* line, char* ID);
void removePatient(pTree* tree, pLine* Line, DocNode* Docs_Head);
void dischargeLastVisit(Visit_Stack* visitStack, Date dismissalDate);
void dischargePatient(DocNode* Docs_Head, pTree* Tree, pLine* Line);
void displayPatientsAssigned2ADoctor(Doc* doctor, pInTree* node);
Visit* promptVisit(Patient* p, DocNode* head);
void getValidString(char* prompt, char* buffer, int size);
#endif // PATIENT_H

