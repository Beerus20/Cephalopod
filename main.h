#ifndef MAIN_H
# define MAIN_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>


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
	__uint32_t			i;
	t_grid		*grid;
	__uint32_t 		depth;
	__uint32_t			*content;
	pthread_t	*pthread;
}	t_arg;

__uint32_t	loop(t_grid *grid, __uint32_t depth, __uint32_t *content);
__uint32_t	hash(__uint32_t *content);
void		show(char *label, __uint32_t grid[9]);
void		showc(char *label, __uint32_t grid[9], __uint32_t id);

#endif