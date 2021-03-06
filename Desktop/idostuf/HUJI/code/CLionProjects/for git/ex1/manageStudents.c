#include <stdio.h>
#include <string.h>
#include <regex.h>

//maximum size for array of students
#define MAX_SIZE 5501
//maximum length of input from user
#define INPUT_LENGTH 61
//maximum length of any field within input from user
#define FIELD_LENGTH 43
//program ran with no problems
#define SUCCESS_CODE 0
//program failed
#define FAILURE_CODE 1
//expected arguments from user
#define ARG_NUM 2

//function and struct declarations
void bestProcedure();

void merge();

void mergeSort();

void sort();

void quickSort();

int makeStudents();

int usage();

int numsCheck();

void doProcedure(const char *inp);

int error();

struct Student;

double best(struct Student student);

void showAll();

int inputCheck();

int regexMatch(char id[FIELD_LENGTH], const char *regex, const char *error, int i);

//constants
const char *MERGE = "merge", *QUICK = "quick", *BEST = "best", *QUIT = "q\n",
		*USAGE = "USAGE: manageStudents <action>",
		*RUN_PROCESS = "Enter student info. To exit press q, then enter",
		*REGEX = "[0-9]+,.+,[0-9]+,[0-9]+,.+,.+",
		*ID_REGEX = "^[1-9][0-9]{9}$", *NAME_REGEX = "^[a-zA-Z -]+$", *COUNTRY_CITY_REGEX =
		"^[a-zA-Z-]+$",
		*REGEX_ERROR = "ERROR: info must match specified format\nin line",
		*ID_ERROR = "ERROR: id must be a 10 digits number that does not start with 0\nin line",
		*GRADE_ERROR = "ERROR: grade must be an integer between 0 and 100\nin line",
		*AGE_ERROR = "ERROR: age must be an integer between 18 and 120\nin line",
		*NAME_ERROR = "ERROR: name can only contain alphabetic characters, whitespaces or '-'\n"
					  "in line",
		*CITY_ERROR = "ERROR: city can only contain alphabetic characters or '-'\nin line",
		*COUNTRY_ERROR = "ERROR: country can only contain alphabetic characters or '-'\nin line",
		*BEST_INFO = "best student info is:";

const int LOWER_GRADE = 0, LOWER_AGE = 18, HIGHER_GRADE = 100, HIGHER_AGE = 120;

//variable to keep track of number of students added to database
int studentNum;

//regex struct for input matching
regex_t regexCompiled;

/**
 * struct for saving student information
 */
typedef struct Student
{
	unsigned long id; //10-digit id, shouldn't start with 0
	int age; //int must be in range LOWER_AGE<=age<=HIGHER_AGE
	int grade; //grade must be in range LOWER_GRADE<=grade<=HIGHER_GRADE
	char name[FIELD_LENGTH]; //alphabetic values, space and hyphen
	char country[FIELD_LENGTH]; //alphabetic values
	char city[FIELD_LENGTH]; //alphabetic values
} Student;

//initialize arrays with no data
Student students[MAX_SIZE] = {{0}}, sortArray[MAX_SIZE - 1] = {{0}};

/**
 * this function is in charge of parsing input received by the user, and choosing
 * the correct procedure according to the given input
 * @param argc number of inputs received
 * @param argv array holding inputs
 * @return 0 if program ran correctly, else 1
 */
int main(int argc, char *argv[])
{
	//check if the correct number of arguments was given by the user
	if (argc != ARG_NUM)
	{
		return usage();
	}
	const char *inp = argv[1];
	//if user input is unequal to any of the expected arguments
	if (strcmp(inp, BEST) && strcmp(inp, QUICK) && strcmp(inp, MERGE) != 0)
	{
		//print informative message and exit with code 1
		return usage();
	}
	if (makeStudents() == FAILURE_CODE)
	{
		//if EOF was reached unexpectedly
		return FAILURE_CODE;
	}
	doProcedure(inp);
	return SUCCESS_CODE;
}

/**
 * this function chooses which procedure to carry out based on the user's input,
 * After inputting students to the database. The options are printing out the best
 * student's info, or sorting the students by name or grade
 * @param inp inputted received from the user
 */
