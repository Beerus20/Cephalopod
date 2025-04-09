#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

# pragma optimize("gt", on)

# define MODULO	1073741824
# define DEPTH	50

typedef struct s_res
{
	pthread_mutex_t	mutex;
	__uint32_t	value;
}	t_res;

typedef struct s_grid
{
	__uint32_t		content[9];
	t_res	*result;
}	t_grid;

typedef struct	s_arg
{
	__uint32_t	i;
	t_grid		*grid;
	__uint32_t 	depth;
	__uint32_t	*content;
}	t_arg;

__uint32_t	loop(t_grid *restrict grid, __uint32_t depth, __uint32_t *restrict content);
__uint32_t	hash(__uint32_t *content);

long long	**g_state;
__uint32_t	*g_result;
__uint32_t	s = 0;
__uint32_t	r = 0;
__uint32_t	result = 0;

__uint32_t	final_result(void)
{
	__uint32_t	i;
	__uint32_t	res;

	res = 0;
	i = -1;
	while (++i < r)
		res = (res + g_result[i]) % 1073741824;
	return (res);
}

inline __uint32_t hash(__uint32_t *content)
{
	if (!content)
		return (0);
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
__uint32_t	*update(__uint32_t *restrict dest, __uint32_t *restrict src, __uint32_t id, __uint32_t v, __uint32_t *restrict pos)
{
	__uint32_t	i;

	i = -1;
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

void	execution(t_arg *restrict arg, __uint32_t *restrict content, __uint32_t res, __uint32_t pos[4]);
inline void	execution(t_arg *restrict arg, __uint32_t *restrict content, __uint32_t res, __uint32_t pos[4])
{
	__uint32_t	dest[9];
	__uint32_t	vhash;
	__uint32_t	value;

	vhash = hash(update(dest, content, arg->i, res, pos));
	if (g_state[arg->depth][vhash] == 0)
		g_state[arg->depth][vhash] = loop(arg->grid, arg->depth - 1, dest);
	result = (result + g_state[arg->depth][vhash]) % MODULO;
}

void	action(t_arg *restrict arg, __uint32_t *restrict content, __uint32_t pos[4]);
inline void	action(t_arg *restrict arg, __uint32_t *restrict content, __uint32_t pos[4])
{
	__uint32_t	i;

	i = 0;
	if (pos[0] && pos[1] && pos[0] + pos[1] <= 6 && ++i)
		execution(arg, content, pos[0] + pos[1], (__uint32_t []){pos[0], pos[1], -1, -1});
	if (pos[0] && pos[2] && pos[0] + pos[2] <= 6 && ++i)
		execution(arg, content, pos[0] + pos[2], (__uint32_t []){pos[0], -1, pos[2], -1});
	if (pos[0] && pos[3] && pos[0] + pos[3] <= 6 && ++i)
		execution(arg, content, pos[0] + pos[3], (__uint32_t []){pos[0], -1, -1, pos[3]});
	if (pos[1] && pos[3] && pos[1] + pos[3] <= 6 && ++i)
		execution(arg, content, pos[1] + pos[3], (__uint32_t []){-1, pos[1], -1, pos[3]});
	if (pos[2] && pos[3] && pos[2] + pos[3] <= 6 && ++i)
		execution(arg, content, pos[2] + pos[3], (__uint32_t []){-1, -1, pos[2], pos[3]});
	if (pos[1] && pos[2] && pos[1] + pos[2] <= 6 && ++i)
		execution(arg, content, pos[1] + pos[2], (__uint32_t []){-1, pos[1], pos[2], -1});
	if (pos[0] && pos[1] && pos[2] && pos[0] + pos[1] + pos[2] <= 6 && ++i)
		execution(arg, content, pos[0] + pos[1] + pos[2], (__uint32_t []){pos[0], pos[1], pos[2], -1});
	if (pos[1] && pos[2] && pos[3] && pos[1] + pos[2] + pos[3] <= 6 && ++i)
		execution(arg, content, pos[1] + pos[2] + pos[3], (__uint32_t []){-1, pos[1], pos[2], pos[3]});
	if (pos[2] && pos[3] && pos[0] && pos[2] + pos[3] + pos[0]<= 6 && ++i)
		execution(arg, content, pos[2] + pos[3] + pos[0], (__uint32_t []){pos[0], -1, pos[2], pos[3]});
	if (pos[3] && pos[0] && pos[1] && pos[3] + pos[0] + pos[1] <= 6 && ++i)
		execution(arg, content, pos[3] + pos[0] + pos[1], (__uint32_t []){pos[0], pos[1], -1, pos[3]});
	if (pos[0] && pos[1] && pos[2] && pos[3] && pos[0] + pos[1] + pos[2] + pos[3] <= 6 && ++i)
		execution(arg, content, pos[0] + pos[1] + pos[2] + pos[3], (__uint32_t []){pos[0], pos[1], pos[2], pos[3]});
	if (!i)
		execution(arg, content, 1, (__uint32_t []){-1, -1, -1, -1});
}

void	*routine(void *arg)
{
	__uint32_t		i;
	__uint32_t		pos[4];
	__uint32_t		*content;

	i = ((t_arg *)arg)->i;
	content = ((t_arg *)arg)->content ;
	if (content[i])
		return (NULL);
	pos[0] = i > 2 ? content[i - 3] : 0;
	pos[1] = i != 0 && i != 3 && i != 6 ? content[i - 1] : 0;
	pos[2] = i != 2 && i != 5 && i != 8 ? content[i + 1] : 0;
	pos[3] = i < 6 ? content[i + 3] : 0;
	action(arg, content, pos);
	return (NULL);
}

__uint32_t	loop(t_grid *restrict grid, __uint32_t depth, __uint32_t *restrict content)
{
	__uint32_t	id;
	__uint32_t	zero;
	__uint32_t	*tab;

	if (content && depth <= 0)
		return (hash(content));
	id = 0;
	zero = 0;
	tab = content ? content : grid->content;
	for (__uint32_t i = 0; i < 9; i++)
	{
		if (!tab[i] && ++zero)
			routine(&(t_arg){i, grid, depth, tab});
	}
	if (!zero)
		return (hash(content));
	return (0);
}

void	test(t_res *res, __uint32_t i)
{
	typedef struct s_test
	{
		__uint32_t		depth;
		t_grid	grid;
		__uint32_t	result;
	}	t_test;

	t_test			checker[12];
	struct timeval	begin, end;

	checker[0] = (t_test){20, {{0, 6, 0, 2, 2, 2, 1, 6, 1}, res}, 322444322};
	checker[1] = (t_test){20, {{5, 0, 6, 4, 5, 0, 0, 6, 4}, res}, 951223336};
	checker[2] = (t_test){1, {{5, 5, 5, 0, 0, 5, 5, 5, 5}, res}, 36379286};
	checker[3] = (t_test){1, {{6, 1, 6, 1, 0, 1, 6, 1, 6}, res}, 264239762};
	checker[4] = (t_test){8, {{6, 0, 6, 0, 0, 0, 6, 1, 5}, res}, 76092874};
	checker[5] = (t_test){24, {{3, 0, 0, 3, 6, 2, 1, 0, 2}, res}, 661168294};
	checker[6] = (t_test){36, {{6, 0, 4, 2, 0, 2, 4, 0, 0}, res}, 0};
	checker[7] = (t_test){32, {{0, 0, 0, 0, 5, 4, 1, 0, 5}, res}, 0};
	checker[8] = (t_test){40, {{0, 0, 4, 0, 2, 4, 1, 3, 4}, res}, 0};
	checker[9] = (t_test){40, {{0, 5, 4, 0, 3, 0, 0, 3, 0}, res}, 0};
	checker[10] = (t_test){20, {{0, 5, 1, 0, 0, 0, 4, 0, 1}, res}, 0};
	checker[11] = (t_test){20, {{1, 0, 0, 3, 5, 2, 1, 0, 0}, res}, 0};
	res->value = 0;
	g_state = (long long **)calloc(DEPTH, sizeof(long long *));
	if (!g_state)
		exit(0);
	for (size_t i = 0; i < DEPTH; i++)
	{
		g_state[i] = (long long *)calloc(MODULO, sizeof(long long));
		if (!g_state[i])
			exit(0);
	}
	gettimeofday(&begin, NULL);
	printf("start\n");
	loop(&checker[i].grid, checker[i].depth, NULL);
	res->value = result;
	gettimeofday(&end, NULL);
	printf("final result : %d %d %s\e[0;0m\n", res->value , checker[i].result, res->value == checker[i].result ? "\e[0;92mOK" : "\e[0;91mKO");
	printf("timer : %ld ms\n", ((end.tv_sec - begin.tv_sec) * 1000L + (end.tv_usec - begin.tv_usec) / 1000l));
	for (size_t i = 0; i < DEPTH; i++)
		free(g_state[i]);
	free(g_state);
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