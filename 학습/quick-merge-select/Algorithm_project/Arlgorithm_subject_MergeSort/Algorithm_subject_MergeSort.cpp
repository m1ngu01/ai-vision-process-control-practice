/*
<Merge Sort>
>> Simple code <<
 MergeSort ( A , p , q )
 input ( A [ p ] ~ A [ q ] ) 
 output ( sort A [ p ] ~ A [ q ] )
 if ( p < q )
	k = (p + q) / 2 
	MergeSort ( A , p , k )
	MergeSort ( A , k + 1 , q )
	merge A [ p ] ~ A [ k ] and A [ k + 1 ] ~ A [ q ]
*/

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100

void merge(int A[], int p, int q, int r) {
    int i = p, j = q + 1, k = 0;
    int size = r - p + 1;
    int* tmp = (int*)malloc(sizeof(int) * (size));

    while (i <= q && j <= r) {
        if (A[i] <= A[j]) {
            tmp[k++] = A[i++];
        }
        else {
            tmp[k++] = A[j++];
        }
    }

    while (i <= q) {
        tmp[k++] = A[i++];
    }
    while (j <= r) {
        tmp[k++] = A[j++];
    }

    for (i = p, k = 0; i <= r; i++, k++) {
        A[i] = tmp[k];
    }
    free(tmp);
}

void mergeSort(int A[], int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        mergeSort(A, p, q);
        mergeSort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

int main() {

    int a[MAX_SIZE];
    int n = 0;

    FILE* fp = fopen("data.txt", "r");
    if ( fp == NULL){
        printf("erorr");
        return -1;
    }

    while(1){
        int num;
        if( fscanf(fp, "%d", &num) == EOF) break;
        a[n++] = num;
    
    }
    /*---------------------------------------------------------*/
	mergeSort(a,0, n-1);
	for (int i = 0; i < 8; i++) {
		printf("%d ", a[i]);
	}
    /*---------------------------------------------------------*/
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