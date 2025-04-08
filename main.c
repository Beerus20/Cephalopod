#include "main.h"

typedef int  (*t_func)(t_grid *, int, int);

int check(t_grid *grid, int i, int j, int index)
{
	static t_func func[] = {top, right, bottom, left};

	return (func[index](grid, i, j));
}

void	lock(int semid)
{
	semop(semid, &(struct sembuf){0, -1, SEM_UNDO}, 1);
}

void	unlock(int semid)
{
	semop(semid, &(struct sembuf){0, 1, SEM_UNDO}, 1);
}

void	setNullGrid(t_grid *grid, int i, int j, int index, int nb)
{
	while (nb >= 0)
	{
		if (index == 0)
			grid->content[i - 1][j] = 0;
		else if (index == 1)
			grid->content[i][j + 1] = 0;
		else if (index == 2)
			grid->content[i + 1][j] = 0;
		else
			grid->content[i][j - 1] = 0;
		if (nb--)
		{
			if (index == 0)
				index = 3;
			else
				index--;
		}
	}
}

int	stars_checking(t_grid *grid, int i, int j, int begin, int depth)
{
	int	tmp;
	int	op;
	int	res;

	tmp = check(grid, i, j, begin);
	op = check(grid, i, j, begin + 2);
	res = op + tmp;
	if (tmp && op && res <= 6)
	{
		if (fork() == 0)
		{
			setNullGrid(grid, i, j, begin, 0);
			setNullGrid(grid, i, j, begin + 2, 0);
			grid->content[i][j] = res;
			loop(grid, --depth);
			exit(0);
		}
		return (1);
	}
	return (0);
}

int	circle_checking(t_grid *grid, int i, int j, int depth, int begin, int *nb, int value)
{
	int	tmp;

	if (*nb == 3)
		return (*nb);
	tmp = check(grid, i, j, begin);
	if (!tmp)
		return (*nb >= 2 ? *nb : 0);
	value += tmp;
	if (value <= 6)
	{
		if (*nb >= 1 && fork() == 0)
		{
			setNullGrid(grid, i, j, begin, *nb);
			grid->content[i][j] = value;
			loop(grid, --depth);
			exit(0);
		}
		*nb += 1;
		if (++begin == 4)
			begin = 0;
		circle_checking(grid, i, j, depth, begin, nb, value);
	}
	return (*nb >= 2 ? *nb : 0);
}

int	routine_test(t_grid *grid, int i, int j, int depth)
{
	int	tmp;

	tmp = 0;
	tmp += circle_checking(grid, i, j, depth, 0, &(int){0}, 0);
	tmp += circle_checking(grid, i, j, depth, 1, &(int){0}, 0);
	tmp += circle_checking(grid, i, j, depth, 2, &(int){0}, 0);
	tmp += circle_checking(grid, i, j, depth, 3, &(int){0}, 0);
	if (tmp == 12 && fork() == 0)
	{
		grid->content[i][j] = grid->content[i + 1][j] + grid->content[i - 1][j] + grid->content[i][j + 1] + grid->content[i][j - 1];
		setNullGrid(grid, i, j, 0, 4);
		loop(grid, --depth);
		exit(0);
	}
	tmp += stars_checking(grid, i, j, 0, depth);
	tmp += stars_checking(grid, i, j, 1, depth);
	return (wait(NULL), tmp);
}

int	lunch(t_grid *grid, int i, int j, int depth)
{
	int	id;

	id = fork();
	if (!id)
	{
		!routine_test(grid, i, j, depth) ? grid->content[i][j] = 1 : 0;
		grid->content[i][j] ? loop(grid, --depth) : 0;
		exit(0);
	}
	return (waitpid(id, NULL, 0), 1);
}

void	loop(t_grid *grid, int depth)
{
	int	zero;

	if (depth <= 0)
	{
		lock(grid->result->sem_id);
		grid->result->value = (grid->result->value + hash(grid)) % 1073741824;
		unlock(grid->result->sem_id);
		exit(0);
	}
	zero = 0;
	zero += !grid->content[0][0] ? lunch(grid, 0, 0, depth) : 0;
	zero += !grid->content[0][1] ? lunch(grid, 0, 1, depth) : 0;
	zero += !grid->content[0][2] ? lunch(grid, 0, 2, depth) : 0;
	zero += !grid->content[1][0] ? lunch(grid, 1, 0, depth) : 0;
	zero += !grid->content[1][1] ? lunch(grid, 1, 1, depth) : 0;
	zero += !grid->content[1][2] ? lunch(grid, 1, 2, depth) : 0;
	zero += !grid->content[2][0] ? lunch(grid, 2, 0, depth) : 0;
	zero += !grid->content[2][1] ? lunch(grid, 2, 1, depth) : 0;
	zero += !grid->content[2][2] ? lunch(grid, 2, 2, depth) : 0;
	if (!zero)
	{
		lock(grid->result->sem_id);
		grid->result->value = (grid->result->value + hash(grid)) % 1073741824;
		unlock(grid->result->sem_id);
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

	checker[0] = (t_test){20, {2, {{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, res}, 322444322};
	checker[1] = (t_test){20, {3, {{5, 0, 6}, {4, 5, 0}, {0, 6, 4}}, res}, 951223336};
	checker[2] = (t_test){1, {2, {{5, 5, 5}, {0, 0, 5}, {5, 5, 5}}, res}, 36379286};
	checker[3] = (t_test){1, {1, {{6, 1, 6}, {1, 0, 1}, {6, 1, 6}}, res}, 264239762};
	checker[4] = (t_test){8, {1, {{6, 0, 6}, {0, 0, 0}, {6, 1, 5}}, res}, 76092874};
	checker[5] = (t_test){1, {5, {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}}, res}, 0};
	checker[6] = (t_test){1, {9, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, res}, 0};
	checker[7] = (t_test){1, {9, {{0, 1, 0}, {1, 0, 0}, {0, 0, 0}}, res}, 0};
	// checker[5] = (t_test){24, (int [3][3]){{3, 0, 0}, {3, 6, 2}, {1, 0, 2}}, 661168294};
	// checker[7] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[8] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[9] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[10] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[11] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[12] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	res->value = 0;
	show("initial grid", &checker[i].grid);
	gettimeofday(&begin, NULL);
	loop(&checker[i].grid, checker[i].depth);
	gettimeofday(&end, NULL);
	printf("final result : %d %d %s\e[0;0m\n", res->value, checker[i].result, res->value == checker[i].result ? "\e[0;92mOK" : "\e[0;91mKO");
	printf("timer : %ld ms\n", ((end.tv_sec - begin.tv_sec) * 1000L + (end.tv_usec - begin.tv_usec) / 1000l));
}

int main(int argc, char **argv)
{
	(void)argc;
	int		shm_id;
	t_res	*result;
	t_sem	sem;

	shm_id = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
	if (shm_id == -1)
		exit(1);
	result = (t_res *)shmat(shm_id, NULL, 0);
	if (result == (void *)-1)
		exit(1);
	result->value = 0;
	result->sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
	if (result->sem_id == -1)
		exit(1);
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
	sem.val = 1;
	if (semctl(result->sem_id, 0, SETVAL, sem) == -1)
		exit(1);
	test(result, argv[1][0] -'0');
	// value = loop(grid, depth, result);
	// printf("%d result : %d\n", value, result->value);

	semctl(result->sem_id, 0, IPC_RMID, sem);
	shmdt(result);
	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}