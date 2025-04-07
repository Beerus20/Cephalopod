#include "main.h"

int top(int tab[3][3], int i, int j)
{
	if (i - 1 >= 0 && tab[i - 1][j] && tab[i - 1][j] < 6)
		return (tab[i - 1][j]);
	return (0);
}

int right(int tab[3][3], int i, int j)
{
	if (j + 1 < 3 && tab[i][j + 1] && tab[i][j + 1] < 6)
		return (tab[i][j + 1]);
	return (0);
}

int bottom(int tab[3][3], int i, int j)
{
	if (i + 1 < 3 && tab[i + 1][j] && tab[i + 1][j] < 6)
		return (tab[i + 1][j]);
	return (0);
}

int left(int tab[3][3], int i, int j)
{
	if (j - 1 >= 0 && tab[i][j - 1] && tab[i][j - 1] < 6)
		return (tab[i][j - 1]);
	return (0);
}

void	show(char *label, int tab[3][3])
{
	printf("label : %s\n", label);
	for (int i = 0; i < 3; i++)
	{
		printf("[");
		for (int j = 0; j < 3; j++)
			printf(" %d ", tab[i][j]);
		printf("]\n");
	}
	printf("\n");
}

void	showc(char *label, int tab[3][3], int a, int b)
{
	printf("\e[0;32mlabel : %s\e[0;0m\n", label);
	for (int i = 0; i < 3; i++)
	{
		printf("[");
		for (int j = 0; j < 3; j++)
		{
			if (i == a && j == b)
				printf(" \e[0;31m%d\e[0;0m ", tab[i][j]);
			else
				printf(" %d ", tab[i][j]);
		}
		printf("]\n");
	}
	printf("\n");
}
