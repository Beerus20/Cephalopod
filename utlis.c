#include "main.h"

// inline __uint32_t top(t_grid *grid, __uint32_t i, __uint32_t j)
// {
// 	return (i - 1 >= 0 ? grid->content[i - 1][j] : 0);
// }

// inline __uint32_t right(t_grid *grid, __uint32_t i, __uint32_t j)
// {
// 	return (j + 1 < 3 ? grid->content[i][j + 1] : 0);
// }

// inline __uint32_t bottom(t_grid *grid, __uint32_t i, __uint32_t j)
// {
// 	return (i + 1 < 3 ? grid->content[i + 1][j] : 0);
// }

// inline __uint32_t left(t_grid *grid, __uint32_t i, __uint32_t j)
// {
// 	return (j - 1 >= 0 ? grid->content[i][j - 1] : 0);
// }

void	show(char *label, __uint32_t grid[9])
{
	printf("label : %s", label);
	for (__uint32_t i = 0; i < 9; i++)
	{
		if (i % 3 == 0)
			printf("\n %d ", grid[i]);
		else
			printf(" %d ", grid[i]);
	}
	printf("\n");
}

void	showc(char *label, __uint32_t grid[9], __uint32_t id)
{
	printf("label : %s", label);
	for (__uint32_t i = 0; i < 9; i++)
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

// void	showc(char *label, t_grid *grid, __uint32_t a, __uint32_t b)
// {
// 	printf("\e[0;32mlabel : %s\e[0;0m\n", label);
// 	for (__uint32_t i = 0; i < 3; i++)
// 	{
// 		printf("[");
// 		for (__uint32_t j = 0; j < 3; j++)
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
