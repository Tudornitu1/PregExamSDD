#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define HSIZE 1024

typedef struct MutualFund {
	char* mutualFundCode;
	char* mutualFundGroup;
	int riskLevel;
	double netAssetValue;
	float returnOfInvestment;
}MutualFund;

struct DoubleLinkedList {
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
	MutualFund* data;
};

typedef struct DoubleLinkedList Node;

MutualFund* createMutualFund(const char* mutualFundCode, const char* mutualFundGroup, const int riskLevel, const double netAssetValue, const float returnOfInvestment);
void addNode(Node** list, Node* node);
Node* createNode(MutualFund* mutualfund);
void printMutualFund(MutualFund* mutualfund);
void printList(Node* list);
int fundsGreaterThanRisk(Node* list, float risk);
void displayCapitalGainLoss(const Node* list);
MutualFund* getFundAboveThreshold(const Node* list, const double threshold);


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

	// ex 3
	float risk = 1.00f;
	int count = fundsGreaterThanRisk(list, risk);
	printf("\n\nThe number of funds that have a risk level greated than %.2f is: %d", risk, count);

	// ex 4
	printf("\n\nPrinting capital gain/loss:");
	displayCapitalGainLoss(list);

	// ex 5
	double threshold = 1500;
	MutualFund* fund = getFundAboveThreshold(list, threshold);
	printf("\n\nFirst mutual fund above threshold %.2f:", threshold);
	if (fund)
		printMutualFund(fund);
	else
		printf("\nNo fund found.");

	fclose(fp);
	return 0;

}

MutualFund* createMutualFund(const char* mutualFundCode, const char* mutualFundGroup, const int riskLevel, const double netAssetValue, const float returnOfInvestment)
{
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

Node* createNode(MutualFund* mutualfund)
{
	Node* node = (Node*)malloc(sizeof(Node));

	if (node)
	{
		node->next = node->prev = NULL;
		node->data = mutualfund;
	}
	return node;
}

void addNode(Node** list, Node* node)
{
	if (*list == NULL)
		*list = node;
	else {
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printMutualFund(MutualFund* mutualfund)
{
	printf("\nMutual fund: code %s, group %s, risk level %d, net Asset value %.2f, return of Investment %.2f", mutualfund->mutualFundCode, mutualfund->mutualFundGroup, mutualfund->riskLevel, mutualfund->netAssetValue, mutualfund->returnOfInvestment);
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

int fundsGreaterThanRisk(const Node* list,const float risk)
{
	int count = 0;
	while (list != NULL)
	{
		if (list->data->riskLevel > risk)
			count++;
		list = list->next;
	}
	
	return count;
}


void displayCapitalGainLoss(const Node* list)
{
	while (list)
	{
		printf("\nMutual fund code: %s, capital gain/loss: %.2f", list->data->mutualFundCode, list->data->netAssetValue * list->data->returnOfInvestment);

		list = list->next;
	}
}

MutualFund* getFundAboveThreshold(const Node* list, const double threshold)
{
	Node* node = NULL;
	while (list)
	{
		if (list->data->netAssetValue > threshold)
		{
			return list->data;
		}
		list = list->next;
	}
	return NULL;
}