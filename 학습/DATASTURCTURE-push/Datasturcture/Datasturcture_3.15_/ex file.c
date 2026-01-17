#include <stdio.h>
#define MAX_ELEMENTS 50000
#define _CRT_SECURE_NO_WARNINGS

/*
int find_max_score(int score[], int n) {
	int i, tmp;
	tmp = score[0];
	for (i = 0; i < n; i++) {
		if (score[i] > tmp) {
			tmp = score[i];
		}
	}
	return tmp;
}
*/

//Use pointer
int find_max_score(int score[], int n) {
	int i, tmp;
	tmp = *(score+0);
	for (i = 0; i < n; i++) {
		if (*(score + i) > tmp) {
			tmp = *(score + i);
		}
	}
	return tmp;
}

int main() {
	int a[5], b[5];
	int i;
	int maxa, maxb;

	for (i = 0; i < 5; i++) {
		printf("Please, Enter two numbers >> ");
		scanf("%d %d", &a[i], &b[i]);
	}

	maxa = find_max_score(a, 5);
	maxb = find_max_score(b, 5);

	if (maxa > maxb)
	{
		printf("Array a");
	}
	else if (maxa < maxb)

	{
		printf("Array b");
	}
	else // maxa == maxb
	{
		printf("Array a and b have same maximun number");
	}

	return 0;
}



