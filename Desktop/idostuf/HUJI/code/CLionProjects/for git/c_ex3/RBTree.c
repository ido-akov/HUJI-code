#include "RBTree.h"
#include <stdlib.h>

#define LESS (-1)

#define EQUAL (0)

#define GREATER (1)

#define FAILURE 0

#define SUCCESS 1

Node *allocateNode(void *data);

void traverseTree(Node *pNode, forEachFunc func, void *args, int *pInt);

void freeAllNodes(Node *pNode, FreeFunc func);

void balanceAfterInsert(RBTree *pTree, Node *pNode);

Node *minimum(Node *pNode);

Node *uncle(Node *pNode);

void rotateRight(RBTree *pTree, Node *pNode1);

void rotateLeft(RBTree *pTree, Node *pNode);

void deleteNode(RBTree *pTree, Node *pNode);

void freeNode(Node *pNode, FreeFunc func);

Node *successor(Node *pNode);

void connectParentToChild(const Node *pNode, Node *rightChild);

Node *findSibling(RBTree *tree, Node *child, Node *parent);

void case3c(RBTree *pTree, Node *sibling, Node *parent);


void balanceAfterDelete(RBTree *pTree, Node *pNode, Node *child);

Node *case3b(const RBTree *pTree, Node *sibling, Node *doubleBlack, Node *parent);

Node *cases3dto3e(RBTree *pTree, Node *sibling, Node *doubleBlack, Node *parent);

Node *case3e(RBTree *pTree, Node *sibling, Node *parent);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	if (!compFunc || !freeFunc)
	{
		//null pointers to function
		return NULL;
	}
	RBTree *newTree = (RBTree *) malloc(sizeof(RBTree));
	if (newTree)
	{
		//initialize new RBtree, set default fields
		newTree->root = NULL;
		newTree->compFunc = compFunc;
		newTree->freeFunc = freeFunc;
		newTree->size = 0;
		return newTree;
	}
	return NULL;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
	if (!tree || !data || RBTreeContains(tree, data))
	{
		return FAILURE;
	}
	Node *p = tree->root;
	Node *new = allocateNode(data);
	if (!new)
	{
		//node allocation returned NULL pointer- problem with data
		return FAILURE;
	}
	if (!p)
		//tree is empty
	{
		tree->root = new;
	}
	else
	{
		while (p)
			//traverse tree to find insertion spot.
		{
			new->parent = p;
			int diff = tree->compFunc(p->data, data);
			if (diff > EQUAL)
			{
				//if current tree node is larger than new node
				if (!p->left)
				{
					//new node is the new pREDecessor to p
					p->left = new;
					break;
				}
				//check left child
				p = p->left;
			}
			else
			{
				//new node is larger
				if (!p->right)
				{
					//new node is the new successor to p
					p->right = new;
					break;
				}
				//check right child
				p = p->right;
			}
		}
		new->parent = p;
	}
	tree->size++;
	balanceAfterInsert(tree, new);
	return SUCCESS;
}

/**
 * fix the tree in order to maintain RBTree properties.
 * @param pTree RBTree
 * @param node last node inserted to tree
 */
void balanceAfterInsert(RBTree *pTree, Node *node)
{
	//case 1
	if (node == pTree->root)
	{
		//color BLACK
		node->color = BLACK;
		return;
	}
	//case 2
	Node *parent = node->parent;
	if (parent->color == BLACK)
	{
		//nothing to fix
		return;
	}
	Node *uNode = uncle(node);
	Node *grandParent = parent->parent;
	//case 4
	if (!uNode || uNode->color == BLACK)
	{
		//RED parent and BLACK uncle
		if (parent == grandParent->left && node == parent->right)
		{
			//node is right child of a left child
			rotateLeft(pTree, parent);
			//fix parent and node to correct ones after rotation
			parent = grandParent->left;
			node = parent->left;
		}
		else if (parent == grandParent->right && node == parent->left)
		{
			//node is left child of a right child
			rotateRight(pTree, parent);
			//fix parent and node to correct ones after rotation
			parent = grandParent->right;
			node = parent->right;
		}
		//continue to 4b
		if (parent == grandParent->right && node == parent->right)
		{
			rotateLeft(pTree, grandParent);
		}
		else
		{
			rotateRight(pTree, grandParent);
		}
		parent->color = BLACK;
		grandParent->color = RED;
	}
	//case 3
	else
	{
		parent->color = uNode->color = BLACK;
		grandParent->color = BLACK;
		balanceAfterInsert(pTree, grandParent);
	}
}

