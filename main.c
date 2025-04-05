#include "main.h"

typedef int  (*t_func)(int [3][3], int, int);

int check(int tab[3][3], int i, int j, int index)
{
	static t_func func[] = {top, right, bottom, left};

	return (func[index](tab, i, j));
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

void	setNullTab(int tab[3][3], int i, int j, int index)
{
	switch (index)
	{
		case 1:
			tab[i - 1][j] = 0;
			break;
		case 2:
			tab[i][j + 1] = 0;
			break;
		case 3:
			tab[i + 1][j] = 0;
			break;
		case 4:
			tab[i][j - 1] = 0;
			break;
		default:
			break;
	}
}

void	updateTab(int tab[3][3], int i, int j, int index)
{
	switch (index)
	{
		case 1:
			tab[i - 1][j] = 0;
			break;
		case 2:
		{
			tab[i][j + 1] = 0;
			break;
		}
		case 3:
			tab[i][j - 1] = 0;
			break;
		case 4:
			tab[i + 1][j] = 0;
			break;
		default:
			break;
	}
}

int	routine(int tab[3][3], int depth, int begin)
{
	int	i;
	int	tmp;
	int	nb;
	int	res;
	int	id;

	i = 0;
	nb = 0;
	tmp = 0;
	res = 0;
	(void)tab;
	(void)depth;
	while (i < 4)
	{
		tmp = check(tab, 1, 1, begin++);
		if (tmp)
		{
			res += tmp;
			printf("res : %d %d\n", res, tmp);
			nb++;
			if (nb >= 2)
			{
				setNullTab(tab, 1, 1, begin);
				setNullTab(tab, 1, 1, begin - 1);
				id = fork();
				if (id == 0)
				{
					tab[1][1] = res;
					printf("Child process \n");
					show("test", tab);
					return (0);
				}
				waitpid(id, NULL, 0);
			}
		}
		else
			res = 0;
		i++;
	}
	return (0);
}

int	loop(int tab[3][3], int depth)
{
	int res;

	(void)tab;
	(void)depth;
	res = 0;
	routine(tab, depth, 0);
	return (res);
}

int main()
{
	int depth;
	int tab[3][3];
	int value;

	printf("depth : ");
	scanf("%d", &depth);
	// for (int i = 0; i < 3; i++) {
	// 	for (int j = 0; j < 3; j++) {
	// 		printf("tab [%d][%d] : ", i ,j);
	// 		scanf("%d", &value);
	// 		tab[i][j] = value;
	// 	}
	// }

	tab[0][0] = 0;
	tab[0][1] = 6;
	tab[0][2] = 0;

	tab[1][0] = 1;
	tab[1][1] = 0;
	tab[1][2] = 1;

	tab[2][0] = 0;
	tab[2][1] = 1;
	tab[2][2] = 0;

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");

	value = loop(tab, depth);
	printf("%d\n", value);

	return 0;
}