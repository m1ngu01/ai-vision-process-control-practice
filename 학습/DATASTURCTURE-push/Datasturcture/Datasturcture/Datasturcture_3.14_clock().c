#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEMENTS 100
int score[MAX_ELEMENTS];

int find_max_score(int n)
{
    int i, tmp;
    tmp = score[0];
    for (i = 1;i < n;i++) {
        if (score[i] > tmp) {
            tmp = score[i];
        }
    }
    return tmp;
}

void main(void)
{
    clock_t start, finish;
    double duration;
    start = clock();
    
    srand(time(NULL));

    // 실행 시간을 측정하고자 하는 코드....
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        score[i] = rand();
    }
    int res = find_max_score(MAX_ELEMENTS);
  
    finish = clock();
    // ....
    duration = (double)(finish - start) / CLOCKS_PER_SEC;

    //Check result
    printf("%d = HIGH NUM \n", res);
    printf("%f 초입니다.\n", duration);
}