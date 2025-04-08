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


typedef union u_sem {
	int				val;
	struct semid_ds	*buf;
	unsigned short	*array;
}	t_sem;

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

void	loop(t_grid *grid, int depth, int *content);
// int 	top(t_grid *grid, int i, int j);
// int 	right(t_grid *grid, int i, int j);
// int 	bottom(t_grid *grid, int i, int j);
// int 	left(t_grid *grid, int i, int j);
// void	show(char *label, t_grid *grid);
// void	showc(char *label, t_grid *grid, int a, int b);
int		hash(t_grid *grid);
void	show(char *label, int grid[9]);

#endif