#include "main.h"

int hash(t_grid *grid)
{
	return (\
		((grid->content[0][0] * 10e7) + \
		(grid->content[0][1] * 10e6) + \
		(grid->content[0][2] * 10e5) + \
		(grid->content[1][0] * 10e4) + \
		(grid->content[1][1] * 10e3) + \
		(grid->content[1][2] * 10e2) + \
		(grid->content[2][0] * 10e1) + \
		(grid->content[2][1] * 10e0) + \
		(grid->content[2][2])));
}

int top(t_grid *grid, int i, int j)
{
	if (i - 1 >= 0)
		return (grid->content[i - 1][j]);
	return (0);
}

int right(t_grid *grid, int i, int j)
{
	if (j + 1 < 3)
		return (grid->content[i][j + 1]);
	return (0);
}

int bottom(t_grid *grid, int i, int j)
{
	if (i + 1 < 3)
		return (grid->content[i + 1][j]);
	return (0);
}

int left(t_grid *grid, int i, int j)
{
	if (j - 1 >= 0)
		return (grid->content[i][j - 1]);
	return (0);
}

void	show(char *label, t_grid *grid)
{
	printf("label : %s\n", label);
	for (int i = 0; i < 3; i++)
	{
		printf("[");
		for (int j = 0; j < 3; j++)
			printf(" %d ", grid->content[i][j]);
		printf("]\n");
	}
	printf("hash : %d\n", hash(grid) % (1 << 30));
}

void	showc(char *label, t_grid *grid, int a, int b)
{
	printf("\e[0;32mlabel : %s\e[0;0m\n", label);
	for (int i = 0; i < 3; i++)
	{
		printf("[");
		for (int j = 0; j < 3; j++)
		{
			if (i == a && j == b)
				printf(" \e[0;31m%d\e[0;0m ", grid->content[i][j]);
			else
				printf(" %d ", grid->content[i][j]);
		}
		printf("]\n");
	}
	printf("\n");
}
