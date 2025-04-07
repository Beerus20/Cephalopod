#include "main.h"

typedef int  (*t_func)(int [3][3], int, int);

int check(int tab[3][3], int i, int j, int index)
{
	static t_func func[] = {top, right, bottom, left};

	return (func[index](tab, i, j));
}

void	lock(int semid)
{
	semop(semid, &(struct sembuf){0, -1, SEM_UNDO}, 1);
}

void	unlock(int semid)
{
	semop(semid, &(struct sembuf){0, 1, SEM_UNDO}, 1);
}

void	setNullTab(int tab[3][3], int i, int j, int index, int nb)
{
	switch (index)
	{
		case 0:
			tab[i - 1][j] = 0;
			break;
		case 1:
			tab[i][j + 1] = 0;
			break;
		case 2:
			tab[i + 1][j] = 0;
			break;
		case 3:
			tab[i][j - 1] = 0;
			break;
		default:
			break;
	}
	if (nb == 2)
	{
		if (index == 0)
			setNullTab(tab, i, j, 3, 0);
		else
			setNullTab(tab, i, j, index - 1, 0);
	}
}

int	routine(int tab[3][3], int i, int j, int depth, int begin, t_res *result)
{
	int	index;
	int	tmp;
	int	nb;
	int	res;

	index = -1;
	nb = 0;
	tmp = 0;
	res = 0;
	while (++index < 4)
	{
		tmp = check(tab, i, j, begin);
		if (tmp)
		{
			res += tmp;
			if (++nb >= 2)
			{
				if (res > 6)
					return (0);
				setNullTab(tab, i, j, begin, nb);
				if (fork() == 0)
				{
					tab[i][j] = res;
					lock(result->sem_id);
					result->value += hash(tab) % (1 << 30);
					unlock(result->sem_id);
					loop(tab, --depth, result);
					exit(0);
				}
				wait(NULL);
			}
		}
		else
			return (nb);
		if (++begin == 4)
			begin = 0;
	}
	return (nb);
}

int	loop(int tab[3][3], int depth, t_res *result)
{
	int nb;
	int	begin;
	int	zero;

	nb = 0;
	zero = 0;
	if (depth <= 0)
	{
		lock(result->sem_id);
		result->value = (result->value + hash(tab)) % (1 << 30);
		unlock(result->sem_id);
		return (nb);
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (!tab[i][j])
			{
				zero = 1;
				if (fork() == 0)
				{
					tab[i][j] = 1;
					begin = -1;
					while (++begin < 4)
					{
						if (routine(tab, i, j, depth, begin, result) == 4)
							break ;

					}
					if (tab[i][j] == 1)
						loop(tab, --depth, result);
					exit(0);
				}
				wait(NULL);
			}
		}
	}
	if (!zero)
	{
		lock(result->sem_id);
		result->value = (result->value + hash(tab)) % (1 << 30);
		unlock(result->sem_id);
		return (nb);
	}
	return (nb);
}

void	test(t_res *res, int i)
{
	typedef struct s_test
	{
		int	depth;
		int	tab[3][3];
		int	result;
	}	t_test;
	
	t_test	checker[5];
	time_t	begin;

	checker[0] = (t_test){20, {{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, 322444322};
	checker[1] = (t_test){20, {{5, 0, 6}, {4, 5, 0}, {0, 6, 4}}, 951223336};
	checker[2] = (t_test){1, {{5, 5, 5}, {0, 0, 5}, {5, 5, 5}}, 36379286};
	checker[3] = (t_test){1, {{6, 1, 6}, {1, 0, 1}, {6, 1, 6}}, 264239762};
	checker[4] = (t_test){8, {{6, 0, 6}, {0, 0, 0}, {6, 1, 5}}, 76092874};
	// checker[5] = (t_test){24, (int [3][3]){{3, 0, 0}, {3, 6, 2}, {1, 0, 2}}, 661168294};
	// checker[6] = (t_test){36, (int [3][3]){{6, 0, 4}, {2, 0, 2}, {4, 0, 0}}, };
	// checker[7] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[8] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[9] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[10] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[11] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	// checker[12] = (t_test){20, (int [3][3]){{0, 6, 0}, {2, 2, 2}, {1, 6, 1}}, };
	res->value = 0;
	begin = time(NULL);
	loop(checker[i].tab, checker[i].depth, res);
	printf("final result : %d %d %s\e[0;0m\n", res->value, checker[i].result, res->value == checker[i].result ? "\e[0;92mOK" : "\e[0;91mKO");
	printf("timer : %ld\n", (unsigned long)difftime(time(NULL), begin));
}

int main(int argc, char **argv)
{
	int		depth;
	int		tab[3][3];
	int		value;
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
	// 		printf("tab [%d][%d] : ", i ,j);
	// 		scanf("%d", &value);
	// 		tab[i][j] = value;
	// 	}
	// }

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");
	sem.val = 1;
	if (semctl(result->sem_id, 0, SETVAL, sem) == -1)
		exit(1);
	test(result, argv[1][0] -'0');
	// value = loop(tab, depth, result);
	// printf("%d result : %d\n", value, result->value);

	semctl(result->sem_id, 0, IPC_RMID, sem);
	shmdt(result);
	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}