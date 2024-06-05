#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ASIZE 1024

struct Order {
	int id;
	int deliveryTime;
	int clientId;
	char* clientName;
	float price;
};
typedef struct Order Order;

struct BinarySearchTree {
	Order* data;
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
};

typedef struct BinarySearchTree BST;


void printOrder(Order*);
void freeOrder(Order**);
Order* createOrder(int, int, int, const char*, float);
BST* createNode(Order*);
void freeNode(BST**);
void addBST(BST**, BST*);
void inorder(BST*);
void getNoNodes(BST*, int*);
void getOrdersMinTime(BST*, Order**, int, int*);
int MoreTime(BST* bst, int time);

void freeBST(BST**);

void freeArrayOrders(Order***, int);



int main()
{
	FILE* fp = fopen("orders.txt", "r");
	if (!fp) return -1;

	char buffer[ASIZE];
	char* tok;
	char del[] = ",\n";

	BST* bst = NULL;

	int id;
	int deliveryTime;
	int clientId;
	char clientName[ASIZE];
	float price;

	while (fgets(buffer, ASIZE, fp))
	{
		tok = strtok(buffer, del);
		id = atoi(tok);

		tok = strtok(NULL, del);
		deliveryTime = atoi(tok);

		tok = strtok(NULL, del);
		clientId = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(clientName, tok);
		memmove(clientName, clientName + 1, strlen(clientName) + 1);

		tok = strtok(NULL, del);
		price = (float)atof(tok);

		Order* o = createOrder(id, deliveryTime, clientId, clientName, price);
		BST* node = createNode(o);
		addBST(&bst, node);
	}

	// ex 1
	printf("\n\nPrinting the BST:");
	inorder(bst);

	//ex 2
	int time = 40;
	int idx = MoreTime(bst, time);
	printf("\nThere are %d more.", idx);
}


Order* createOrder(int id, int deliveryTime, int clientId, const char* clientName, float price)
{
	Order* o = (Order*)malloc(sizeof(Order));
	if (o)
	{
		o->id = id;
		o->deliveryTime = deliveryTime;
		o->clientId = clientId;
		o->price = price;
		o->clientName = (char*)malloc(strlen(clientName) + 1);
		if (o->clientName)
			strcpy(o->clientName, clientName);
	}
	return o;
}

BST* createNode(Order* o)
{
	BST* node = (BST*)malloc(sizeof(BST));
	if (node)
	{
		node->data = o;
		node->left = node->right = NULL;
	}
	return node;
}

void addBST(BST** bst, BST* node)
{
	if (*bst == NULL)
		*bst = node;
	else
	{
		if (node->data->id > (*bst)->data->id)
			addBST(&(*bst)->right, node);
		else if (node->data->id < (*bst)->data->id)
			addBST(&(*bst)->left, node);
		else
		{
			freeOrder(&(*bst)->data);
			(*bst)->data = node->data;
			free(node);
		}
	}
}

void freeOrder(Order** o)
{
	free((*o)->clientName);
	free(*o);
	*o = NULL;
}

void inorder(BST* bst)
{
	if (bst)
	{
		inorder(bst->left);
		printOrder(bst->data);
		inorder(bst->right);
	}
}

void printOrder(Order* o)
{
	printf("\nOrder id: %d, deliveryTime: %d, clientId: %d, clientName: %s, price: %.2f",
		o->id, o->deliveryTime, o->clientId, o->clientName, o->price);
}

int MoreTime(BST* bst, int time)
{
	if (bst == NULL)
	{
		return 0; // Base case: If the tree is empty, return 0
	}

	int index = 0;

	// Check if the current node's deliveryTime is greater than the given time
	if (bst->data->deliveryTime > time)
	{
		index = 1; // Count this node
	}

	// Recursively count in left and right subtrees
	int leftCount = MoreTime(bst->left, time);
	int rightCount = MoreTime(bst->right, time);

	// Return the total count
	return index + leftCount + rightCount;
}