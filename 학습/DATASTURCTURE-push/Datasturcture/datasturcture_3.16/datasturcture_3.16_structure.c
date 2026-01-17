#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

struct student {
	int id;
	char name[20];
	double score;
};

typedef struct student Student;

void printf_student(Student s);
Student Max_student(Student s[], int n);

int main() {
	struct student s1 = { 1, "park", 80.5 }, s2;
	Student s[3];
	int i;

	s2.id = 2;
	strcpy(s2.name, "kim");

	printf_student(s1);

	for (i = 0; i < 3; i++) {
		printf("ID. name . score >>\n");
		scanf("%d %s %lf", &s[i].id, s[i].name, &s[i].score);
	}

	for (i = 0; i < 3; i++) {
		printf("ID = %d NAME = %s score = %lf\n", s[i].id, s[i].name, s[i].score);
		//or printf_student(s[i]);
	}

	s2 = Max_student(s, 3);
	printf("MAX score student is : \n");
	printf_student(s2);


	return 0;
}

void printf_student(Student s) {
	printf("ID = %d NAME = %s score + %lf\n", s.id, s.name, s.score);

}

Student Max_student(Student s[], int n) {
	int i;
	Student Max;
	Max = s[0];
	for (i = 1; i < n; i++) {
		if (Max.score < s[i].score) {
			Max = s[i];
		}
	}
	return Max;
}