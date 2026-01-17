/*

유연하게
동적할당 사용
tok사용
eof 시 return 줄 라인



*/









#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100

typedef struct _data
{
	int num;
	int w;
	int v;
}data;


int load_file(const char* txt, data d[])
{
	FILE* fp;
	fp = fopen(txt, "r");
	char *c;
	int a;
	int i = 0;
	char str[MAX_LEN];

	int line = 0;

	while (feof(fp) == 0)
	{
		i++;
		fgets(str, MAX_LEN, fp);
		c = strtok(str, " ");
		d[i].num = atoi(c);
		c = strtok(str, " ");
		d[i].w = atoi(c);
		c = strtok(str, "\n");
		d[i].v = atoi(c);
		line++;
	}
	fclose(fp);

	return line;
}
int max(int a, int b)
{
	return a > b ? a : b;
}
void kalgo(data d[], int knapsack[][11])
{
	/*
	for (int i = 0; i < 7; i++)
	{
		//배낭의 용량이 0일때
		knapsack[i][0] = 0;
	}
	for (int w = 0; w < 11; w++)
	{
		//물건 0이란 어떤 물건도 고려하지 않을떄
		knapsack[0][w] = 0;
	}
	*/

	int ii = 6;
	int c = 10;

	for (int i = 1; i <= ii; i++)
	{
		for (int w = 1; w <= c; w++)
		{
			if (d[i].w > w)
			{
				knapsack[i][w] = knapsack[i - 1][w];
			}
			else
			{
				int a = knapsack[i - 1][w];
				int b = knapsack[i - 1][w - d[i].w] + d[i].v;
				knapsack[i][w] = max(a, b);
			}
		}
	}
}
void test_print(int knapsack[][11])
{
	for (int i = 0; i < 7; i++)
	{
		for (int w = 0; w < 11; w++)
		{
			printf("%3d ", knapsack[i][w]);
		}
		printf("\n");
	}
}

int main()
{
	data d[7];
	printf("%d", load_file("items.txt", d));

	int knapsack[7][11] = { 0 };  // 0 ~ 6 , 0 ~ 10

	kalgo(d, knapsack);
	test_print(knapsack);
}