#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<assert.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	FILE* file = fopen(fileName, "rt");
	if (file == NULL) {
		printf("open error");
	}
	int count = 0;
	char buffer[1023];
	
	while (fgets(buffer, 1023, file)) {
		//Student* student = (Student*)realloc(sizeof(Student),student);
		count++;

	}

	*numberOfStudents = count;
	int* arr = (int*)malloc(sizeof(int) * count);
	if (arr == NULL) {
		printf("alocation failed");
		exit(1);
	}
	count = 0;
	rewind(file);

	while (fgets(buffer, 1023, file)) {
		
		*(arr + count) =  countPipes(buffer, 1023); //--
		count++;
	}
	*coursesPerStudent = arr;
	fclose(file);
}

int countPipes(const char* lineBuffer, int maxCount)
{
	int count = 0;
	char* temp = NULL;
	if (lineBuffer == NULL) {
		return -1;
	}
	else if(maxCount <= 0) { 
		return 0;
	}
	temp = strchr(lineBuffer, '|');
	while (temp != NULL) {
		count++;
		lineBuffer = temp + 1;
		temp = strchr(lineBuffer, '|');
	}
	return count;
}

char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	char* pipe = "|";
	FILE* file = fopen(fileName, "rt");
	if (file == NULL) {
		printf("open error");
	}
	int count = 0;
	char buffer[1023];
	char* tempStudent;
	
	char*** students = (char***)malloc(sizeof(char**) * (*numberOfStudents));

	for (int i = 0; i < *numberOfStudents; i++)
	{		
		*(students+i) = (char*)malloc(sizeof(char*) * (*(*(coursesPerStudent + i)))*2); //coursesPerStudent[i]	
	}

	while (fgets(buffer, 1023, file)) {
		count++;
		tempStudent = strtok(buffer, pipe);
	}
	fclose(file);
	return students;
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	int grade = 0;
	if (factor < (-20) && (factor > 20))
		printf("error- try again");
	for (int i = 0;i < numberOfStudents; i++) {
		for (int j = 0; j < *(*(*(students))); j++) {
			if (strstr(students, courseName)) {
				grade = atoi(students[i][j]);
				grade += factor;
				if (grade > 100)
					grade = 100;
				if (grade < 0)
					grade = 0;
				_itoa(grade, students[i][j], 10);
			}
		}
		
	}
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	FILE* file =fopen("studentList.txt", "w");

	for (int i = 0; i < numberOfStudents; i++) {
		for (int j = 0; j < *(*(*(students))); j++) {
			fprintf(file, "%s", students);
			free(students[i][j]);
		}
		free(students[i]);
	}
	free(coursesPerStudent);
	free(students);
	fclose(file);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	//add code here
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}
