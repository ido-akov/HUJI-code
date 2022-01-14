#include "SpreaderDetectorParams.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//macros
#define LINE_LENGTH 1025 //maximum length of line is 1024, including null char
#define ARGS_NUM 3 //arguments in command line
#define READ "r" //for reading from file stream
#define WRITE "w+" //for writing to file stream
#define SMALLER -1 //for comparator function
#define EQUAL 0 //same
#define GREATER 1 //same
#define POSITIVE 1.0f //describes definite corona virus carrier

//messages to the user
const char *USAGE = "error: ./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n";
const char *INPUT_ERROR = "Error in input files.\n";
const char *OUTPUT_ERROR = "“Error in output file.\n";

//global variable for keeping track of recursive binary search tree traversal
unsigned int ind = 0;

/**
 * Potential Corona carrier
 * @param id of the carrier- 9 digits
 * @param age of the carrier- float between 0-120
 * @param name of the carrier- alphabetic values, space and hyphen
 */
typedef struct Person
{
	unsigned long id;
	float age;
	char name[LINE_LENGTH];
	float infectionRate;
} Person;

/**
 * Node struct for BST data structure. Node data is a pointer to a potential corona carrier
 * @param left child, right child
 * @param data- pointer to Person struct holding corona carrier data
 */
typedef struct Node
{
	struct Node *left, *right;
	Person *data;
} Node;

//
/**
 * Binary search tree- data structure to hold info about corona carriers, insertion of nodes
 * is by ID. Finds and adds a node in average time O(logn), worst time O(n).
 * @param root of the tree
 * @param size of the tree
 */
typedef struct BST
{
	Node *root;
	long unsigned size;
} BST;

//function declarations (see implementations for documentation)
BST *readPersonsFile(FILE *persons, FILE *meetings);

Node *makeNode(const char name[LINE_LENGTH], const unsigned long id, const float age);

void freeTree(BST **tree);

void freeAllNodes(Node *pNode);

void freeNode(Node *pNode);

void insertNodeToTree(BST *pBst, Node *pNode);

int idCompare(const unsigned long id1, const unsigned long id2);

void readMeetingsFile(const BST *pTree, FILE *meetings);

int writeFile(BST *pTree, const char *p, Person **pArray, const unsigned int arrLength);

int stdError(FILE *persons, FILE *meetings);

Node *findNode(const BST *pBst, const unsigned long id);

float crna(const float d, const float time);

void copyTreeToArray(const Node *pNode, Person **pPerson);

BST *initializeNewBST();

/**
 * function returns informative message to the user about an error and aborts the program
 * @param err informative message
 * @return code 1
 */
int error(const char *err)
{
	fprintf(stderr, "%s", err);
	return EXIT_FAILURE;
}

/**
 * comparator function for sorting persons by infectionRate
 * @param a pointer to first Person
 * @param b pointer to second Person
 * @return 0 if equal, -1 if first is greater (for descending order), else 1
 */
int compareByInfectionRate(const void *a, const void *b)
{
	Person *A = (*(Person **) a);
	Person *B = (*(Person **) b);
	return A->infectionRate > B->infectionRate ? SMALLER : A->infectionRate <
														   B->infectionRate ? GREATER : 0;
}

/**
 * main program - process files, save data to a binary search tree, calculate each Person's
 * probable likelihood of infection, sort by likelihood and save analysis to new file
 * @param argc number of arguments to program
 * @param argv arguments
 * @return success or failure
 */
int main(int argc, char *argv[])
{
	if (argc != ARGS_NUM)
	{
		//wrong number of inputs
		return error(USAGE);
	}
	FILE *persons = fopen(argv[1], READ);
	if (persons == NULL)
	{
		//persons file wasn't found
		fclose(persons);
		return error(INPUT_ERROR);
	}
	FILE *meetings = fopen(argv[2], READ);
	if (meetings == NULL)
	{
		//meetings file wasn't found
		fclose(persons);
		fclose(meetings);
		return error(INPUT_ERROR);
	}
	//save data to binary search tree
	BST *tree = readPersonsFile(persons, meetings);
	fclose(persons);
	if (tree == NULL)
	{
		//meetings file must also be empty because of program assumptions
		fclose(meetings);
		//return empty file
		return writeFile(NULL, OUTPUT_FILE, NULL, 0);
	}
	//calculate infection likelihoods
	readMeetingsFile(tree, meetings);
	fclose(meetings);
	Person **pArr = (Person **) malloc(sizeof(Person *) * tree->size);
	if (pArr == NULL)
	{
		//in case of malloc failure free all allocated memory and exit
		freeTree(&tree);
		return stdError(persons, meetings);
	}
	//copy data to array for sorting
	copyTreeToArray(tree->root, pArr);
	//sort by infectionRate
	qsort(pArr, tree->size, sizeof(Person *), compareByInfectionRate);
	//write analysis to new file
	return writeFile(tree, OUTPUT_FILE, pArr, tree->size);
}

