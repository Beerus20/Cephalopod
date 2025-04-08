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
	switch (index)
	{
		case 0:
			grid->content[i - 1][j] = 0;
			break;
		case 1:
			grid->content[i][j + 1] = 0;
			break;
		case 2:
			grid->content[i + 1][j] = 0;
			break;
		case 3:
			grid->content[i][j - 1] = 0;
			break;
		default:
			break;
	}
	if (nb == 2)
	{
		if (index == 0)
			setNullGrid(grid, i, j, 3, 0);
		else
			setNullGrid(grid, i, j, index - 1, 0);
	}
}

void	routine_test(t_grid *grid, int i, int j, int depth, int begin, t_res *result)
{
	int	tmp;
	int	res;

	tmp = 0;
	res = 0;
	tmp = check(grid, i, j, begin);
	if (tmp)
	{
		res += tmp;
		setNullGrid(grid, i, j, begin, 0);
		if (!fork())
		{
			grid->content[i][j] = res;
			show("child process", grid);
			loop(grid, --depth, result);
			exit(0);
		}
		wait(NULL);
	}
}

void	loop(t_grid *grid, int depth, t_res *result)
{
	int	begin;
	int	zero;

	(void)zero;
	zero = 0;
	if (depth <= 0)
	{
		show("END", grid);
		lock(result->sem_id);
		printf("test before result : %d %d\n", result->value, hash(grid));
		result->value = (result->value + hash(grid)) % 1073741824;
		printf("test after result : %d\n\n", result->value);
		unlock(result->sem_id);
		exit(0);
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (!grid->content[i][j])
			{
				zero = 1;
				if (fork() == 0)
				{
					grid->content[i][j] = 1;
					begin = -1;
					showc("template", grid, i, j);
					while (++begin < 4)
						routine(grid, i, j, depth, begin, result);
					if (grid->content[i][j] == 1)
						loop(grid, --depth, result);
					exit(0);
				}
				wait(NULL);
			}
		}
	}
	if (!zero)
	{
		lock(result->sem_id);
		result->value = (result->value + hash(grid)) % 1073741824;
		unlock(result->sem_id);
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

	t_test			checker[6];
	struct timeval	begin, end;

	checker[0] = (t_test){20, {2, {{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}}, 322444322};
	checker[1] = (t_test){20, {3, {{5, 0, 6}, {4, 5, 0}, {0, 6, 4}}}, 951223336};
	checker[2] = (t_test){1, {2, {{5, 5, 5}, {0, 0, 5}, {5, 5, 5}}}, 36379286};
	checker[3] = (t_test){1, {1, {{6, 1, 6}, {1, 0, 1}, {6, 1, 6}}}, 264239762};
	checker[4] = (t_test){8, {4, {{6, 0, 6}, {0, 0, 0}, {6, 1, 5}}}, 76092874};
	checker[5] = (t_test){1, {5, {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}}}, 0};
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
	loop(&checker[i].grid, checker[i].depth, res);
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