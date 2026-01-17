#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 10


typedef struct Point {
    int x;
    int y;
} Point;

double distance(Point p1, Point p2)
{
    double result;
    double a, b, c;

    if (p1.x == p2.x) //x축의 위치가 같을경우 
    {
        result = p2.y - p1.y;
        fabs(result); //절댓값 함수fabs를 이용해 음수일경우 대비 
    }
    else if (p1.y == p2.y) //y축의 위치가 같을경우 
    {
        result = p2.x - p1.x;
        fabs(result);
    }
    else //피터고라스의 정리를 활용해야 하는경우 
    {
        a = p1.x - p2.x;
        b = p1.y - p2.y;
        c = (pow(a, 2)) + (pow(b, 2));
        result = sqrt(c);
        return result;
    }
}
void Read_File(Point a[], int size)
{
    int i, count;
    FILE* fp = fopen("data.txt", "r"); // 읽기용 파일 열기

    if (fp == NULL) // 파일 열기 실패시
    {
        printf("파일열기 실패\n");
        return;
    }
    printf("파일읽기 성공!\n");

    for (i = 0; i < size; i++)
    {
        if (fscanf(fp, "%d", &count) == 1)
        {
            a[i].x = count;

            if (fscanf(fp, "%d", &count) == 1)
                a[i].y = count;
            else
                printf("범위를 초과했습니다.\n");
        }
        else
        {
            printf("범위를 초과했습니다.\n");
            return;
        }
    }
    for (i = 0; i < size; i++)
        printf("(%d, %d)\n", a[i].x, a[i].y);
    fclose(fp);
}


double Closest_Pair(Point P[], int left, int right, Point* Pair[])
{
    int i, j;

    if (right - left <= 2) //점의 개수가 3개 미만인 경우
    {
        double minDistance = 99999999;
        for (i = left; i < right; i++)
        {
            for (j = i + 1; j < right; j++)
            {
                double dist = distance(P[i], P[j]);
                if (dist < minDistance)
                {
                    minDistance = dist;
                    Pair[0] = &P[i];
                    Pair[1] = &P[j];
                }
            }
        }
        return minDistance;
    }
    // 중앙점 인덱스 계산
    int mid = (left + right) / 2;

    // 좌측 부분과 우측 부분에 대해 재귀적으로 최근접 점의 쌍과 거리 찾기
    Point* leftClosestPair[2];
    double minDistanceLeft = Closest_Pair(P, left, mid, leftClosestPair);
    Point* rightClosestPair[2];
    double minDistanceRight = Closest_Pair(P, mid + 1, right, rightClosestPair);

    // 좌측 부분과 우측 부분에서 찾은 최근접 점의 쌍 중에서 최소 거리 선택
    double minDistance = (minDistanceLeft < minDistanceRight) ? minDistanceLeft : minDistanceRight;
    Point** minDistancePair = (minDistanceLeft < minDistanceRight) ? leftClosestPair : rightClosestPair;

    // 가로지르는 영역에서 더 가까운 점의 쌍이 있는지 확인
    double minDistanceStrip = 99999999;
    Point* stripPair[2];
    int stripPairCount = 0;

    for (i = left; i <= right; i++) {
        if (abs(P[i].x - P[mid].x) < minDistance) {
            for (j = i + 1; j <= right; j++) {
                if (abs(P[j].x - P[mid].x) < minDistance) {
                    double dist = distance(P[i], P[j]);
                    if (dist < minDistance) {
                        minDistance = dist;
                        stripPair[0] = &P[i];
                        stripPair[1] = &P[j];
                        stripPairCount = 1;
                    }
                    else if (dist == minDistance) {
                        if (stripPairCount < 2) {
                            stripPair[stripPairCount++] = &P[i];
                            stripPair[stripPairCount++] = &P[j];
                        }
                    }
                }
                else {
                    break;
                }
            }
        }
    }

    // 가로지르는 영역에서의 최근접 점의 쌍과 거리 중 최소값 선택
    if (stripPairCount > 0) {
        minDistancePair = stripPair;
    }

    Pair[0] = minDistancePair[0];
    Pair[1] = minDistancePair[1];
    return minDistance;
}

int main()
{
    Point P[SIZE];
    Read_File(P, SIZE);
    Point* Pair[2];
    Closest_Pair(P, 0, SIZE, Pair);

    int i;
    FILE* fp = fopen("result.txt", "w");// 쓰기용 파일 열기
    if (fp == NULL) 					// 파일 열기 실패시
    {
        printf("파일열기 실패\n");
        return -1;
    }

    fprintf(fp, "최근접 점의 쌍은 ");
    for (i = 0; i < 2; i++)
    {
        fprintf(fp, "(");
        fprintf(fp, "%d ", Pair[i]->x);
        fprintf(fp, "%d", Pair[i]->y);
        fprintf(fp, ") ");
    }
    fclose(fp);
    return 0;
}
