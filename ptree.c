#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"
#include "common.h"
#include "ptree.h"

/* Inserts a new patient into the tree */
void insertToTree(pTree* tree, Patient p)
{
	pInTree* node = (pInTree*)malloc(sizeof(pInTree));
	if (node == NULL)
	{
		displayError("allocation failed\n");
		return;
	}
	node->tpatient = p;
	node->left = NULL;
	node->right = NULL;
	if (tree->root == NULL)
		tree->root = node;
	else
	{
		pInTree* temp = tree->root;
		pInTree* leaf = NULL;
		while (temp != NULL)
		{
			leaf = temp;
			if (strcmp(p.ID, temp->tpatient.ID) > 0)
				temp = temp->right;
			else
				temp = temp->left;
		}
		if (strcmp(p.ID, leaf->tpatient.ID) > 0)
			leaf->right = node;
		else
			leaf->left = node;
	}
}

/* Initializes a new tree with a NULL root */
pTree* initializeTree() {
	pTree* tree = (pTree*)malloc(sizeof(pTree));
	if (tree == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	tree->root = NULL;
	return tree;
}

/* Frees the memory allocated for a tree */
void freeTree(pInTree* node) {
	if (node != NULL) {
		freeTree(node->left);
		freeTree(node->right);
		free(node);
	}
}
