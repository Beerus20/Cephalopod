#ifndef MAIN_H
# define MAIN_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <pthread.h>


typedef struct s_res
{
	pthread_mutex_t	mutex;
	int				value;
}	t_res;

typedef struct s_grid
{
	int		content[9];
	t_res	*result;
}	t_grid;

typedef struct	s_arg
{
	int			i;
	t_grid		*grid;
	int 		depth;
	int			*content;
	pthread_t	*pthread;
}	t_arg;

void	loop(t_grid *grid, int depth, int *content);
int		hash(int *content);
void	show(char *label, int grid[9]);
void	showc(char *label, int grid[9], int id);

#endif