void doProcedure(const char *inp)
{
	//if input received is "best"- do bestProcedure, else if input is "merge"
	// do mergeSort, else do quickSort
	strcmp(inp, BEST) == 0 ? bestProcedure() : strcmp(inp, MERGE) == 0 ? sort(0) :
											   sort(1);
}

/**
 * prints out an informative message about how to use the program, returns exit code 1
 */
int usage()
{
	printf("%s\n", USAGE);
	return FAILURE_CODE;
}

/**
 * function to create new students and add them into database.
 */
int makeStudents()
{
	//input from user
	char input[INPUT_LENGTH];
	//line number
	int i = 0;
	while (i < MAX_SIZE)
	{
		//	use regex to match input to pattern
		regcomp(&regexCompiled, REGEX, REG_EXTENDED);
		printf("%s\n", RUN_PROCESS);
		if (!fgets(input, INPUT_LENGTH, stdin))
		{
			//EOF, return failure code
			return error(REGEX_ERROR, i);
		}
		if (strcmp(input, QUIT) == 0)
			//q was received as input
		{
			break;
		}
		int status = regexec(&regexCompiled, input, 0, NULL, 0);
		//free regex for next pattern-match
		regfree(&regexCompiled);
		if (status != 0)
			//input doesn't match requirements
		{
			error(REGEX_ERROR, i);
		}
		else
		{
			if (inputCheck(input, i) == 0)
			{
				//if input matches requirements add student to database
				Student *s = &students[studentNum];
				sscanf(input, "%lu,%[^,],%d,%d,%[^,],%s", &s->id, s->name, &s->grade, &s->age,
					   s->country, s->city);
				studentNum++;
			}
		}
		i++;
	}
	return SUCCESS_CODE;
}

/**
 * function checks that input from user is legal
 * @param input from user
 * @param i line number
 * @return 1 if false, else 0
 */
int inputCheck(char input[], int i)
{
	int grade, age;
	char id[FIELD_LENGTH], name[FIELD_LENGTH], country[FIELD_LENGTH], city[FIELD_LENGTH];
	sscanf(input, "%[^,],%[^,],%d,%d,%[^,],%s", id, name, &grade, &age,
		   country, city);
	//check each field individually- if any is invalid return error code
	if (regexMatch(id, ID_REGEX, ID_ERROR, i) || regexMatch(name, NAME_REGEX, NAME_ERROR, i) ||
		numsCheck(grade, LOWER_GRADE, HIGHER_GRADE, GRADE_ERROR, i) ||
		numsCheck(age, LOWER_AGE, HIGHER_AGE, AGE_ERROR, i) ||
		regexMatch(country, COUNTRY_CITY_REGEX, COUNTRY_ERROR, i) ||
		regexMatch(city, COUNTRY_CITY_REGEX, CITY_ERROR, i))
	{
		return FAILURE_CODE;
	}
	return SUCCESS_CODE;
}

/**
 * match a string inputted by the user to a pattern
 * @param string inputted
 * @param regex pattern to match
 * @param errorString error message to be printed
 * @param i line number
 * @return
 */
int regexMatch(char string[FIELD_LENGTH], const char *regex, const char *errorString, int i)
{
	regcomp(&regexCompiled, regex, REG_EXTENDED);
	int result = regexec(&regexCompiled, string, 0, NULL, 0);
	//free regex to use once more for next pattern match
	regfree(&regexCompiled);
	return result != SUCCESS_CODE ? error(errorString, i) : SUCCESS_CODE;
}

/**
 * print out an informative error message for the user
 * @param s informative message about error
 * @param i line in which error was raised
 */
int error(const char *s, int i)
{
	printf("%s %d\n", s, i);
	return FAILURE_CODE;
}

/**
 * check if a given number lies within a given range
 * @param num to check
 * @param lower value
 * @param higher value
 * @return 0 if true, else 1
 */
int numsCheck(int num, int lower, int higher, char string[], int i)
{
	return (lower <= num) && (num <= higher) ? SUCCESS_CODE : error(string, i);
}

/**
 * print out best student's info
 * @param p pointer to a Student
 */