/**
 * copy all data from binary search tree to array for sorting by infectionRate
 * @param pNode pointer to a node within the tree
 * @param pPerson pointer to dynamically allocated-array
 */
void copyTreeToArray(const Node *pNode, Person **pPerson)
{
	if (!pNode)
	{//reached NULL leaf
		return;
	}
	//recursively call left and right children
	copyTreeToArray(pNode->left, pPerson);
	pPerson[ind] = pNode->data;
	//keep track of indices
	ind++;
	copyTreeToArray(pNode->right, pPerson);
}

/**
 * write analysis to new file
 * @param pTree pointer to binary search tree
 * @param path of new file
 * @param pArray pointer to sorted array
 * @param arrLength length of array
 * @return success or failure
 */
int writeFile(BST *pTree, const char *path, Person **pArray, const unsigned int arrLength)
{
	FILE *analysis = fopen(path, WRITE);
	if (analysis == NULL)
	{
		//error opening file
		return error(OUTPUT_ERROR);
	}
	if (pTree == NULL)
	{
		//if Persons file was empty other files are already closed
		fclose(analysis);
		return EXIT_SUCCESS;
	}
	else
	{
		//neither tree nor array are empty
		for (unsigned int i = 0; i < arrLength; i++)
		{
			Person *cur = pArray[i];
			float val = cur->infectionRate;
			//check infection factor of current person, print relevant message
			const char *msg = val > MEDICAL_SUPERVISION_THRESHOLD ?
							  MEDICAL_SUPERVISION_THRESHOLD_MSG :
							  val > REGULAR_QUARANTINE_THRESHOLD ? REGULAR_QUARANTINE_MSG
																 : CLEAN_MSG;
			fprintf(analysis, msg, cur->name, cur->id);
		}
	}
	fclose(analysis);
	//free allocated memory- data pointers first
	freeTree(&pTree);
	//finally pointer to dynamically-allocated array
	free(pArray);
	pArray = NULL;
	return EXIT_SUCCESS;
}

/**
 * parse file with data about meetings of carriers and potential carriers
 * @param pTree pointer to binary search tree
 * @param meetings pointer to file with data about meetings
 */
void readMeetingsFile(const BST *pTree, FILE *meetings)
{
	char line[LINE_LENGTH];
	if (fgets(line, LINE_LENGTH, meetings))
	{
		//parse first line
		unsigned long infector;
		sscanf(line, "%lu", &infector);
		Node *carrier = findNode(pTree, infector);
		carrier->data->infectionRate = POSITIVE;
		//set vars to hold data
		unsigned long infected;
		float dist;
		float time;
		while (fgets(line, LINE_LENGTH, meetings))
		{
			sscanf(line, "%lu %lu %f %f", &infector, &infected, &dist, &time);
			if (idCompare(infector, carrier->data->id) != EQUAL)
				//we are examining meetings with a different potential carrier
			{
				carrier = findNode(pTree, infector);
			}
			Node *exposed = findNode(pTree, infected);
			//we know that the product of all previous encounters is equal to the infectionRate
			//of the current potential carrier
			exposed->data->infectionRate = carrier->data->infectionRate * crna(dist, time);
		}
	}
}

/**
 * formula for the calculation of infectionRate of a potential carrier, with respect to time and
 * distance in which they met a previous potential carrier
 * @param dist distance
 * @param time
 * @return probability likelihood of a being a carrier
 */
float crna(const float dist, const float time)
{
	return (time * MIN_DISTANCE) / (dist * MAX_TIME);
}

/**
 * find a node within the BST by id
 * @param pBst pointer to binary search tree
 * @param id to be found within tree
 * @return relevant node
 */
Node *findNode(const BST *pBst, const unsigned long id)
{
	Node *p = pBst->root;
	while (p)
	{
		//search for node with relevant data
		int diff = idCompare(p->data->id, id);
		if (diff == EQUAL)
		{
			//node with data was found
			return p;
		}
		else
		{
			//if diff is smaller than equal check node's right child, otherwise check left child
			p = (diff < EQUAL) ? p->right : p->left;
		}
	}
	//this shouldn't happen because we assume every id in Meetings file exists in
	//Persons file
	return NULL;
}