/**
 * do a left rotation on a node in the tree,making the node's right child root of the subtree
 * spanned by the node
 * @param pTree tree in which to do rotation
 * @param pNode node to rotate
 */
void rotateLeft(RBTree *pTree, Node *pNode)
{
	Node *rightChild = pNode->right;
	pNode->right = rightChild->left;
	if (rightChild->left)
	{
		//set parent to pNode
		rightChild->left->parent = pNode;
	}
	//make rightChild root of subTree
	rightChild->parent = pNode->parent;

	if (!(pNode->parent))
	{
		//pNode was the root of the tree
		pTree->root = rightChild;
	}
	else
	{
		//check if pNode is a left child or right child and update rightChild's grandparent
		connectParentToChild(pNode, rightChild);
	}
	//switch node connections
	rightChild->left = pNode;
	pNode->parent = rightChild;
}

/**
 * do a right rotation on a node in the tree,making the node's left child root of the subtree
 * spanned by the node
 * @param pTree tree in which to do rotation
 * @param pNode node to rotate
 */
void rotateRight(RBTree *pTree, Node *pNode)
{
	Node *leftChild = pNode->left;
	pNode->left = leftChild->right;
	if (leftChild->right)
	{
		//set parent to pNode
		leftChild->right->parent = pNode;
	}
	//make leftChild root of subTree spanned by pNode
	leftChild->parent = pNode->parent;

	if (!(pNode->parent))
	{
		//pNode was the root of the tree
		pTree->root = leftChild;
	}
	else
	{
		//check if pNode is a left child or right child
		connectParentToChild(pNode, leftChild);
	}
	//switch node connections
	leftChild->right = pNode;
	pNode->parent = leftChild;
}

/**
 * find other node in same level as parent
 * @param pNode node for which to find uncle
 * @return pointer to correct node
 */
Node *uncle(Node *pNode)
{
	if (!pNode)
	{
		return NULL;
	}
	Node *parent = pNode->parent;
	if (!parent)
	{
		//node is tree root
		return NULL;
	}
	Node *grandParent = parent->parent;
	return parent == grandParent->right ? grandParent->left : grandParent->right;
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
	if (!tree || !data || !RBTreeContains(tree, data))
	{
		return FAILURE;
	}
	//find node with relevant data in tree
	Node *p = tree->root;
	while (p)
	{
		int diff = tree->compFunc(p->data, data);
		if (diff == EQUAL)
		{
			//node with data was found
			deleteNode(tree, p);
			break;
		}
		else
		{
			//if diff is smaller than equal check node's right child, otherwise check left child
			p = (diff < EQUAL) ? p->right : p->left;
		}
	}
	tree->size--;
	return SUCCESS;
}

/**
 * connect a parent node to a child node of a given node
 * @param pNode node for which to connect parent to child
 * @param child of node
 */
void connectParentToChild(const Node *pNode, Node *child)
{
	//connect child to parent instead of node
	if (pNode == pNode->parent->left)
	{
		pNode->parent->left = child;
	}
	else
	{
		pNode->parent->right = child;
	}
}

/**
 * find successor of a given node in the tree
 * @param node
 * @return successor node
 */
Node *successor(Node *node)
{
	Node *p;
	if (!node)
	{
		return NULL;
	}
	if (node->right)
	{
		//find minimum in right sub-tree
		p = node->right;
		return minimum(p);
	}
	//no right sub-tree
	p = node->parent;
	while (p)
	{
		//find first ancestor for which node is in left sub-tree
		if (p->left == node)
		{
			//we found successor
			break;
		}
		node = p;
		p = p->parent;
	}
	return p;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order.
 * if one of the activations of the function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if (!func || !tree)
	{
		return FAILURE;
	}
	//check status of function throughout tree traversal
	int flag = SUCCESS;
	traverseTree(tree->root, func, args, &flag);
	//what to return in case of null tree?
	return flag;
}

/**
 * go through all 
 * @param node 
 * @param func 
 * @param args 
 * @param p 
 */
