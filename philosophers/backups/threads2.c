#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

typedef struct s_philo {
	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *pen;
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
	
	meditate_start = get_time();
    current_time = meditate_start;
	while (current_time < meditate_start + time_to_meditate)
    {
    	current_time = get_time();
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
	t_philo *st_philo;

	st_philo = (t_philo*)(s);
	while (42)
	{
		pthread_mutex_lock(st_philo->r_fork);
		pthread_mutex_lock(st_philo->l_fork);

        write(1, "eating ", 7);
        write(1, ft_itoa(st_philo->who_am_i), 1);
        write(1, "\n", 1);


		meditate(100);	
		/* pthread_mutex_unlock(st_philo->r_fork); */


		/* pthread_mutex_unlock(st_philo->l_fork); */

        /* write(1, "droped ", 7); */
        /* write(1, ft_itoa(st_philo->who_am_i), 1); */
        /* write(1, "\n", 1); */

		meditate(100);	
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
	t_philo		*st_philo;
	int			i = 0;

	thread = malloc(sizeof(pthread_t) * nb_of_philos);
	st_philo = malloc(sizeof(t_philo) * nb_of_philos);



	st_philo[nb_of_philos - 1].l_fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_philo[nb_of_philos - 1].l_fork, NULL);
	st_philo[0].r_fork = st_philo[nb_of_philos - 1].l_fork;



	while (i < nb_of_philos)
	{
		if (i)
			st_philo[i].r_fork = st_philo[i - 1].l_fork;
		if (i != nb_of_philos - 1)
		{
			st_philo[i].l_fork = malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(st_philo[i].l_fork, NULL);
		}
		st_philo[i].who_am_i = i;
		i++;
	}
	i = 0;

	while (i < nb_of_philos)
	{
		meditate(100);
		if (pthread_create(&thread[i], NULL, &routine, &st_philo[i]))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		pthread_mutex_destroy(st_philo[i].l_fork);
		i++;
	}
	bury_philosophers(nb_of_philos, &st_philo, thread);
	return (0);
}


int main(int argc, char **argv)
{
	birth_philosophers(1, argv);
	return (0);
}
