#ifndef __STRUCTS_H__
#define __STRUCTS_H__
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONE 0
#define PENICILLIN 1
#define SULFA 2
#define OPIOIDS 4
#define ANESTHETICS 8
#define EGGS 16
#define LATEX 32
#define PRESERVATIVES 64

typedef struct
{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Min;
} Date;

typedef struct
{
	char* Name;
	char nLicense[7];
	int nPatients;
} Doc;

typedef struct DocNode
{
	Doc doctor;
	struct DocNode* next;
} DocNode;

typedef struct
{
	Date tArrival;
	Date tDismissed;
	float Duration;
	Doc* Doctor;
	char* vSummary;
} Visit;

typedef struct Visit_Stack
{
	Visit* visit;
	struct Visit_Stack* next;
} Visit_Stack;

typedef struct
{
	char* Name;
	char ID[10];
	char Allergies;
	Visit_Stack* Visits;
	int nVisits;
} Patient;

typedef struct pInTree
{
	Patient tpatient;
	struct pInTree* right;
	struct pInTree* left;
} pInTree;

typedef struct
{
	pInTree* root;
} pTree;

typedef struct pInLine
{
	Patient* lpatient;
	struct pInLine* next;
} pInLine;

typedef struct
{
	pInLine* head;
} pLine;

#endif
