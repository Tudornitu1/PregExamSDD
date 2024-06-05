#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define HT_SIZE 20

struct Room {
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;
};
typedef struct Room Room;

struct SimpleLinkedList {
	Room* data;
	struct SimpleLinkedList* next;
};
typedef struct SimpleLinkedList Node;

struct DoubleLinkedList {
	Room* data;
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
};
typedef struct DoubleLinkedList DNode;

struct HashTable {
	Node** buckets;
	int size;
};
typedef struct HashTable HT;

void printRoom(Room*);
Room* createRoom(char*, unsigned short, unsigned short, double);
Node* createNode(Room*);
void printHT(HT);
void putNode(HT*, Node*, int);
void putHT(HT*, Node*);
int fhash(unsigned short, int);
void determineFloor(HT*, unsigned short);
DNode* createDNode(Room*);
DNode* createDLL(HT*, int);
void printDLL(DNode*);
void deleteRoomType(DNode**, const char*);


int main()
{
	FILE* fp = fopen("rooms.txt", "r");
	if (!fp)
	{
		printf("\nFile not found");
		return -1;
	}
	char buffer[256];
	char del[] = ",\n";
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;

	HT ht = { NULL, 0 };

	while (fgets(buffer, 256, fp))
	{
		char* token = strtok(buffer, del);
		roomType = (char*)malloc(strlen(token) + 1);
		strcpy(roomType, token);
		token = strtok(NULL, del);
		floor = atoi(token);
		token = strtok(NULL, del);
		roomNo = atoi(token);
		token = strtok(NULL, del);
		price = atof(token);
		Room* room = createRoom(roomType, floor, roomNo, price);
		Node* node = createNode(room);
		putHT(&ht, node);
	}

	//ex 2
	printHT(ht);

	//ex 3
	unsigned short fl = 10;
	determineFloor(&ht, fl);
}

Room* createRoom(char* type, unsigned short floor, unsigned short roomNo, double price)
{
	Room* room = (Room*)malloc(sizeof(Room));
	room->floor = floor;
	room->roomNo = roomNo;
	room->price = price;
	room->roomType = (char*)malloc(strlen(type) + 1);
	strcpy(room->roomType, type);
	return room;
}


Node* createNode(Room* room)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->next = NULL;
	node->data = room;
	return node;
}

int fhash(unsigned short key, int size)
{
	return key % size;
}

void putHT(HT* ht, Node* node)
{
	if (ht->buckets == NULL)
	{
		ht->size = HT_SIZE;
		ht->buckets = (Node**)malloc(ht->size * sizeof(Node*));
		memset(ht->buckets, NULL, ht->size * sizeof(Node*));
	}
	int index = fhash(node->data->floor, ht->size);
	putNode(ht, node, index);
}

void putNode(HT* ht, Node* node, int index)
{
	if (ht->buckets[index] == NULL)
		ht->buckets[index] = node;
	else
	{
		node->next = ht->buckets[index];
		ht->buckets[index] = node;
	}
}



void printRoom(Room* room)
{
	printf("\nRoom Type: %s, Floor: %d, Number: %d, Price: %.1f$", room->roomType, room->floor, room->roomNo, room->price);
}

void printHT(HT ht)
{
	printf("\nPRINTING HASH TABLE:\n");
	for (int i = 0; i < ht.size; i++)
	{
		printf("\nBucket %d:", i);
		Node* bucket = ht.buckets[i];
		while (bucket)
		{
			printRoom(bucket->data);
			bucket = bucket->next;
		}
	}
	printf("\nPRINTING STOPPED");
}

void determineFloor(HT* ht, unsigned short floor)
{
	int noRooms = 0;
	int index = fhash(floor, ht->size);
	Node* bucket = ht->buckets[index];
	while (bucket)
	{
		noRooms++;
		bucket = bucket->next;
	}
	if (noRooms == 0)
		printf("\nNo rooms found for floor %d", floor);
	else
		printf("\nFound %d rooms for floor %d", noRooms, floor);
}
