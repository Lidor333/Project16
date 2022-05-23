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
	studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

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
	
	while (fgets(buffer, 1023, file)!=NULL) {
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
		
		*(arr + count) =  countPipes(buffer, 1023); 
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
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);
	char* pipe = "|";
	char* psik = ",";
	int details;


	FILE* file = fopen(fileName, "rt"); //open a  file for only reading
	if (file == NULL) {
		printf("open error");
		exit(1);
	}
	
	
	char*** students = (char***)malloc(sizeof(char**) * (*numberOfStudents)); //make arr of 3 pointers of students
	if (!students) {
		printf("error\n");
		exit(1);
	}
	 
	
	for (int i = 0; i < *numberOfStudents; i++)
	{
		char* buffer =  (char*)malloc(1023 * sizeof(char));
		if (!buffer) {
			exit(1);
		}
		char* bufferp = buffer;
		details = (*coursesPerStudent)[i] * 2 + 1;	
		students[i] = (char**)malloc(details * sizeof(char*));
		if (!students[i]) {
			exit(1);
		}

		fgets(buffer, 1023, file);

		for (int j = 0; j < details; j = j + 2) {
   
				char* variable = strtok(buffer, pipe);
				students[i][j] = (char*)malloc(strlen(variable) * sizeof(char) + 1);
				if (!students[i][j + 1]) {
					exit(1);
				}
				strcpy(students[i][j],variable);
				buffer = buffer + strlen(variable) + 1;
				char* variable1 = strtok(buffer, psik);
				students[i][j+1] = (char*)malloc(strlen(variable1)*sizeof(char) + 1);
				if (!students[i][j+1]) {
					exit(1);
				}
				strcpy(students[i][j+1], variable1);
				buffer = buffer + strlen(variable1) + 1;
		}
		free(bufferp);
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
	printStudentArray(students, coursesPerStudent, numberOfStudents);
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
	FILE* pfile;
	pfile = fopen("studentListcopy.txt", "w");
	if (pfile == NULL)
	{
		printf("error\n");
		exit(1);
	}
	int i = 0, j = 0;
	char buffer[1023];
	char* token = NULL;
	for(int i=0;i<numberOfStudents;i++){
	
		for(int j=0;j<coursesPerStudent[i]*+1;j++){
			strcpy(buffer, students[i][j]);
			if (j == (coursesPerStudent[i] * 2))
			{
				token = strtok(buffer, "\n");
				fputs(token, pfile);
				fputc('\n', pfile);
				break;
			}
			fputs(buffer, pfile);
			if (j % 2 != 0)
			{
				fputc(',', pfile);
			}
			else
			{
				fputc('|', pfile);
			}
		}
		fputc('\n', pfile);
	}
	fclose(pfile);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	FILE* pbfile;
	pbfile = fopen(fileName, "wb");
	if (!pbfile)
		printf("Unable to open file!");
	else
	{
		fwrite(&numberOfStudents, sizeof(int), 1, pbfile); //add a number of students to the start of file
		int i = 0;
		for (; i < numberOfStudents; i++)
		{
			fwrite(&students[i].name, 35 * sizeof(char), 1, pbfile); //write the name of student
			fwrite(&students[i].numberOfCourses, sizeof(int), 1, pbfile); // write number of courses of student
		}
		for (int j = 0; j < students[i].numberOfCourses; j++)
		{
			fwrite(&students[i].grades[j].courseName, 35 * sizeof(char), 1, pbfile); //write the name of the courses
			fwrite(&students[i].grades[j].grade, sizeof(int), 1, pbfile);//write the grades of the courses
		}

	}
	fclose(pbfile);
}

Student* readFromBinFile(const char* fileName)
{
	int numberOfStudents;
	FILE* pbfile;
	pbfile = fopen(fileName, "rb");
	if (!pbfile)
		printf("Unable to open file!");
	else
	{
		fread(&numberOfStudents, sizeof(int), 1, pbfile);
		Student* students = (Student*)calloc(numberOfStudents, sizeof(Student));//Dynamic allocation of the number of students
		int i = 0;
		for (; i < numberOfStudents; i++)
		{
			fread(&students[i].name, 35 * sizeof(char), 1, pbfile); //read the name of student
			fread(&students[i].numberOfCourses, sizeof(int), 1, pbfile); // read number of courses of student
		}

		students[i].grades = (StudentCourseGrade*)calloc(students[i].numberOfCourses, sizeof(StudentCourseGrade));//Dynamic allocation of the number of courses

		for (int j = 0; j < students[i].numberOfCourses; j++)
		{
			fread(&students[i].grades[j].courseName, 35 * sizeof(char), 1, pbfile); //read the name of the courses
			fread(&students[i].grades[j].grade, sizeof(int), 1, pbfile);//read the grades of the courses
		}
	}
	fclose(pbfile);
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	Student* studentList = (Student*)calloc(numberOfStudents, sizeof(Student)); //Dynamic allocation of the student list
	if (!studentList)//(studentList==NULL)--> allocation didn't succeed
	{
		printf("ERROR!out of memory!\n");
		return;
	}

	for (int i = 0; i < numberOfStudents; i++)
	{
		strcpy((studentList[i]).name, students[i][0]); //copy the names of students
		studentList[i].numberOfCourses = coursesPerStudent[i]; //copy the number of course per student
		studentList[i].grades = (StudentCourseGrade*)calloc(coursesPerStudent[i], sizeof(StudentCourseGrade));//Dynamic allocation of the grades per student
		for (int j = 1, k = 0; j <= coursesPerStudent[i] * 2; j += 2)
		{
			strcpy(studentList[i].grades[k].courseName, students[i][j]);//copy course name
			studentList[i].grades[k].grade = atoi(students[i][j + 1]);// copy course grade for every course from string to integer
			k++;
		}
	}
	return(studentList);
}
