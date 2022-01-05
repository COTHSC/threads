#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

pthread_mutex_t fork1;
pthread_mutex_t fork2;

typedef struct s_philo {
	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
	int 			who_am_i;
} t_philo;

uint64_t    get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

/**
 * meditate() - this function is a reimplementation of the usleep function
 * @time_to_meditate: time in microseconds the function should pause execution for
 * The name is mostly to stay in the theme of this project.
 */
void	meditate(int time_to_meditate)
{
    uint64_t        current_time;
	uint64_t		meditate_start;
	
    current_time = get_time();
	meditate_start = current_time.tv_usec;
	while (current_time.tv_usec < meditate_start + time_to_meditate)
    {
		gettimeofday(&current_time, NULL);
    }
}

int	bury_philosophers(int nb_of_philos, t_philo **st_philo, pthread_t *thread)
{
	int			i = 0;

	while (i < nb_of_philos)
	{
		if (pthread_join(thread[i], NULL))
		{
			printf("error during thread joining\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void*	routine(void *s)
{
	t_philo st_philo;

	st_philo = *(t_philo*)(s);
	if (st_philo.who_am_i == 0)
		meditate(10000);
	while (42)
	{
		pthread_mutex_lock(st_philo.r_fork);
		pthread_mutex_lock(st_philo.l_fork);

        /* write(1, "eating ", 7); */
        write(1, ft_itoa(st_philo.who_am_i), 1);
        /* write(1, "\n", 1); */


		meditate(10000);	
		pthread_mutex_unlock(st_philo.r_fork);
        write(1, ft_itoa(st_philo.who_am_i), 1);
		pthread_mutex_unlock(st_philo.l_fork);

        /* write(1, "done ", 5); */
        /* write(1, ft_itoa(st_philo.who_am_i), 1); */
        /* write(1, "\n", 1); */

		meditate(10000);	
	}

	return (NULL);
}

/**
 * birth_philosophers() - this function should create a thread per philosopher
 * @nb_of_philos: the number of philosophers to be created
 * @argv: contains the arguments needed to fill in the philosophers information
 *
 * 
 * Returns int: return a nonzero value if an error occured
 */
int	birth_philosophers(int nb_of_philos, char **argv)
{
	pthread_t	*thread;
	t_philo		**st_philo;
	int			i = 0;

	thread = malloc(sizeof(pthread_t) * nb_of_philos);
	st_philo = malloc(sizeof(t_philo) * nb_of_philos);
	while (i < nb_of_philos)
	{
		st_philo[i] = malloc(sizeof(t_philo));
		st_philo[i]->r_fork = &fork1;
		st_philo[i]->l_fork = &fork2;
		st_philo[i]->who_am_i = i;
		if (pthread_create(&thread[i], NULL, &routine, st_philo[i]))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		i++;
	}
	bury_philosophers(2, st_philo, thread);
	return (0);
}


int main(int argc, char **argv)
{
	pthread_t	thread[2];
	t_philo		*st_philo[2];
	int			i = 0;
	
	pthread_mutex_init(&fork1, NULL);
	pthread_mutex_init(&fork2, NULL);

	birth_philosophers(2, argv);
	pthread_mutex_destroy(&fork1);
	pthread_mutex_destroy(&fork2);

	return (0);
}
