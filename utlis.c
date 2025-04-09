#include "main.h"

// inline int top(t_grid *grid, int i, int j)
// {
// 	return (i - 1 >= 0 ? grid->content[i - 1][j] : 0);
// }

// inline int right(t_grid *grid, int i, int j)
// {
// 	return (j + 1 < 3 ? grid->content[i][j + 1] : 0);
// }

// inline int bottom(t_grid *grid, int i, int j)
// {
// 	return (i + 1 < 3 ? grid->content[i + 1][j] : 0);
// }

// inline int left(t_grid *grid, int i, int j)
// {
// 	return (j - 1 >= 0 ? grid->content[i][j - 1] : 0);
// }

void	show(char *label, int grid[9])
{
	printf("label : %s", label);
	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0)
			printf("\n %d ", grid[i]);
		else
			printf(" %d ", grid[i]);
	}
	printf("\n");
}

void	showc(char *label, int grid[9], int id)
{
	printf("label : %s", label);
	for (int i = 0; i < 9; i++)
	{
		if (i == id)
			printf("\e[0;32m");
		if (i % 3 == 0)
			printf("\n %d ", grid[i]);
		else
			printf(" %d ", grid[i]);
		if (i == id)
			printf("\e[0;0m");
	}
	printf("\n");
}

// void	showc(char *label, t_grid *grid, int a, int b)
// {
// 	printf("\e[0;32mlabel : %s\e[0;0m\n", label);
// 	for (int i = 0; i < 3; i++)
// 	{
// 		printf("[");
// 		for (int j = 0; j < 3; j++)
// 		{
// 			if (i == a && j == b)
// 				printf(" \e[0;31m%d\e[0;0m ", grid->content[i][j]);
// 			else
// 				printf(" %d ", grid->content[i][j]);
// 		}
// 		printf("]\n");
// 	}
// 	printf("\n");
// }
