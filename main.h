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
# include <time.h>

#define SHM_SIZE sizeof(int)
#define SHM_KEY 1234
#define SEM_KEY 5678

typedef union u_sem {
	int				val;
	struct semid_ds	*buf;
	unsigned short	*array;
}	t_sem;

typedef struct s_res
{
	int	sem_id;
	int	value;
}	t_res;

int		loop(int tab[3][3], int depth, t_res *result);
int 	top(int tab[3][3], int i, int j);
int 	right(int tab[3][3], int i, int j);
int 	bottom(int tab[3][3], int i, int j);
int 	left(int tab[3][3], int i, int j);
void	show(char *label, int tab[3][3]);
void	showc(char *label, int tab[3][3], int a, int b);
int		hash(int tab[3][3]);

#endif