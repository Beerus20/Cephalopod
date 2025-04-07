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

void	setNullTab(int tab[3][3], int i, int j, int index, int nb)
{
	switch (index)
	{
		case 0:
			tab[i - 1][j] = 0;
			break;
		case 1:
			tab[i][j + 1] = 0;
			break;
		case 2:
			tab[i + 1][j] = 0;
			break;
		case 3:
			tab[i][j - 1] = 0;
			break;
		default:
			break;
	}
	if (nb == 2)
	{
		if (index == 0)
			setNullTab(tab, i, j, 3, 0);
		else
			setNullTab(tab, i, j, index - 1, 0);
	}
}

int	routine(int tab[3][3], int depth, int begin)
{
	int	i;
	int	tmp;
	int	nb;
	int	res;

	i = -1;
	nb = 0;
	tmp = 0;
	res = 0;
	(void)depth;
	while (++i < 4)
	{
		tmp = check(tab, 1, 1, begin);
		if (tmp)
		{
			res += tmp;
			printf("res : %d %d\n", res, tmp);
			nb++;
			if (nb >= 2 && res <= 6)
			{
				setNullTab(tab, 1, 1, begin, nb);
				if (fork() == 0)
				{
					tab[1][1] = res;
					printf("Child process \n");
					show("test", tab);
					return (0);
				}
				wait(NULL);
			}
		}
		else
			return (0);
		if (++begin == 4)
			begin = 0;
	}
	return (res);
}

int	loop(int tab[3][3], int depth)
{
	int res;

	(void)tab;
	(void)depth;
	res = 0;
	if (!routine(tab, depth, 0))
		return (0);
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
	tab[0][1] = 0;
	tab[0][2] = 0;

	tab[1][0] = 1;
	tab[1][1] = 0;
	tab[1][2] = 0;

	tab[2][0] = 0;
	tab[2][1] = 1;
	tab[2][2] = 0;

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");
	show("init", tab);
	value = loop(tab, depth);
	printf("%d\n", value);

	return 0;
}