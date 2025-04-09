#include "main.h"

inline int hash(int *content)
{
	return ((\
		(content[0] * 10e7) + \
		(content[1] * 10e6) + \
		(content[2] * 10e5) + \
		(content[3] * 10e4) + \
		(content[4] * 10e3) + \
		(content[5] * 10e2) + \
		(content[6] * 10e1) + \
		(content[7] * 10e0) + \
		(content[8])));
}

int	*update(int *restrict dest, int *restrict src, int id, int v, int *restrict pos)
{
	int	i;

	i = -1;
	show("SRC", src);
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
	return (dest);
}

void	action(t_arg *restrict arg, int *restrict content, int pos[4])
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

void	*routine(void *arg)
{
	int		i;
	int		pos[4];
	int		*content;

	i = ((t_arg *)arg)->i;
	content = ((t_arg *)arg)->content ;
	if (content[i])
		return (pthread_detach(*((t_arg *)arg)->pthread), NULL);
	printf("DEPTH : %d\n", ((t_arg *)arg)->depth);	
	pos[0] = i > 2 ? content[i - 3] : 0;
	pos[1] = i != 0 && i != 3 && i != 6 ? content[i - 1] : 0;
	pos[2] = i != 2 && i != 5 && i != 8 ? content[i + 1] : 0;
	pos[3] = i < 6 ? content[i + 3] : 0;
	action(arg, content, pos);
	return (NULL);
}

void	loop(t_grid *grid, int depth, int *content)
{
	int			id;
	int			zero;
	int			*tab;
	pthread_t	pthread[9];

	if (content && depth <= 0)
	{
		// pthread_mutex_lock(&grid->result->mutex);
		show("END", content);
		grid->result->value = (grid->result->value + hash(content)) % (1 << 30);
		// pthread_mutex_unlock(&grid->result->mutex);
		return ;
	}
	id = 0;
	zero = 0;
	tab = content ? content : grid->content;
	for (int i = 0; i < 9; i++)
	{
		if (!tab[i])
		{
			zero++;
			routine(&(t_arg){i, grid, depth, tab, &pthread[i]});
			// pthread_create(&pthread[id], NULL, &routine, &(t_arg){i, grid, depth, tab, &pthread[i]});
			// pthread_join(pthread[id++], NULL);
		}
	}
	if (!zero)
	{
		// pthread_mutex_lock(&grid->result->mutex);
		grid->result->value = (grid->result->value + hash(content)) % (1 << 30);
		// pthread_mutex_unlock(&grid->result->mutex);
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

	t_test			checker[12];
	struct timeval	begin, end;

	checker[0] = (t_test){20, {{0, 6, 0, 2, 2, 2, 1, 6, 1}, res}, 322444322};
	checker[1] = (t_test){20, {{5, 0, 6, 4, 5, 0, 0, 6, 4}, res}, 951223336};
	checker[2] = (t_test){1, {{5, 5, 5, 0, 0, 5, 5, 5, 5}, res}, 36379286};
	checker[3] = (t_test){1, {{6, 1, 6, 1, 0, 1, 6, 1, 6}, res}, 264239762};
	checker[4] = (t_test){8, {{6, 0, 6, 0, 0, 0, 6, 1, 5}, res}, 76092874};
	checker[5] = (t_test){24, {{3, 0, 0, 3, 6, 2, 1, 0, 2}, res}, 0};
	checker[6] = (t_test){36, {{6, 0, 4, 2, 0, 2, 4, 0, 0}, res}, 0};
	checker[7] = (t_test){32, {{0, 0, 0, 0, 5, 4, 1, 0, 5}, res}, 0};
	checker[8] = (t_test){40, {{0, 0, 4, 0, 2, 4, 1, 3, 4}, res}, 0};
	checker[9] = (t_test){40, {{0, 5, 4, 0, 3, 0, 0, 3, 0}, res}, 0};
	checker[10] = (t_test){20, {{0, 5, 1, 0, 0, 0, 4, 0, 1}, res}, 0};
	checker[11] = (t_test){20, {{1, 0, 0, 3, 5, 2, 1, 0, 0}, res}, 0};
	res->value = 0;
	show("initial grid", checker[i].grid.content);
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
	test(&result, argv[1][0] -'0');
	return 0;
}