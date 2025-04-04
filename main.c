#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int	loop(int tab[3][3], int depth);
int top(int tab[3][3], int i, int j, int res, int depth, int *nb);
int right(int tab[3][3], int i, int j, int res, int depth, int *nb);
int bottom(int tab[3][3], int i, int j, int res, int depth, int *nb);
int left(int tab[3][3], int i, int j, int res, int depth, int *nb);

int top(int tab[3][3], int i, int j, int res, int depth, int *nb)
{
	int tmp;

	tmp = 0;
	if (i - 1 >= 0 && tab[i - 1][j] && tab[i - 1][j] < 6)
	{
		if (*nb >= 4)
			return (res);
		*nb -= 1;
		tmp = right(tab, i, j, res + tab[i - 1][j], depth, nb);
		if (tmp && tmp < 6 && tmp + res < 6)
		{
			tab[i][j] = tmp + res;
			tab[i - 1][j] = 0;
			tab[i][j - 1] = 0;
			loop(tab, --depth);
			return (res + tmp);
		}
	}
	return (res);
}

int right(int tab[3][3], int i, int j, int res, int depth, int *nb)
{
	int tmp;

	tmp = 0;
	if (j + 1 >= 0 && tab[i][j + 1] && tab[i][j + 1] < 6)
	{
		if (*nb >= 4)
			return (res);
		*nb -= 1;
		tmp = bottom(tab, i, j, res + tab[i][j + 1], depth, nb);
		if (tmp && tmp < 6 && tmp + res < 6)
		{
			tab[i][j] = res + tmp;
			tab[i][j + 1] = 0;
			tab[i + 1][j] = 0;
			loop(tab, --depth);
			return (res + tmp);
		}
	}	
	return (0);
}

int bottom(int tab[3][3], int i, int j, int res, int depth, int *nb)
{
	int tmp;

	tmp = 0;
	if (i + 1 >= 0 && tab[i + 1][j] && tab[i + 1][j] < 6)
	{
		if (*nb >= 4)
			return (res);
		*nb -= 1;
		tmp = left(tab, i ,j, res + tab[i + 1][j], depth, nb);
		if (tmp && tmp < 6 && tmp + res < 6)
		{
			tab[i][j] = res + tmp;
			tab[i + 1][j] = 0;
			tab[i][j - 1] = 0;
			loop(tab, --depth);
			return (res + tmp);
		}
	}	
	return (0);
}

int left(int tab[3][3], int i, int j, int res, int depth, int *nb)
{
	int tmp;

	tmp = 0;
	if (j - 1 >= 0 && tab[i][j - 1] && tab[i][j - 1] < 6)
	{
		if (*nb >= 4)
			return (res);
		*nb -= 1;
		tmp = top(tab, i, j, res + tab[i][j - 1], depth, nb);
		if (tmp && tmp < 6 && tmp + res < 6)
		{
			tab[i][j] = res + tmp;
			tab[i][j] = 0;
			tab[i][j] = 0;
			loop(tab, --depth);
			return (res + tmp);
		}
	}	
	return (0);
}

typedef int  (*t_func)(int [3][3], int, int, int, int, int *);

int check(int tab[3][3], int i, int j, int depth, int *nb, int index)
{
	static t_func func[] = {top, right, bottom, left};

	return (func[index](tab, i, j, 0, depth, nb));
}

int hash(int tab[3][3])
{
	return (\
		((tab[0][0] * 10e8) + \
		(tab[0][1] * 10e7) + \
		(tab[0][2] * 10e6) + \
		(tab[1][0] * 10e5) + \
		(tab[1][1] * 10e4) + \
		(tab[1][2] * 10e3) + \
		(tab[2][0] * 10e2) + \
		(tab[2][1] * 10e1) + \
		(tab[2][2] * 10e0) * (1 << 30)));
}

int	loop(int tab[3][3], int depth)
{
	int index;
	int res;
	int nb;

	res = 0;
	if (!depth)
		return (res);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nb = 0;
			if (!tab[i][j])
			{
				index = -1;
				while (++index < 4)
				{
					if (check(tab, i, j, depth, &nb, index))
						res += hash(tab);
					if (nb >= 4)
						break ;
				}
			}
		}
	}
	return (res);
}

int main()
{
	int depth;
	int tab[3][3];
	int value;

	printf("depth : ");
	scanf("%d", &depth);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("tab [%d][%d] : ", i ,j);
			scanf("%d", &value);
			tab[i][j] = value;
		}
	}

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");

	value = loop(tab, depth);
	printf("%d\n", value);

	return 0;
}