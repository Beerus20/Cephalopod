#include "main.h"

typedef int  (*t_func)(t_grid *, int, int);
typedef struct	s_arg
{
	int			i;
	t_grid		*grid;
	int 		depth;
	pthread_t	*pthread;
}	t_arg;

// int check(t_grid *grid, int i, int j, int index)
// {
// 	static t_func func[] = {top, right, bottom, left};

// 	return (func[index](grid, i, j));
// }

// void	setNullGrid(t_grid *grid, int i, int j, int index, int nb)
// {
// 	while (nb >= 0)
// 	{
// 		if (index == 0)
// 			grid->content[i - 1][j] = 0;
// 		else if (index == 1)
// 			grid->content[i][j + 1] = 0;
// 		else if (index == 2)
// 			grid->content[i + 1][j] = 0;
// 		else
// 			grid->content[i][j - 1] = 0;
// 		if (nb--)
// 		{
// 			if (index == 0)
// 				index = 3;
// 			else
// 				index--;
// 		}
// 	}
// }

// int	stars_checking(t_grid *grid, int i, int j, int begin, int depth)
// {
// 	int	tmp;
// 	int	op;
// 	int	res;

// 	tmp = check(grid, i, j, begin);
// 	op = check(grid, i, j, begin + 2);
// 	res = op + tmp;
// 	if (tmp && op && res <= 6)
// 	{
// 		if (fork() == 0)
// 		{
// 			setNullGrid(grid, i, j, begin, 0);
// 			setNullGrid(grid, i, j, begin + 2, 0);
// 			grid->content[i][j] = res;
// 			loop(grid, --depth);
// 			exit(0);
// 		}
// 		return (1);
// 	}
// 	return (0);
// }

// int	circle_checking(t_grid *grid, int i, int j, int depth, int begin, int *nb, int value)
// {
// 	int	tmp;

// 	if (*nb == 3)
// 		return (*nb);
// 	tmp = check(grid, i, j, begin);
// 	if (!tmp)
// 		return (*nb >= 2 ? *nb : 0);
// 	value += tmp;
// 	if (value <= 6)
// 	{
// 		if (*nb >= 1 && fork() == 0)
// 		{
// 			setNullGrid(grid, i, j, begin, *nb);
// 			grid->content[i][j] = value;
// 			loop(grid, --depth);
// 			exit(0);
// 		}
// 		*nb += 1;
// 		if (++begin == 4)
// 			begin = 0;
// 		circle_checking(grid, i, j, depth, begin, nb, value);
// 	}
// 	return (*nb >= 2 ? *nb : 0);
// }

int	*update(int dest[9], int src[9], int id, int v, int pos[4])
{
	int	i;

	i = -1;
	printf("pos : %d %d %d %d %d\n", pos[0], pos[1], pos[2], pos[3], v);
	while (++i < 9)
	{
		if (id == i)
			dest[i] == v;
		else if (pos[0] != -1 && i + 3 == id)
			dest[i] = 0;
		else if (pos[1] != -1 && i + 1 == id)
			dest[i] = 0;
		else if (pos[2] != -1 && i - 1 == id)
			dest[i] = 0;
		else if (pos[3] != -1 && i - 3 == id)
			dest[i] = 0;
		else
			dest[i] == src[i];
		printf("dest[%d] : %d\n", i, dest[i]);
		printf("src[%d] : %d\n", i, src[i]);
	}
	show("update", dest);
	return (dest);
}

void	top_action(t_arg *arg, int pos[4])
{
	static int	tab[4][9] = {0};
	int	sum;

	sum = pos[0] + pos[1];
	printf("\nsum : %d\n", sum);
	if (pos[1] && sum <= 6)
	{
		loop(arg->grid, arg->depth - 1, update(tab[0], arg->grid->content, arg->i, sum, (int []){pos[0], pos[1], -1, -1}));
	}
}

void	action(t_arg *arg, int pos[4])
{
	if (pos[0])
		top_action(arg, pos);
	loop(arg->grid, arg->depth - 1, arg->grid->content);
}