void traverseTree(Node *node, forEachFunc func, void *args, int *p)
{
	if (!node)
	{//reached NULL leaf
		return;
	}
	//recursively call left and right children
	traverseTree(node->left, func, args, p);
	int funcStatus = func(node->data, args);
	if (funcStatus == FAILURE)
		//for each function failed
	{
		//set flag to failure
		*p = FAILURE;
	}
	traverseTree(node->right, *func, args, p);
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	if (!tree || !data)
	{
		return FAILURE;
	}
	Node *p = tree->root;
	while (p)
	{
		//search for node with relevant data
		int diff = tree->compFunc(p->data, data);
		if (diff == EQUAL)
		{
			//node with data was found
			return SUCCESS;
		}
		else
		{
			//if diff is smaller than equal check node's right child, otherwise check left child
			p = (diff < EQUAL) ? p->right : p->left;
		}
	}
	//node with data wasn't found
	return FAILURE;
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
	freeAllNodes((*tree)->root, (*tree)->freeFunc);
	free(*tree);
	*tree = NULL;
}

/**
 * free all nodes in tree recursively
 * @param pNode given node in tree
 * @param func function to free node data
 */
void freeAllNodes(Node *pNode, FreeFunc func)
{
	if (!pNode)
	{
		return;
	}
	//recursively free left and right children
	freeAllNodes(pNode->left, func);
	freeAllNodes(pNode->right, func);
	freeNode(pNode, func);
}

/**
 * free a given node in the tree
 * @param pNode node to free
 * @param func for freeing data
 */
void freeNode(Node *pNode, FreeFunc func)
{
	//free data
	func(pNode->data);
	//free pointer
	free(pNode);
}

/**
 * allocate memory for a new tree node
 * @param data to be stoRED in node
 * @param color of node
 * @return pointer to allocated memory or NULL
 */
Node *allocateNode(void *data)
{
	Node *node = (Node *) malloc(sizeof(Node));
	if (!node)
	{
		return NULL;
	}
	//initialize node to default values
	node->parent = node->right = node->left = NULL;
	node->data = data;
	node->color = RED;
	return node;
}

/**
 * return minimum node of a sub-tree.
 */
Node *minimum(Node *node)
{
	Node *p = node;
	while (p->left)
	{
		p = p->left;
	}
	//left most node of tree
	return p;
}

/**
 * find sibling of a given node in the tree. Return pointer to sibling
 */
Node *findSibling(RBTree *tree, Node *child, Node *parent)
{
	if(!parent)
	{
		parent = tree->root;
	}
	return child == parent->left ? parent->right : parent->left;
}

/** specific case for balancing tree in which sibling is red
 * @param pTree tree to balance
 * @param pNode DB
 * @param doubleBlack 
 * @param sibling 
 */
void case3c(RBTree *pTree, Node *sibling, Node *parent)
{
	//swap sibling and parent colors
	sibling->color = BLACK;
	parent->color = RED;
	if (sibling == parent->right)
	{
		//DB child is left child of parent, rotate in child's direction
		rotateLeft(pTree, parent);
	}
	else
	{
		//DB child is right child of parent, rotate in child's direction
		rotateRight(pTree, parent);
	}
	//DB stays
}


/**
 * delete found node with data from the tree
 * @param tree RBTree
 * @param pNode node to be deleted
 */
void deleteNode(RBTree *tree, Node *pNode)
{
	//pNode isn't null because we found it in the tree
	if (tree->size == 1)
	{
		//pNode is tree-root
		tree->root = NULL;
		freeNode(pNode, tree->freeFunc);
		return;
	}
	if (pNode->left && pNode->right)
	{
		//if node has two children
		//find successor
		Node *nextLargest = successor(pNode);
		//swap data
		void *data = pNode->data;
		pNode->data = nextLargest->data;
		nextLargest->data = data;
		//delete successor- which has only one child
		deleteNode(tree, nextLargest);
	}
	else
	{
		Node *child = (pNode->left) ? pNode->left : pNode->right;
		if (child)
		{
			child->parent = pNode->parent;
		}
		if (!(pNode->parent))
		{
			//if we are deleting the root, make the child the new tree node
			tree->root = child;
		}
		else
		{
			//link the node's parent to its child
			connectParentToChild(pNode, child);
		}
		if (pNode->color == BLACK)
		{
			//if cases 2 or 3 we need to fix tree
			balanceAfterDelete(tree, pNode, child);
		}
		freeNode(pNode, tree->freeFunc);
	}
}

