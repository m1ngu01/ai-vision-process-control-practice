/*
#include <stdio.h>

void main()
{
	int A[5] = { 10, 20, 30 };
	int B[5], i, x = 2018, y = 0;

	y = x;

	//input A[]
	printf("input A");
	for (i = 0; i < 5; i++)
	{
		scanf("%d", &A[i]);
	}

	//input B[]
	printf("input B");
	for (i = 0; i < 5; i++)
	{
		scanf("%d", &B[i]);
	}

	//Check big num A[], B[]
	int check_tmp;
	check_tmp = A[0];
	for (i = 0; i < 5; i++) {
		if (check_tmp < A[i]) {
			check_tmp = A[i];
		}
	}
	printf("Max num in A[]= %d\n", check_tmp);

	check_tmp = B[0];
	for (i = 0; i < 5; i++) {
		if (check_tmp < B[i]) {
			check_tmp = B[i];
		}
	}
	printf("Max num in b[]= %d\n", check_tmp);

	//copy A[] to B[]
	for (i = 0; i < 5; i++)
		B[i] = A[i];

	printf("변수 복사 결과: x=%d, y=%d\n", x, y);
	printf("배열 복사 결과: \n");

	//print reslut a, b array
	for (i = 0; i < 5; i++) {
		printf("A[%d] = %d\t", i, A[i]);
		printf("B[%d] = %d\n", i, B[i]);
	}
}
*/