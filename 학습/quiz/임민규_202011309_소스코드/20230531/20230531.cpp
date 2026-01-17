#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>



typedef struct _data
{
	int num;
	int w;
	int v;
}data;


void load_file(const char* txt, data d[])
{
	FILE* fp;
	fp = fopen(txt, "r");

	d[0] = { 0 };
	for (int i = 1; i <= 6; i++)
		fscanf(fp, "%d %d %d", &d[i].num, &d[i].w, &d[i].v);
	fclose(fp);
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
	load_file("items.txt", d);

	int knapsack[7][11] = { 0 } ;  // 0 ~ 6 , 0 ~ 10

	kalgo(d, knapsack);
	test_print(knapsack);
}