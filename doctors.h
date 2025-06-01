#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"

DocNode* loadDoctors();
DocNode* ParseDoctor(char* str);
DocNode* SearchDoc(char* str, DocNode* head);
DocNode* PushToEndOfNode(DocNode* head, DocNode* newNode);
void displayAllDocs(DocNode* head);
void assignDoctor2case(DocNode* head, Visit* newVisit);
void removeNodeByDocPtr(DocNode** head, Doc* doc);
void addDocNode(DocNode** head, char* name, char* nLicense, int nPatients);
DocNode* initializeNode(char* name, char* nLicense, int nPatients);
void freeDocList(DocNode* node);
void displayAllDocs(DocNode* head);
DocNode* getMostAvailableDoc(DocNode* head);