void showBest(Student *p)
{
	printf("%s %lu,%s,%d,%d,%s,%s\n", BEST_INFO, p->id, p->name, p->grade,
		   p->age, p->country, p->city);
}

/**
 * print out normal Student info
 * @param p pointer to a Student
 */
void show(Student *p)
{
	printf("%lu,%s,%d,%d,%s,%s\n", p->id, p->name, p->grade,
		   p->age, p->country, p->city);
}

/**
 * find student with best grade-to-age ratio in the database
 */
void bestProcedure()
{
	double isBest = 0.0;
	struct Student *p = NULL;
	for (int i = 0; i < studentNum; i++)
	{
		if (students[i].id != 0)
		//if student info isn't empty
		{
			double current = best(students[i]);
			if (current > isBest)
			{
				//pointer points to current student's address
				p = &students[i];
				isBest = current;
			}
		}
	}
	//print best student if any such was found
	if (p)
	{
		showBest(p);
	}
}

/**
 * return a Student's ratio of grade:age
 * @param student
 * @return ratio
 */
double best(Student student)
{
	return (double) student.grade / student.age;
}

/**
 * print all students inputted in the database to stdout
 * @param p pointer to student within array
 */
void showAll(Student *p)
{
	for (int i = 0; i < studentNum; i++)
	{
		if (students[i].id != 0)
		{
			show(p);
		}
		p++;
	}
}

/**
 * sort the students in database by grade
 * @param arr array of Students
 * @param arrayLength
 */
void mergeSort(Student arr1[], Student arr2[], int start, int end)
{
	int length = (end - start + 1);
	if (length < 2)
	{
		//nothing to sort
		return;
	}
	int mid = (start + end) / 2;
	mergeSort(arr1, arr2, start, mid);
	mergeSort(arr1, arr2, mid + 1, end);
	merge(arr1, arr2, start, end);
}

/**
 * merge two sorted arrays in O(N) to a sorted array combined of both
 * @param arr1 array from which to merge elements
 * @param arr2 array to copy merged elements into
 * @param start index from which to start merge
 * @param end  index from which to end merge
 */
void merge(Student arr1[], Student arr2[], int start, int end)
{
	int i, j, k;
	int mid = (start + end) / 2;
	for (i = start, j = mid + 1, k = start; i <= mid && j <= end; k++)
	{
		//if arr1[i] is smaller, put it before arr1[j] in arr2
		arr2[k] = (arr1[i].grade <= arr1[j].grade) ? arr1[i++] :
				  arr1[j++];
	}
	while (k < end + 1)
		//fill in remaining elements
	{
		arr2[k++] = (i < mid + 1) ? arr1[i++] : arr1[j++];
	}
	//copy merged elements back into array
	for (k = start; k <= end; k++)
	{
		arr1[k] = arr2[k];
	}
}

/**
 * choose how to sort database according to flag received
 * @param flag 0 or 1
 */
void sort(int flag)
{
	//if 1 do quick sort, else do merge sort
	flag ? quickSort(students, 0, studentNum - 1) :
	mergeSort(students, sortArray, 0, studentNum - 1);
	showAll(students);
}

/**
 * swap two elements within the Student array
 * @param pa pointer to a student
 * @param pb pointer to another student
 */
void swap(Student *pa, Student *pb)
{
	Student temp = *pa;
	*pa = *pb;
	*pb = temp;
}

/**
 * sort the array by Student name
 * @param arr array of Students
 * @param start first index to sort from
 * @param end last index to sort up to
 */
void quickSort(Student arr[], int start, int end)
{
	int length = (end - start + 1);
	if (length < 2)
	{
		//nothing to sort
		return;
	}
	int storeIndex = start;
	for (int i = start + 1; i <= end; i++)
	{
		if (strcmp(arr[i].name, arr[start].name) < 0)
		{
			storeIndex++;
			swap(arr + storeIndex, arr + i);
		}
	}
	//pivot created
	swap(arr + start, arr + storeIndex);
	if (storeIndex > start)
	{
		//recursively call algorithm on sub-array left to pivot
		quickSort(arr, start, storeIndex - 1);
	}
	if (storeIndex < end)
	{
		//recursively call algorithm on sub-array right to pivot
		quickSort(arr, storeIndex + 1, end);
	}
}

