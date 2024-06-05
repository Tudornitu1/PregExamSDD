/*Implement a C application for managing mutual investment funds. For this, you will use a double-linked list that contains pointers to elements of type MutualFund,
structure with the following attributes: mutual fund code (char*), mutual fund group (char*), risk level (int), net asset value (double), return of investment (float – percentage value).*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HSIZE 256

typedef struct MutualFund {
	char* mutualFundCode;
	char* mutualFundGroup;
	int riskLevel;
	double netAssetValue;
	float returnOfInvestment;
}MutualFund;

typedef struct DNode {
	MutualFund* data;
	struct DNode* next;
	struct Dnode* prev;
}Node;

MutualFund* createMutualFund(char* mutualFundCode, char* mutualFundGroup, int riskLevel, double netAssetValue, float returnOfInvestment);
Node* createNode(MutualFund* fund);
void addNode(Node** list, Node* node);
void printList(Node* list);
inline void printMutualFund(const MutualFund* mutualFund);
int fundsGreaterThanRisk(const Node*, const float);
void displayCapitalGainLoss(const Node*);
void findFirstAbove(const Node* list, double t);

int main()
{
	FILE* fp = fopen("mutualfunds.txt", "r");
	if (fp == NULL) return -1;

	Node* list = NULL;

	char buffer[HSIZE];
	char* tok;
	char del[] = ",\n";

	char mutualFundCode[HSIZE];
	char mutualFundGroup[HSIZE];
	int riskLevel;
	double netAssetValue;
	float returnOfInvestment;

	while (fgets(buffer, HSIZE, fp) != NULL)
	{
		tok = strtok(buffer, del);
		strcpy(mutualFundCode, tok);

		tok = strtok(NULL, del);
		strcpy(mutualFundGroup, tok);
		memmove(mutualFundGroup, mutualFundGroup + 1, strlen(mutualFundGroup) + 1);

		tok = strtok(NULL, del);
		riskLevel = atoi(tok);

		tok = strtok(NULL, del);
		netAssetValue = atof(tok);

		tok = strtok(NULL, del);
		returnOfInvestment = (float)atof(tok);

		MutualFund* mutualFund = createMutualFund(mutualFundCode, mutualFundGroup, riskLevel, netAssetValue, returnOfInvestment);
		Node* node = createNode(mutualFund);
		addNode(&list, node);
	}

	// ex 2
	printList(list);

	printf("\n\n");

	// ex 3
	int risk = 1;
	int no = fundsGreaterThanRisk(list, risk);
	printf("\nThe number of funds with risk greater than %d is %d", risk, no);
	printf("\n\n");

	// ex 4
	displayCapitalGainLoss(list);
	printf("\n\n");

	// ex 5
	double threshold = 1500;
	findFirstAbove(list, threshold);
}

MutualFund* createMutualFund(char* mutualFundCode, char* mutualFundGroup, int riskLevel, double netAssetValue, float returnOfInvestment) {
	MutualFund* mutualFund = (MutualFund*)malloc(sizeof(MutualFund));

	if (mutualFund)
	{
		mutualFund->mutualFundCode = (char*)malloc(strlen(mutualFundCode) + 1);
		if (mutualFund->mutualFundCode)
			strcpy(mutualFund->mutualFundCode, mutualFundCode);
		mutualFund->mutualFundGroup = (char*)malloc(strlen(mutualFundGroup) + 1);
		if (mutualFund->mutualFundGroup)
			strcpy(mutualFund->mutualFundGroup, mutualFundGroup);
		mutualFund->riskLevel = riskLevel;
		mutualFund->netAssetValue = netAssetValue;
		mutualFund->returnOfInvestment = returnOfInvestment;
	}

	return mutualFund;
}

Node* createNode(MutualFund* fund)
{
	Node* node = NULL;
	node = (Node*)malloc(sizeof(Node));
	if (node)
	{
		node->data = fund;
		node->next = NULL;
		node->prev = NULL;
	}
	return node;
}

void addNode(Node** list, Node* node)
{
	if (!*list)
	{
		*list = node;
	}
	else
	{
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printList(const Node* list)
{
	printf("\nPrinting list forwards:");
	while (list->next != NULL)
	{
		printMutualFund(list->data);
		list = list->next;
	}
	printMutualFund(list->data);

	printf("\nPrinting list backwards:");
	while (list != NULL)
	{
		printMutualFund(list->data);
		list = list->prev;
	}
}

inline void printMutualFund(const MutualFund* mutualFund)
{
	printf("\nMutual fund: code %s, group: %s, riskLevel: %d, netAssetValue: %.2f, returnOfInvestment: %.2f", mutualFund->mutualFundCode, mutualFund->mutualFundGroup, mutualFund->riskLevel, mutualFund->netAssetValue, mutualFund->returnOfInvestment);
}

int fundsGreaterThanRisk(const Node* list, const float risk)
{
	int count = 0;
	while (list)
	{
		if(list->data->riskLevel>risk)
		{
			count++;
		}
		list = list->next;
	}
	
	return count;
}

void displayCapitalGainLoss(const Node* list)
{
	while (list)
	{
		double p = list->data->returnOfInvestment * list->data->netAssetValue;
		printf("\nThe Fund %s has a capital of %.1f", list->data->mutualFundCode, p);
		list = list->next;
	}
}

void findFirstAbove(const Node* list, double t)
{
	int ok = 0;
	while (list)
	{
		if (list->data->netAssetValue > t)
		{
			ok = 1;
			printf("\nThe first fund in the list with the net asset value above %.1f is the fund %s with the value %.1f.", t, list->data->mutualFundCode, list->data->netAssetValue);
			break;
		}
		list = list->next;
	}
	if (ok == 0)
		printf("None found");
	
}