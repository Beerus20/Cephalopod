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
			printf("res : %d %d, begin : %d\n", res, tmp, begin);
			if (++nb >= 2)
			{
				if (res > 6)
					return (0);
				setNullTab(tab, i, j, begin, nb);
				if (fork() == 0)
				{
					printf("nb : %d\n", nb);
					tab[i][j] = res;
					printf("Child process \n");
					show("test", tab);
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

	nb = 0;
	if (depth <= 0)
		return (nb);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (!tab[i][j])
			{
				if (fork() == 0)
				{
					tab[i][j] = 1;
					showc("test", tab, i, j);
					begin = -1;
					while (++begin < 4)
					{
						printf("%d =======================================\n", begin);
						if (routine(tab, i, j, depth, begin, result) == 4)
							break ;

					}
					if (tab[i][j] == 1)
					{
						lock(result->sem_id);
						result->value = (result->value + hash(tab) % (1 << 30));
						unlock(result->sem_id);
						loop(tab, --depth, result);
					}
					exit(0);
				}
				wait(NULL);
			}
		}
	}
	return (nb);
}

int main()
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
	printf("depth : ");
	scanf("%d", &depth);
	// for (int i = 0; i < 3; i++) {
	// 	for (int j = 0; j < 3; j++) {
	// 		printf("tab [%d][%d] : ", i ,j);
	// 		scanf("%d", &value);
	// 		tab[i][j] = value;
	// 	}
	// }

	tab[0][0] = 0;
	tab[0][1] = 0;
	tab[0][2] = 0;

	tab[1][0] = 0;
	tab[1][1] = 0;
	tab[1][2] = 0;

	tab[2][0] = 0;
	tab[2][1] = 0;
	tab[2][2] = 0;

	// Write an action using printf(). DON'T FORGET THE TRAILING \n
	// To debug: fprintf(stderr, "Debug messages...\n");
	show("init", tab);
	sem.val = 1;
	if (semctl(result->sem_id, 0, SETVAL, sem) == -1)
		exit(1);
	value = loop(tab, depth, result);
	printf("%d result : %d\n", value, result->value);

	semctl(result->sem_id, 0, IPC_RMID, sem);
	shmdt(result);
	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}