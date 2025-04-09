#include "main.h"

typedef int  (*t_func)(t_grid *, int, int);
typedef struct	s_arg
{
	int			i;
	t_grid		*grid;
	int 		depth;
	int			*content;
	pthread_t	*pthread;
}	t_arg;

int	*update(int dest[9], int src[9], int id, int v, int pos[4])
{
	int	i;

	i = -1;
	showc("SRC", src, id);
	while (++i < 9)
	{
		dest[i] = id == i ? v : src[i];
		if (pos[0] != -1 && i + 3 == id)
			dest[i] = 0;
		if (pos[1] != -1 && i + 1 == id)
			dest[i] = 0;
		if (pos[2] != -1 && i - 1 == id)
			dest[i] = 0;
		if (pos[3] != -1 && i - 3 == id)
			dest[i] = 0;
	}
	show("update", dest);
	return (dest);
}

void	action(t_arg *arg, int *content, int pos[4])
{
	int	tab[12][9] = {0};
	int			i;

	i = 0;
	if (pos[0] && pos[1] && pos[0] + pos[1] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[0], content, arg->i, pos[0] + pos[1], (int []){pos[0], pos[1], -1, -1}));
	if (pos[0] && pos[2] && pos[0] + pos[2] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[1], content, arg->i, pos[0] + pos[2], (int []){pos[0], -1, pos[2], -1}));
	if (pos[0] && pos[3] && pos[0] + pos[3] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[2], content, arg->i, pos[0] + pos[3], (int []){pos[0], -1, -1, pos[3]}));

	if (pos[1] && pos[3] && pos[1] + pos[3] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[3], content, arg->i, pos[1] + pos[3], (int []){-1, pos[1], -1, pos[3]}));
	if (pos[2] && pos[3] && pos[2] + pos[3] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[4], content, arg->i, pos[2] + pos[3], (int []){-1, -1, pos[2], pos[3]}));
	if (pos[1] && pos[2] && pos[1] + pos[2] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[5], content, arg->i, pos[1] + pos[2], (int []){-1, pos[1], pos[2], -1}));

	if (pos[0] && pos[1] && pos[2] && pos[0] + pos[1] + pos[2] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[6], content, arg->i, pos[0] + pos[1] + pos[2], (int []){pos[0], pos[1], pos[2], -1}));
	if (pos[1] && pos[2] && pos[3] && pos[1] + pos[2] + pos[3] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[7], content, arg->i, pos[1] + pos[2] + pos[3], (int []){-1, pos[1], pos[2], pos[3]}));
	if (pos[2] && pos[3] && pos[0] && pos[2] + pos[3] + pos[0]<= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[8], content, arg->i, pos[2] + pos[3] + pos[0], (int []){pos[0], -1, pos[2], pos[3]}));
	if (pos[3] && pos[0] && pos[1] && pos[3] + pos[0] + pos[1] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[9], content, arg->i, pos[3] + pos[0] + pos[1], (int []){pos[0], pos[1], -1, pos[3]}));
	if (pos[0] && pos[1] && pos[2] && pos[3] && pos[0] + pos[1] + pos[2] + pos[3] <= 6 && ++i)
		loop(arg->grid, arg->depth - 1, update(tab[10], content, arg->i, pos[0] + pos[1] + pos[2] + pos[3], (int []){pos[0], pos[1], pos[2], pos[3]}));
	if (!i)
		loop(arg->grid, arg->depth - 1, update(tab[11], content, arg->i, 1, (int []){-1, -1, -1, -1}));
}

int	routine_test(t_arg *arg)
{
	int		i;
	int		pos[4];
	int		*content;

	i = arg->i;
	content = arg->content ? arg->content : arg->grid->content;
	if (content[i])
		return (pthread_detach(*arg->pthread), 0);
	pos[0] = i > 2 ? content[i - 3] : 0;
	pos[1] = i != 0 && i != 3 && i != 6 ? content[i - 1] : 0;
	pos[2] = i != 2 && i != 5 && i != 8 ? content[i + 1] : 0;
	pos[3] = i < 6 ? content[i + 3] : 0;
	action(arg, content, pos);
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
	int			zero;
	int			*tab;
	pthread_t	pthread[9];
	t_arg		arg;

	printf("DEPTH	: %d\n", depth);
	if (content && depth <= 1)
	{
		show("END", content);
		pthread_mutex_lock(&grid->result->mutex);
		grid->result->value = (grid->result->value + hash(content)) % (1 << 30);
		printf("result : %d\n", grid->result->value);
		pthread_mutex_unlock(&grid->result->mutex);
		return ;
	}
	id = 0;
	zero = 0;
	// content ? show("test", content) : 0;
	tab = content ? content : grid->content;
	for (int i = 0; i < 9; i++)
	{
		if (!tab[i])
		{
			zero++;
			arg = (t_arg){i, grid, depth, content, &pthread[i]};
			pthread_create(&pthread[id], NULL, &lunch, &arg);
			pthread_join(pthread[id++], NULL);
		}
	}
	if (content && !zero)
	{
		show("ZERO", content);
		pthread_mutex_lock(&grid->result->mutex);
		grid->result->value = (grid->result->value + hash(content)) % (1 << 30);
		printf("result : %d\n", grid->result->value);
		pthread_mutex_unlock(&grid->result->mutex);
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
	checker[5] = (t_test){1, {{0, 1, 0, 1, 0, 5, 0, 1, 0}, res}, 0};
	checker[6] = (t_test){1, {{0, 0, 0, 0, 0, 0, 0, 0, 0}, res}, 0};
	checker[7] = (t_test){2, {{0, 1, 0, 1, 0, 0, 0, 0, 0}, res}, 0};
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