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
	int i;

	st_philo = (t_philo*)(s);
	while (42)
	{
		i = pthread_mutex_lock(st_philo->r_fork);
		/* printf("this is I for right fork: %d\n", i); */
		i = pthread_mutex_lock(st_philo->l_fork);
		/* printf("this is I for left fork: %d\n", i); */

        write(1, "eating ", 7);
        write(1, ft_itoa(st_philo->who_am_i), 1);
        write(1, "\n", 1);


		meditate(100);	
		i = pthread_mutex_unlock(st_philo->l_fork);
		i = pthread_mutex_unlock(st_philo->r_fork);

		meditate(100);	
	}

	return (NULL);
}

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
		if (i != 0)
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
		meditate(1000);
		if (pthread_create(&thread[i], NULL, &routine, &st_philo[i]))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		i++;
	}
	bury_philosophers(nb_of_philos, &st_philo, thread);
	i = 0;
	while ( i < nb_of_philos)
		pthread_mutex_destroy(st_philo[i++].l_fork);
	return (0);
}

void testfunc(pthread_mutex_t *r_fork)
{
	int i;
	i = pthread_mutex_lock(r_fork);
	printf("this is I for right fork: %d\n", i);
	i = pthread_mutex_unlock(r_fork);
	printf("this is I for right fork: %d\n", i);

}

int main(int argc, char **argv)
{

	pthread_mutex_t *r_fork;
	pthread_mutex_t l_fork;
	pthread_mutex_t *r_fork2;


	r_fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(r_fork, NULL);
	pthread_mutex_init(&l_fork, NULL);

	r_fork2 = r_fork;
	/* testfunc(r_fork2); */
	birth_philosophers(5, argv);
	return (0);
}
