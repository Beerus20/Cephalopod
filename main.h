#ifndef MAIN_H
# define MAIN_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <unistd.h>

int		loop(int tab[3][3], int depth);
int 	top(int tab[3][3], int i, int j);
int 	right(int tab[3][3], int i, int j);
int 	bottom(int tab[3][3], int i, int j);
int 	left(int tab[3][3], int i, int j);
void	show(char *label, int tab[3][3]);

#endif