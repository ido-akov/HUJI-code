//
// Created by Tamar Akov on 5/20/20.
//

#include "Structs.h"
#include <string.h>
#include "RBTree.h"
#include <stdlib.h>
#include <math.h>

#ifndef LESS
#define LESS (-1)
#endif

#ifndef EQUAL
#define EQUAL (0)
#endif

#ifndef GREATER
#define GREATER (1)
#endif

#ifndef FAILURE
#define FAILURE (0)
#endif

#ifndef SUCCESS
#define SUCCESS (1)
#endif

#define EPSILON 0.001

const char *NEW_LINE = "\n";

double vectorNorm(Vector *pVector);

int vectorDeepCopy(Vector *dest, const Vector *src);

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
	//no need to check NULL cases
	const char *first = (char *) a;
	const char *second = (char *) b;
	int diff = strcmp(first, second);
	return diff < 0 ? LESS : diff > 0 ? GREATER : EQUAL;
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	if (!word || !pConcatenated)
	{
		//pointers are illegal
		return FAILURE;
	}
	//cast
	const char *string = (char *) word;
	char *concat = (char *) pConcatenated;
	//concatenate word to buffer
	strcat(concat, string);
	strcat(concat, NEW_LINE);
	return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
	char *string = (char *) s;
	free(string);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	Vector *first = (Vector *) a;
	Vector *second = (Vector *) b;
	int minimalLength = first->len <= second->len ? first->len : second->len;
	int i = 0;
	while (i < minimalLength)
	{
		double firstIthElement = first->vector[i];
		double secondIthElement = second->vector[i];
		if (fabs(firstIthElement - secondIthElement) < EPSILON)
		{
			//i'th elements are the same
			i++;
			continue;
		}
		else if (firstIthElement < secondIthElement)
		{
			return LESS;
		}
		else if (firstIthElement > secondIthElement)
		{
			return GREATER;
		}
	}
	//all values are the same till length of minimal vector
	if (i == first->len && i < second->len)
		//first was of shorter length, so first is smaller than second
	{
		return LESS;
	}
	else if (i < first->len && i == second->len)
	{
		//second was of shorter length, so first is larger than second
		return GREATER;
	}
	//the vectors are equal
	return EQUAL;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
	Vector *v = (Vector *) pVector;
	free(v->vector);
	free(v);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{

	if (!pVector || !pMaxVector)
	{
		return FAILURE;
	}
	Vector *current = (Vector *) pVector;
	Vector *max = (Vector *) pMaxVector;
	if (!max->vector || vectorNorm(current) > vectorNorm(max))
	{
		return vectorDeepCopy(max, current);
	}
	return SUCCESS;
}

/**
 * compute euclidean norm of a vector
 * @param pVector vector for which to calculate norm
 * @return norm
 */
double vectorNorm(Vector *pVector)
{
	double sumOfSquares = 0.0;
	for (int i = 0; i < pVector->len; i++)
	{
		sumOfSquares += pVector->vector[i] * pVector->vector[i];
	}
	return sqrt(sumOfSquares);
}

/**
 * copy source vector to destination vector
 * @param dest vector to which to copy
 * @param src vector to be copied
 * @return 0 for failure, 1 for success
 */
int vectorDeepCopy(Vector *dest, const Vector *src)
{
	if (!src || !dest || !src->vector)
	{
		//nothing to copy, or problem to copy to destination
		return FAILURE;
	}
	dest->len = src->len;
	//reallocate enough space to copy source vector data
	dest->vector = (double *) realloc(dest->vector, src->len *
													sizeof(double));
	for (int i = 0; i < dest->len; i++)
	{
		//deep copy
		dest->vector[i] = src->vector[i];
	}
	return SUCCESS;
}

Vector *findMaxNormVectorInTree(RBTree *tree)
{
	void *copy = malloc(sizeof(Vector));
	int status = forEachRBTree(tree, &copyIfNormIsLarger, copy);
	return status == SUCCESS ? (Vector *) copy : NULL;
}