int	routine_test(t_arg *arg)
{
	int		i = arg->i;
	t_grid	*grid = arg->grid;
	int		pos[4];

	printf("grid[%d] : %d\n", i, grid->content[i]);
	if (grid->content[i])
		return (pthread_detach(*arg->pthread), 0);
	pos[0] = i - 3 >= 0 ? grid->content[i - 3] : 0;
	pos[1] = i - 1 >= 0 ? grid->content[i - 1] : 0;
	pos[2] = i + 1 >= 0 ? grid->content[i + 1] : 0;
	pos[3] = i + 3 >= 0 ? grid->content[i + 3] : 0;
	action(arg, pos);
	return (0);
}

void	*lunch(void *arg)
{
	routine_test(arg);
	return (NULL);
}

void	loop(t_grid *grid, int depth, int *content)
{
	int			id;
	pthread_t	pthread[9];

	if (content && depth <= 0)
	{
		pthread_mutex_lock(&grid->result->mutex);
		grid->result->value = (grid->result->value + hash(grid));
		printf("result : %d\n", grid->result->value);
		pthread_mutex_unlock(&grid->result->mutex);
		return ;
	}
	id = 0;
	show("test", grid->content);
	for (int i = 0; i < 9; i++)
	{
		pthread_create(&pthread[id], NULL, &lunch, &(t_arg){i, grid, depth, &pthread[i]});
		pthread_join(pthread[id++], NULL);
	}
}

void	test(t_res *res, int i)
{
	typedef struct s_test
	{
		int		depth;
		t_grid	grid;
		int	result;
	}	t_test;

	t_test			checker[8];
	struct timeval	begin, end;

	checker[0] = (t_test){20, {{0, 6, 0, 2, 2, 2, 1, 6, 1}, res}, 322444322};
	checker[1] = (t_test){20, {{5, 0, 6, 4, 5, 0, 0, 6, 4}, res}, 951223336};
	checker[2] = (t_test){1, {{5, 5, 5, 0, 0, 5, 5, 5, 5}, res}, 36379286};
	checker[3] = (t_test){1, {{6, 1, 6, 1, 0, 1, 6, 1, 6}, res}, 264239762};
	checker[4] = (t_test){8, {{6, 0, 6, 0, 0, 0, 6, 1, 5}, res}, 76092874};
	checker[5] = (t_test){1, {{0, 1, 0, 1, 0, 1, 0, 1, 0}, res}, 0};
	checker[6] = (t_test){1, {{0, 0, 0, 0, 0, 0, 0, 0, 0}, res}, 0};
	checker[7] = (t_test){1, {{0, 1, 0, 1, 0, 0, 0, 0, 0}, res}, 0};
	// checker[5] = (t_test){24, (int [3][3]){{3, 0, 0}, {3, 6, 2}, {1, 0, 2}}, 661168294};
	// checker[7] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[8] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[9] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[10] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[11] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[12] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	res->value = 0;
	// show("initial grid", &checker[i].grid);
	gettimeofday(&begin, NULL);
	loop(&checker[i].grid, checker[i].depth, NULL);
	gettimeofday(&end, NULL);
	printf("final result : %d %d %s\e[0;0m\n", res->value, checker[i].result, res->value == checker[i].result ? "\e[0;92mOK" : "\e[0;91mKO");
	printf("timer : %ld ms\n", ((end.tv_sec - begin.tv_sec) * 1000L + (end.tv_usec - begin.tv_usec) / 1000l));
}

int main(int argc, char **argv)
{
	(void)argc;
	t_res	result;

	pthread_mutex_init(&result.mutex, NULL);
	result.value = 0;
	// printf("depth : ");
	// scanf("%d", &depth);
	// for (int i = 0; i < 3; i++) {
	// 	for (int j = 0; j < 3; j++) {
	// 		printf("grid [%d][%d] : ", i ,j);
	// 		scanf("%d", &value);
	// 		grid->content[i][j] = value;
	// 	}
	// }

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");
	test(&result, argv[1][0] -'0');
	// value = loop(grid, depth, result);
	// printf("%d result : %d\n", value, result->value);

	return 0;
}