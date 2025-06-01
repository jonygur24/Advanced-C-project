#ifndef __PLINE_H__
#define __PLINE_H__
#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"

pLine* loadLine(pTree* tree);
pLine* PushToEndOfLine(pLine* root, Patient* p);
Patient* searchLine(pLine* line, char* patientID);
void displayPatientsInLine(pInLine* line);
void movePatientToFrontOfLine(pLine* line);
pLine* initializeLine();
void freeLine(pInLine* node);

#endif
