#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ASIZE 1024

struct Student
{
	int studentId;
	char* name;
	float grade;
};

typedef struct Student Student;

typedef struct BinarySearchTree {
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
	Student* data;

}BST;

Student* createStudent(int id, char* name, float grade);
BST* createNode(Student* stud);
void addBST(BST** bst, BST* node);
void inorder(BST* bst);
void printStudent(Student* stud);
void freeBST(BST** bst);
void freeNode(BST** node);
void freeStudent(Student** stud);

int main()
{
	FILE* fp = fopen("students.txt", "r");

	BST* bst=NULL;

	char buffer[ASIZE];
	char* tok;
	char del[] = ",\n";

	int studentId;
	char name[ASIZE];
	float grade;

	while (fgets(buffer, ASIZE, fp))
	{
		tok = strtok(buffer, del);
		studentId = atoi(tok);
		tok = strtok(NULL, del);
		strcpy(name, tok);
		tok = strtok(NULL, del);
		grade = (float)atof(tok);
		Student* stud = createStudent(studentId, name, grade);
		BST* node = createNode(stud);
		addBST(&bst, node);

	}
	inorder(bst);

	freeBST(&bst);
	fclose(fp);
}

Student* createStudent(int id, char* name, float grade)
{
	Student* stud = (Student*)malloc(sizeof(Student));
	if (stud)
	{
		stud->studentId = id;
		stud->grade = grade;
		stud->name = (char*)malloc(strlen(name) + 1);
		strcpy(stud->name, name);
	}
	return stud;
}

BST* createNode(Student* stud)
{
	BST* node = (BST*)malloc(sizeof(BST));
	if (node)
	{
		node->data = stud;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

void addBST(BST** bst, BST* node)
{
	if (*bst == NULL)
		*bst = node;
	else 
	{
		if (node->data->studentId > (*bst)->data->studentId)
			addBST(&(*bst)->right, node);
		else if(node->data->studentId < (*bst)->data->studentId)
			addBST(&(*bst)->left, node);
	}
}

void inorder(BST* bst)
{
	if (bst)
	{
		inorder(bst->left);
		printStudent(bst->data);
		inorder(bst->right);
	}
}

void printStudent(Student* stud)
{
	printf("\nStudent ID: %d, Name:%s, Grade: %.1f", stud->studentId, stud->name, stud->grade);
}

void freeStudent(Student** stud)
{	
	if (*stud)
	{
		free((*stud)->name);
		free(*stud);
		*stud = NULL;
	}
}


void freeNode(BST** node)
{
	if (*node)
	{
		freeStudent(&(*node)->data);
		free(*node);
		*node = NULL;
	}
}

void freeBST(BST** bst)
{
	if (*bst)
	{
		freeBST(&(*bst)->left);
		freeBST(&(*bst)->right);
		freeNode(*bst);
		

	}
}