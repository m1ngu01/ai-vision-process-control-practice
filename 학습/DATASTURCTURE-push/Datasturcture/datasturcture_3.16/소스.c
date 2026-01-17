#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
struct student {
	int id;
	char name[20];
	double score;
};

typedef struct student Student;

void printf_student(Student s);

int main() {
	struct student s1 = (1, "park", 80.5), s2;
	Student s[3];
	int i;

	s2.id = 2;
	strcpy(s2.name, "kim");

	printf_student(s1);

	for (i = 0; i < 3; i++) {
		printf("ID. name . score >>");
		scanf("%d %s %lf", % s[i].id, s[i].name, % s[i].score);
	}

	for (i = 0; i < 3; i++) {
		printf("ID = %d NAME = %s score + %lf", s[i].id, s[i].name, s[i].score);
		//or printf_student(s[i]);
	}

	return 0;
}

void printf_student(Student s) {
	printf("ID = %d NAME = %s score + %lf", s.id, s.name, s.score);

}