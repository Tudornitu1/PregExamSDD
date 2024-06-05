#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ASIZE 1024
#define HSIZE 26

typedef struct Task{
	unsigned int taskId;
	char* taskDate;
	char* ownerName;
	float taskCompletion;
} Task;

struct SimpleLinkedList {
	struct SimpleLinkedList* next;
	Task* data;
};

typedef struct SimpleLinkedList Node;

typedef struct HashTable {
	Node** buckets;
	int size;
} HT;


Task* createTask(unsigned int, const char*, const char*, float);
void printTask(Task*);
void freeTask(Task**);
void addHT(HT**, Task*);
int fhash(const char, int);
Node* createNode(Task*);
void addNode(Node**, Task*);
void printList(Node*);
void printHT(HT*);
void freeList(Node**);
void freeHT(HT**);
int findTotalTasks(HT*, const char*);
int findTasksPercentage(HT*, float);
void changeNameHT(HT*, int, const char*);
void changeName(char**, const char*);
Node* getListByDate(HT*, const char*);

int main()
{

	FILE* fp = fopen("tasks.txt", "r");
	if (!fp) return -1;

	HT* ht = NULL;

	char buffer[ASIZE];
	char* tok;
	char del[] = ",\n";

	unsigned int taskId;
	char taskDate[ASIZE];
	char ownerName[ASIZE];
	float taskCompletion;

	while (fgets(buffer, ASIZE, fp))
	{
		tok = strtok(buffer, del);
		taskId = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(taskDate, tok);
		memmove(taskDate, taskDate + 1, strlen(taskDate) + 1);

		tok = strtok(NULL, del);
		strcpy(ownerName, tok);
		memmove(ownerName, ownerName + 1, strlen(ownerName) + 1);

		tok = strtok(NULL, del);
		taskCompletion = (float)atof(tok);

		Task* task = createTask(taskId, taskDate, ownerName, taskCompletion);
		addHT(&ht, task);
	}

	// ex 2
	printHT(ht);

	freeHT(&ht);
	fclose(fp);
	return 0;
}

Task* createTask(unsigned int taskId, const char* taskDate, const char* ownerName, float taskCompletion)
{
	Task* t = (Task*)malloc(sizeof(Task));

	if (t)
	{
		t->taskId = taskId;
		t->taskCompletion = taskCompletion;
		t->taskDate = (char*)malloc(strlen(taskDate) + 1);
		if (t->taskDate)
			strcpy(t->taskDate, taskDate);
		t->ownerName = (char*)malloc(strlen(ownerName) + 1);
		if (t->ownerName)
			strcpy(t->ownerName, ownerName);
	}

	return t;
}


void addNode(Node** list, Task* t)
{
	Node* n = createNode(t);

	//if list null
	if (*list == NULL)
		*list = n;

	// add to beginning of the list, O(1)
	else
	{
		n->next = *list;
		*list = n;
	}
}
Node* createNode(Task* t)
{
	Node* n = (Node*)malloc(sizeof(Node));
	if (n)
	{
		n->data = t;
		n->next = NULL;
	}
	return n;
}

int fhash(const char key, int size)
{
	// should be first letter of the word (A -> Z) % size (which is 26, number of letters)
	return key % size;
}

void addHT(HT** ht, Task* t)
{
	// initialize hash table on first run
	if (*ht == NULL)
	{
		*ht = (HT*)malloc(sizeof(HT));
		if (*ht)
		{
			(*ht)->size = HSIZE;
			(*ht)->buckets = (Node**)malloc((*ht)->size * sizeof(Node*));
			if ((*ht)->buckets)
				memset((*ht)->buckets, 0, (*ht)->size * sizeof(Node*));
		}
	}
	// check null
	if (*ht)
	{
		// get the hash
		int index = fhash(t->ownerName[0], (*ht)->size);

		// add the element
		if ((*ht)->buckets)
			addNode(&(*ht)->buckets[index], t);
	}
}
void printHT(HT* ht)
{
	for (int i = 0; i < ht->size; i++)
	{
		printf("\nBucket %d:", i);
		printList(ht->buckets[i]);
	}
}

void printList(Node* list)
{
	while (list)
	{
		printTask(list->data);
		list = list->next;
	}
}

void printTask(Task* t)
{
	printf("\nTask id: %d, taskDate: %s, ownerName: %s, taskCompletion: %.1f",
		t->taskId, t->taskDate, t->ownerName, t->taskCompletion * 100);
}

void freeTask(Task** t)
{
	// free the char fields and set to NULL
	free((*t)->taskDate);
	free((*t)->ownerName);
	*t = NULL;
}

void freeList(Node** list)
{
	// traverse entire list and free Tasks + the node itself
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeTask(&freeMe->data);
		free(freeMe);
	}
	*list = NULL;
}


void freeHT(HT** ht) {
	if (*ht) {
		for (int i = 0; i < (*ht)->size; i++) {
			freeList(&((*ht)->buckets[i]));
		}
		free((*ht)->buckets);
		free(*ht);
		*ht = NULL;
	}
}
