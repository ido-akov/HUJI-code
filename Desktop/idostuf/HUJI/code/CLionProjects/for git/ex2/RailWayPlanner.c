#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include <stdlib.h>

//macros

//return code for parsing functions in case of illegal line
#define NO_SOLUTION -1

//initial size for rail part array
#define ALLOC_SIZE 5

//necessary number of args for program
#define ARG_NUM 2

//longest possible line to be parsed in file
#define LINE_LENGTH 1024

//argument to strtol
#define BASE_TEN 10

//flag for error function
#define FILE_OPENED 1

//globals
FILE *filePointer;
int railLength, setSize, partNum;
char line[LINE_LENGTH];

//regex struct for pattern matching
regex_t regexCompiled;

//strings for informative messages, regular expressions for pattern matching
const char *RAIL_PART_REGEX = "^[^\n\r,],[^\n\r,],[1-9][0-9]*,[1-9][0-9]*(\n)*$",
		*USAGE_ERROR = "USAGE_ERROR: RailWayPlanner <InputFile>", *LENGTH_REGEX = "^[0-9]+\n$",
		*SET_SIZE_REGEX = "^[1-9][0-9]*\n$", *SET_REGEX = "^[^\n\r,](,[^\n\r,])*\n$",
		*FILE_ERROR = "File doesn't exists.", *EMPTY_ERROR = "File is empty.",
		*INPUT_ERROR = "Invalid input in line: %d.", *OUTPUT = "railway_planner_output.txt",
		*MINIMUM_MESSAGE = "The minimal price is: %d", *WRITE = "w+", *READ = "r", *COMMA = ",";

/**
 * struct for keeping information about a single rail part
 */
typedef struct railPart
{
	char left, right;
	int length, price, leftIndex, rightIndex;
} railPart;

/**
 * create new file with error message
 * @param message to be writen
 * @param flag describing if input file was opened or not
 * @return -1 or 1
 */
int error(const char *message, int flag);

/**
 * regular expression matching
 * @param string to be matched
 * @param regex regular expression for matching
 * @return 0 if successful, else other
 */
int regexMatch(char string[LINE_LENGTH], const char *regex);

/**
 * dynamically-programmed algorithm running in O(N*|K|*|L|) time
 * @param length current length for which to calculate minimum price
 * @param setIndex current right-switch for which to find minimum price
 * @param matrixPtr pointer to 2-D array holding all minimum prices
 * @param railPtr pointer to all rail parts
 * @param setPtr pointer to set of all switches
 * @return minimum computed for specific length and right-end switch
 */
int dynamicProgramming(int length, int setIndex, int *matrixPtr, const railPart *const railPtr,
					   const char *const setPtr);

/**
 * parse all file lines for minimum procedure
 * @return success if file was parsed successfully and all lines were legal,
 * otherwise failure
 */
int parseFile();

/**
 * parse first two lines
 * @param regex regular expression
 * @param lineNum current line number in file
 * @return relevant data if successful, otherwise create file with error data and exit
 */
int parseLine(const char *regex, int lineNum);

/**
 * parse third line of file
 * @param size of set from previous line (assumed to be correct)
 * @param regex regular expression for pattern matching
 * @param lineNum current line number in file
 * @return pointer to char array if parsing successful, else NULL
 */
char *parseSet(int size, const char *regex, int lineNum);

/**
 * input all characters in line to array
 * @param setPtr pointer to array
 */
void makeSetArray(char *const ptr);

/**
 * parse file-lines remaining to end, add these to array of rail parts
 * @param lineNum current line number
 * @param regex regular expression for pattern matching
 * @param setPtr pointer to set of switch-types
 * @return pointer to array if successful, otherwise NULL
 */
railPart *addPartsToArray(int lineNum, const char *regex, const char *const setPtr);

/**
 * check that ends of a rail part are legal
 * @param c end type
 * @param setPtr pointer to all switch types
 * @return
 */
int checkChars(char c, const char *const ptr);

/**
 * finds the minimum price for a railway of a certain length
 * using possible rail parts
 * @param railPtr pointer to array of rail parts
 * @param setPtr pointer to array of end-types
 * @return minimum price
 */
int minimumProcedure(const railPart *const railPtr, char *const setPtr);

