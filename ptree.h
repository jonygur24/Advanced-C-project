#ifndef __PTREE_H__
#define __PTREE_H__
#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"

void insertToTree(pTree* tree, Patient p);
pTree* loadPatients(DocNode* head);
pTree* initializeTree();
void freeTree(pInTree* node);

#endif