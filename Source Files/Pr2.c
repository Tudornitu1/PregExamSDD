#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ASIZE 256
#define HSIZE 26

struct Task {
	unsigned int taskId;
	char* taskDate;
	char* ownerName;
	float taskCompletion;
};
typedef struct Task Task;

struct SimpleLinkedList {
	Task* data;
	struct SimpleLinkedList* next;
};
typedef struct SimpleLinkedList Node;

struct HashTable {
	Node** buckets;
	int size;
};
typedef struct HashTable HT;

Task* createTask(unsigned int taskId, char* taskDate, char* ownerName, float taskCompletion);
Node* createNode(Task* t);
void addNode(Node** list, Task* task);
void addHT(HT* ht, Task* task);
int fhash(const char key, int size);
void printTask(Task* t);
void printList(Node** list);
void printHT(HT* ht);
void NoOfTasksOwner(HT* ht, char* owner);
void changeNameById(HT* ht, unsigned int id, char* name);
void NoOfTasksCompletion(HT* ht, double lvl);

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

	// ex 3
	char* owner = "Alice";
	NoOfTasksOwner(ht, owner);

	// ex 4
	double lvl = 0.5;
		NoOfTasksCompletion(ht, lvl);

	// ex 5
		char name = "Tudor";
		int id = 2;
		changeNameById(ht, id, name);
		printf("\nChanged Name of the task owner.");
			printHT(ht);
}


Task* createTask(unsigned int taskId, const char* taskDate, const char* ownerName, float taskCompletion)
{
	Task* task = (Task*)malloc(sizeof(Task));
	if (task)
	{
		task->taskId = taskId;
		task->taskCompletion = taskCompletion;
		task->taskDate = (char*)malloc(strlen(taskDate) + 1);
		if(task->taskDate)
		strcpy(task->taskDate, taskDate);
		task->ownerName = (char*)malloc(strlen(ownerName) + 1);
		if(task->ownerName)
		strcpy(task->ownerName, ownerName);
		
	}
	return task;
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

void addNode(Node** list, Task* task)
{
	Node* n = createNode(task);

	if (*list == NULL)
		*list = n;
	else
	{
		n->next = *list;
		*list = n;
	}
}

int fhash(const char key, int size)
{
	// should be first letter of the word (A -> Z) % size (which is 26, number of letters)
	return key % size;
}

void addHT(HT** ht, Task* t)
{

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

void printTask(Task* t)
{
	printf("\nTask id: %d, taskDate: %s, ownerName: %s, taskCompletion: %.1f",
		t->taskId, t->taskDate, t->ownerName, t->taskCompletion * 100);
}

void printList(Node* list)
{
	while (list)
	{
		printTask(list->data);
		list = list->next;
	}
}

void printHT(HT* ht)
{
	printf("\nStarting print");
	for (int i = 0; i < ht->size; i++)
	{
		printf("\nBucket %d: ", i);
		printList(ht->buckets[i]);
		}
	printf("\nEnd");
}

void NoOfTasksCompletion(HT* ht, double lvl)
{
	int count = 0;
	for (int i = 0; i < ht->size; i++)
	{
		Node* bucket = ht->buckets[i];
		while (bucket)
		{
			if (bucket->data->taskCompletion > lvl)
				count++;
			bucket = bucket->next;
		}
	}
	printf("\nThe amount of tasks with the completion level above %.1f is %d", lvl, count);
}

void NoOfTasksOwner(HT* ht, char* owner)
{
	int count = 0;
	for (int i = 0; i < ht->size; i++)
	{
		Node* bucket = ht->buckets[i];
			while (bucket)
			{
				if (strcmp(bucket->data->ownerName, owner) == 0)
					count++;
				bucket = bucket->next;
			}
		}
	printf("\nThe number of tasks for %s is %d", owner, count);
}

void changeNameById(HT* ht, unsigned int id, char* name)
{
	for (int i = 0; i < ht->size; i++)
	{
		Node* bucket = ht->buckets[i];
		while (bucket)
		{
			if (bucket->data->taskId == id)
				strcpy(bucket->data->ownerName, name);	
			bucket = bucket->next;
		}
	}
}