#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#define MAX_SIZE 100

void swap(int* a, int* b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int medians(int a[], int left, int right) {
	int mid = (left + right) / 2;
	if (((a[left] > a[mid]) && (a[left] < a[right])) || ((a[left] > a[right]) && (a[left] < a[mid]))) return left;
	if (((a[mid] > a[right]) && (a[mid] < a[left])) || ((a[mid] > a[left]) && (a[mid] < a[right]))) return mid;
	if (((a[right] > a[left]) && (a[right] < a[mid])) || ((a[right] > a[mid]) && (a[right] < a[left]))) return right;
}

int dividepivot(int a[], int left, int right) {
	int p = medians(a, left, right);
	swap(&a[left], &a[p]);
	// pivot position left side
	p = left;
	// check array without pivot ==> start check to a[pivot + 1]
	left++;

	while (left <= right) {
		while (a[p] >= a[left] && left <= right)
			left++;
		while (a[p] <= a[right] && left <= right)
			right--;
		if (left <= right)
			swap(&a[left], &a[right]);
	}
	// move pivot to its final position
	swap(&a[p], &a[right]);

	// return pivot
	return right;
}

int Selection(int a[], int left, int right, int k) {
	int p = dividepivot(a, left, right);
	int s = (p - 1) - left + 1;
	if (k <= s)
		Selection(a, left, p - 1, k);
	else if (k == s + 1)
		return a[p];
	else
		Selection(a, p + 1, right, k - s - 1);
}

int main() {

	int a[MAX_SIZE];
	int n = 0;

	FILE* fp = fopen("data.txt", "r");
	if (fp == NULL) {
		printf("파일 열기 실패\n");
		return -1;
	}

	while (1) {
		int num;
		if (fscanf(fp, "%d", &num) == EOF) break;
		a[n++] = num;
	}
	fclose(fp);
	/*------------------------------------------------------------------------*/

	int count = 6;
	printf("%d번째로 큰 수는 => %d 입니다.\n", count, Selection(a, 0, n - 1, count));
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);

	/*------------------------------------------------------------------------*/

	fp = fopen("result.txt", "w");
	if (fp == NULL) {
		printf("파일 열기 실패\n");
		return -1;
	}

	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d ", a[i]);
	}

	fclose(fp);

	printf("정렬 결과를 result.txt 파일에 저장하였습니다.\n");

}