/**
 * create file with relevant data.
 * @param message data to be written in file
 * @param num number corresponding to message
 * @param flag error was encountered while parsing or not
 * @return failure exit code in case of error, else success code
 */
int makeFile(const char *message, int num, int flag);

/**
 * fills matrix with zeroes in first row, otherwise "infinity"
 * @param row matrix row
 * @param col matrix column
 * @param matrixPtr pointer to 2-D matrix
 */
void fillMatrix(int row, int col, int *matrixPtr);

/**
 * finds the minimum price in the cost matrix after running the dynamically
 * programmed algorithm.
 * @param matrixPtr pointer to matrix
 * @param col matrix columns
 * @return minimum price
 */
int getMinimumPrice(const int *matrixPtr, int col);

/**
 * free pointers to switch type and rail part arrays, set to null
 * @param pointer to setPtr
 * @param pointer to railPtr
 */
void freePointers(char **setPtr, railPart **railPtr);

/**
 * receive argument from user specifying file to parse
 * @param argc arg num
 * @param argv program name
 * @return success in case file was parsed successfully, else failure
 */
int main(int argc, char *argv[])
{
	//check if the correct number of arguments was given by the user
	if (argc == ARG_NUM)
	{
		//attempt to open file
		filePointer = fopen(argv[1], READ);
		if (filePointer)
		{
			//file found
			return parseFile();
		}
		//file not found
		return error(FILE_ERROR, !FILE_OPENED);
	}
	//wrong arg number
	return error(USAGE_ERROR, !FILE_OPENED);
}

/**
 * parse all file lines for minimum procedure
 * @return success if file was parsed successfully and all lines were legal,
 * otherwise failure
 */
int parseFile()
{
	int i = 1;
	//parse first line
	railLength = parseLine(LENGTH_REGEX, i);
	if (railLength != NO_SOLUTION)
		//if length is legal
	{
		//parse second line
		setSize = parseLine(SET_SIZE_REGEX, ++i);
		if (setSize != NO_SOLUTION)
			//if size of set is legal
		{
			//parse third line
			char *setPtr = parseSet(setSize, SET_REGEX, ++i);
			//if line is legal
			if (setPtr)
			{
				//parse chars in line
				makeSetArray(setPtr);
				//parse file till end
				railPart *railPtr = addPartsToArray(++i, RAIL_PART_REGEX, setPtr);
				//if parsing succeeded and all lines were legal
				if (railPtr)
				{
					//do minimum procedure
					int minimum = minimumProcedure(railPtr, setPtr);
					freePointers(&setPtr, &railPtr);
					//create file with result
					return makeFile(MINIMUM_MESSAGE, minimum, EXIT_SUCCESS);
				}
				//if parsing rail parts failed free previous pointer
				free(setPtr);
				setPtr = NULL;
			}
		}
	}
	return EXIT_FAILURE;
}

/**
 * free pointers to switch type and rail part arrays, set to null
 * @param pointer to setPtr
 * @param pointer to railPtr
 */
void freePointers(char **setPtr, railPart **railPtr)
{//free allocated memory
	free(*railPtr);
	free(*setPtr);
	//set pointers to NULL
	*railPtr = NULL;
	*setPtr = NULL;
}

/**
 * dynamically-programmed algorithm running in O(N*|K|*|L|) time
 * @param length current length for which to calculate minimum price
 * @param setIndex current right-switch for which to find minimum price
 * @param matrixPtr pointer to 2-D array holding all minimum prices
 * @param railPtr pointer to all rail parts
 * @param setPtr pointer to set of all switches
 * @return minimum computed for specific length and right-end switch
 */