/**
 * parse Persons data file
 * @param persons pointer to file
 * @param meetings pointer to file with meetings data
 * @return pointer to a binary search tree which holds all data
 */
BST *readPersonsFile(FILE *persons, FILE *meetings)
{
	BST *tree = NULL;
	//parse all data in file line by line
	char line[LINE_LENGTH];
	unsigned long id;
	float age;
	char name[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, persons))
	{
		if (tree == NULL)
		{
			//file isn't empty- create BST to process and store file data
			tree = initializeNewBST();
			if (tree == NULL)
			{
				//memory allocation failed
				exit(stdError(persons, meetings));
			}
		}
		sscanf(line, "%s %lu %f", name, &id, &age);
		Node *cur = makeNode(name, id, age);
		if (cur == NULL)
		{
			//problem creating a new node, free all allocated memory
			freeTree(&tree);
			exit(stdError(persons, meetings));
		}
		insertNodeToTree(tree, cur);
	}
	return tree;
}

/**
 * make a new, empty binary search tree to hold data within
 * @return pointer to the tree
 */
BST *initializeNewBST()
{
	BST *newTree = (BST *) malloc(sizeof(BST));
	if (newTree == NULL)
	{
		//memory allocation failed
		return NULL;
	}
	else
	{
		//initialize new BST, set default fields
		newTree->root = NULL;
		newTree->size = 0;
	}
	return newTree;
}

/**
 * close all files in case of error, return informative message and code 1
 * @param persons file
 * @param meetings file
 * @return failure
 */
int stdError(FILE *persons, FILE *meetings)
{
	fclose(persons);
	fclose(meetings);
	return error(STANDARD_LIB_ERR_MSG);
}

/**
 * put new Node with data describing a potential corona carrier into the binary search tree
 * @param pBst pointer to tree
 * @param pNode pointer to node
 */
void insertNodeToTree(BST *pBst, Node *pNode)
{
	Node *p = pBst->root;
	if (p == NULL)//tree is empty
	{
		pBst->root = pNode;
	}
	else
	{
		while (p)
			//traverse tree to find insertion spot.
		{
			int diff = idCompare(p->data->id, pNode->data->id);
			if (diff == GREATER)
			{
				//if current tree node is larger than new node
				if (p->left == NULL)
				{
					//new node is the new predecessor to p
					p->left = pNode;
					break;
				}
				//compare left child to new node
				p = p->left;
			}
			else
			{
				//new node is larger
				if (p->right == NULL)
				{
					//new node is the new successor to p
					p->right = pNode;
					break;
				}
				//compare right child to new node
				p = p->right;
			}
		}
	}
	pBst->size++;
}

/**
 * comparator within binary search tree. Allows us to find a node by ID in O(logn) on average
 * @param id1 first id
 * @param id2 second id
 * @return -1 if first is smaller, 1 if first is larger, otherwise 0
 */
int idCompare(const unsigned long id1, const unsigned long id2)
{
	return id1 < id2 ? SMALLER : id1 > id2 ? GREATER : EQUAL;
}

/**
 * free all allocated memory within binary search tree
 * @param tree pointer to pointer to the BST
 */
void freeTree(BST **tree)
{
	freeAllNodes((*tree)->root);
	free(*tree);
	*tree = NULL;
}

/**
 * free all memory allocated within nodes for data
 * @param pNode pointer to a node
 */
void freeAllNodes(Node *pNode)
{
	if (!pNode)
	{
		return;
	}
	//recursively free left and right children
	freeAllNodes(pNode->left);
	freeAllNodes(pNode->right);
	freeNode(pNode);
}

/**
 * free data within a node
 * @param pNode pointer to node
 */
void freeNode(Node *pNode)
{
	free(pNode->data);
	free(pNode);
}

/**
 * make a new Node holding data about a corona carrier to add to the BST
 * @param name of carrier
 * @param id of carrier
 * @param age of carrier
 * @return pointer to new Node if successful
 */
Node *makeNode(const char name[LINE_LENGTH], const unsigned long id, const float age)
{
	Node *node = (Node *) malloc(sizeof(Node));
	if (!node)
	{
		return NULL;
	}
	//initialize node to default values
	node->right = node->left = NULL;
	node->data = (Person *) malloc(sizeof(Person));
	if (node->data == NULL)
	{
		//if memory allocation failed, free previous allocated memory
		free(node);
		return NULL;
	}
	node->data->age = age;
	node->data->id = id;
	strcpy(node->data->name, name);
	node->data->infectionRate = 0.0f;
	return node;
}