/**
 * balance tree according to coloring rules specified by node
 * @param pTree tree to balance
 * @param pNode DB node
 */
void balanceAfterDelete(RBTree *pTree, Node *pNode, Node *child)
{
	Node *sibling;
	Node *doubleBlack;
	if (child && child->color == RED)
	{
		//case 2
		child->color = BLACK;
		return;
	}
	else
	{
		//case 3: child->color = BLACK
		doubleBlack = child;

		//fix tree recursively
		while (doubleBlack != pTree->root && (!doubleBlack || doubleBlack->color == BLACK))
		{
			if (doubleBlack)
			{
				//this is not first call in recursion, so we can use pNode as a "dummy" to find
				//doubleBlack's sibling.
				pNode = doubleBlack;
			}
			sibling = findSibling(pTree, doubleBlack, pNode->parent);
			if (sibling)
			{
				if (sibling->color == BLACK)
				{
					if ((!(sibling->left) || sibling->left->color == BLACK) &&
						(!(sibling->right) || sibling->right->color == BLACK))
					{
						//3b
						doubleBlack = case3b(pTree, sibling, doubleBlack, pNode->parent);
					}
					else
					{
						doubleBlack = cases3dto3e(pTree, sibling, doubleBlack, pNode->parent);
					}
				}
				else
					//case 3c- sibling->color = RED
				{
					case3c(pTree, sibling, pNode->parent);
				}
			}
			else
			{
				//sibling is NULL leaf, thus colored BLACK
				doubleBlack = case3b(pTree, sibling, doubleBlack, pNode->parent);
			}
		}
	}
}

/**
 * sibling has one red child and one black child. If red child is closer do case 3d then
 * move on to case 3e
 * @param pTree RBTree
 * @param sibling of DB
 * @param doubleBlack DB for tree fix
 * @param parent of DB
 * @return new DB
 */
Node *cases3dto3e(RBTree *pTree, Node *sibling, Node *doubleBlack, Node *parent)
{
	if (sibling == parent->right)
	{
		if (sibling->left && sibling->left->color == RED)
		{
			//case 3d- closer child of S is RED
			sibling->left->color = BLACK;
			sibling->color = RED;
			//rotation to direction farther from DB
			rotateRight(pTree, sibling);
		}
	}
	else
	{
		//sibling is left child
		if (sibling->right && sibling->right->color == RED)
		{
			//case 3d- closer child of S is RED, swap colors of sibling and child
			sibling->right->color = BLACK;
			sibling->color = RED;
			//rotation to direction farther from DB
			rotateLeft(pTree, sibling);
		}
	}
	//we must do case 3e now anyway
	sibling = findSibling(pTree, doubleBlack, parent);
	return case3e(pTree, sibling, parent);
}

/**
 * farther child of sibling is red. Fix the tree accordingly
 * @param pTree RBTree
 * @param sibling of DB
 * @param doubleBlack DB for tree fix
 * @param parent of DB
 * @return new DB
 */
Node *case3e(RBTree *pTree, Node *sibling, Node *parent)
{
	Color tmp = parent->color;
	//farther child of S is red
	parent->color = sibling->color;
	sibling->color = tmp;
	if (sibling == parent->right)
	{
		//rotate to DB's direction
		rotateLeft(pTree, parent);
		//paint farther child of S black
		sibling->right->color = BLACK;
	}
	else
	{
		//sibling is left child
		rotateRight(pTree, parent);
		//paint farther child of S black
		sibling->left->color = BLACK;
	}
	//we're finished fixing the tree
	return pTree->root;
}
/**
 * sibling has two black children.
 * @param pTree RBTree
 * @param sibling of DB
 * @param doubleBlack DB for tree fix
 * @param parent of DB
 * @return new DB
 */
Node *case3b(const RBTree *pTree, Node *sibling, Node *doubleBlack, Node *parent)
{
	if (sibling)
	{
		sibling->color = RED;
	}
	if (parent->color == RED)
	{
		//case 3b1
		//we can color parent BLACK because of DB and terminate process
		parent->color = BLACK;
		//process is finished
		doubleBlack = pTree->root;
	}
	else
	{
		//case 3b2- we need to fix parent
		doubleBlack = parent;
	}
	return doubleBlack;
}