int dynamicProgramming(int length, int setIndex, int *matrixPtr, const railPart *const railPtr,
					   const char *const setPtr)
{
	//get cost corresponding to length and right-end from cost matrix
	int value = matrixPtr[length * setSize + setIndex];
	if (value != INT_MAX)
		//value was memoized in a previous function call, and is minimal
	{
		return value;
	}
	for (int i = 0; i < partNum; i++)
	{
		//find relevant rail parts to compute minimal price
		if (railPtr[i].right == setPtr[setIndex] && railPtr[i].length <= length)
		{
			//recursive call- compute sub-problem
			int prev = dynamicProgramming(length - railPtr[i].length, railPtr[i].leftIndex,
										  matrixPtr, railPtr, setPtr);
			//if no solution to sub-problem
			if (prev == INT_MAX)
			{
				continue;
			}
			//price for rail part
			int current = railPtr[i].price + prev;
			if (current < value)
			{
				//price for current rail piece is minimal so far
				value = current;
			}
		}
	}
	//memoize minimal value in 2-d matrix
	matrixPtr[length * setSize + setIndex] = value;
	return value;
}

/**
 * finds the minimum price for a railway of a certain length
 * using possible rail parts
 * @param railPtr pointer to array of rail parts
 * @param setPtr pointer to array of end-types
 * @return minimum price
 */
int minimumProcedure(const railPart *const railPtr, char *const setPtr)
{
	//set matrix boundaries
	int row = railLength + 1, col = setSize;
	//allocate memory for 2-d matrix
	int *matrixPtr = (int *) malloc(row * col * sizeof(int));
	if (!matrixPtr)
		//memory allocation failed
	{
		return NO_SOLUTION;
	}
	fillMatrix(row, col, matrixPtr);
	for (int i = 0; i < col; i++)
	{
		//compute minimal price for every end-type, put value in corresponding cell of matrix
		matrixPtr[railLength * col + i] = dynamicProgramming(railLength, i, matrixPtr, railPtr,
															 setPtr);
	}
	//find minimum price in last row
	int minimum = getMinimumPrice(matrixPtr, col);
	free(matrixPtr);
	matrixPtr = NULL;
	return minimum == INT_MAX ? NO_SOLUTION : minimum;
}

/**
 * this function finds the minimum price in the cost matrix after running the dynamically
 * programmed algorithm.
 * @param matrixPtr pointer to matrix
 * @param col matrix columns
 * @return minimum price
 */
int getMinimumPrice(const int *const matrixPtr, int col)
{
	int minimum = INT_MAX;
	for (int j = 0; j < col; j++)
	{
		int currentPrice = *(matrixPtr + railLength * col + j);
		if (currentPrice < minimum)
		{
			minimum = currentPrice;
		}
	}
	return minimum;
}

/**
 * this function fills matrix with zeroes in first row, otherwise "infinity"
 * @param row matrix row
 * @param col matrix column
 * @param matrixPtr pointer to 2-D matrix
 */
void fillMatrix(int row, int col, int *matrixPtr)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//fill in zeroes for first row, else fill in "infinity"
			*(matrixPtr + i * col + j) = (i == 0 ? 0 : INT_MAX);
		}
	}
}

/**
 * parse file-lines remaining to end, add these to array of rail parts
 * @param lineNum current line number
 * @param regex regular expression for pattern matching
 * @param setPtr pointer to set of switch-types
 * @return pointer to array if successful, otherwise NULL
 */
railPart *addPartsToArray(int lineNum, const char *regex, const char *const setPtr)
{
	int len = ALLOC_SIZE;
	railPart *railPtr = (railPart *) malloc(ALLOC_SIZE * sizeof(railPart));
	while (fgets(line, LINE_LENGTH, filePointer) != NULL)
		//parse till end of file
	{
		if (partNum == len)
			//array size was reached
		{
			len += ALLOC_SIZE;
			//allocate more memory
			railPtr = realloc(railPtr, len * sizeof(railPart));
		}
		int pattern = regexMatch(line, regex);
		if (pattern == EXIT_SUCCESS)
		{
			//add data to new railPart struct in array
			sscanf(line, "%c,%c,%d,%d", &railPtr[partNum].left, &railPtr[partNum].right,
				   &railPtr[partNum].length, &railPtr[partNum].price);
			railPtr[partNum].leftIndex = checkChars(railPtr[partNum].left, setPtr);
			railPtr[partNum].rightIndex = checkChars(railPtr[partNum].right, setPtr);
			//check if ends are legal
			if (railPtr[partNum].leftIndex != NO_SOLUTION &&
				railPtr[partNum].rightIndex != NO_SOLUTION)
			{
				//continue parsing file
				partNum++, lineNum++;
				continue;
			}
		}
		//error encountered
		makeFile(INPUT_ERROR, lineNum, EXIT_FAILURE);
		free(railPtr);
		return NULL;
	}
	//parsing successful
	return railPtr;
}

/**
 * check that ends of a rail part are legal
 * @param c end type
 * @param setPtr pointer to all switch types
 * @return
 */
int checkChars(char c, const char *const setPtr)
{
	//go over array of all legal char types
	for (int i = 0; i < setSize; i++)
	{
		//this character was found
		if (setPtr[i] == c)
		{
			//return index in array
			return i;
		}
	}
	//char wasn't found
	return NO_SOLUTION;
}

/**
 * input all characters in line to array
 * @param setPtr pointer to array
 */
void makeSetArray(char *const setPtr)
{
	int i = 0;
	//parse third line
	char *token = strtok(line, COMMA);
	while (token != NULL)
		//while line end wasn't found
	{
		//input char into array
		setPtr[i] = *token;
		token = strtok(NULL, COMMA);
		i++;
	}
}

/**
 * parse third line of file
 * @param size of set from previous line (assumed to be correct)
 * @param regex regular expression for pattern matching
 * @param lineNum current line number in file
 * @return pointer to char array if parsing successful, else NULL
 */
char *parseSet(int size, const char *regex, int lineNum)
{
	//this line is assumed to exist, according to the exercise guidelines
	fgets(line, LINE_LENGTH, filePointer);
	int pattern = regexMatch(line, regex);
	if (pattern == EXIT_FAILURE)
		//line doesn't adhere to format
	{
		makeFile(INPUT_ERROR, lineNum, EXIT_FAILURE);
		return NULL;
	}
	//allocate memory for char array, return pointer
	return (char *) malloc(size * sizeof(char));
}

/**
 * parse first two lines
 * @param regex regular expression
 * @param lineNum current line number in file
 * @return relevant data if successful, otherwise create file with error data and exit
 */
int parseLine(const char *regex, int lineNum)
{
	//check if line is empty
	if (fgets(line, LINE_LENGTH, filePointer))
	{
		int pattern = regexMatch(line, regex);
		if (pattern == EXIT_SUCCESS)
		{
			//return number data
			return (int) strtol(line, NULL, BASE_TEN);
		}
		//line doesn't adhere to format
		return makeFile(INPUT_ERROR, lineNum, EXIT_FAILURE);
	}
	//file is empty
	fclose(filePointer);
	return error(EMPTY_ERROR, FILE_OPENED);
}

/**
 * create file with relevant data.
 * @param message data to be written in file
 * @param num number corresponding to message
 * @param flag error was encountered while parsing or not
 * @return failure exit code in case of error, else success code
 */
int makeFile(const char *message, int num, int flag)
{
	//close input file
	fclose(filePointer);
	filePointer = fopen(OUTPUT, WRITE);
	if (filePointer)
		//managed to create new file
	{
		//format message to print to new file
		fprintf(filePointer, message, num);
		fclose(filePointer);
	}
	filePointer = NULL;
	//if flag is failure- return no solution, else return success
	return flag ? NO_SOLUTION : EXIT_SUCCESS;
}

/**
 * regular expression matching
 * @param string to be matched
 * @param regex regular expression for matching
 * @return 0 if successful, else other
 */
int regexMatch(char string[LINE_LENGTH], const char *regex)
{
	//compile regex
	regcomp(&regexCompiled, regex, REG_EXTENDED);
	int result = regexec(&regexCompiled, string, 0, NULL, 0);
	//free regex to use once more for next pattern match
	regfree(&regexCompiled);
	//if pattern was matched return success, else return error
	return result == EXIT_FAILURE ? EXIT_FAILURE : EXIT_SUCCESS;
}

/**
 * create new file with error message
 * @param message to be writen
 * @param flag describing if input file was opened or not
 * @return -1 or 1
 */
int error(const char *message, int flag)
{
	filePointer = fopen(OUTPUT, WRITE);
	if (filePointer)
		//managed to create new file
	{
		fputs(message, filePointer);
		fclose(filePointer);
		filePointer = NULL;
	}
	//if file was found return -1, else return 1
	return flag ? NO_SOLUTION : EXIT_FAILURE;
}

