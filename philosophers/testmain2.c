#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

#define die 1000
#define eat 200
#define sleep 200

typedef struct s_philo {
	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *imma_eat;
	pthread_mutex_t *left_eat;
	pthread_mutex_t *right_eat;
	pthread_mutex_t *megaphone;
	pthread_mutex_t *update_time_of_death;
	pthread_mutex_t *die_one_at_a_time;
	int				*funeral_time;
	uint64_t		time_of_death;
	int 			who_am_i;
} t_philo;

uint64_t    get_time(void)
{
	static struct timeval	tv;
	static int 				begining_of_time;


	if (!begining_of_time)
	{
		gettimeofday(&tv, NULL);
		begining_of_time = (tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000);
	}
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000) - begining_of_time);
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
		usleep(100);
		current_time = get_time();
	}
}

int	check_death(t_philo *st_philo)
{
	pthread_mutex_lock(st_philo->die_one_at_a_time);
	if (*st_philo->funeral_time)
	{
		pthread_mutex_unlock(st_philo->die_one_at_a_time);
		return (1);
	}
	pthread_mutex_unlock(st_philo->die_one_at_a_time);
	return (0);
}

void    print_philo_status(t_philo *st_philo, uint64_t timestamp, int action)
{
	char *actions[5] = {"has taken a fork", "is eating", "is sleeping", "is thinking", "died"};

	pthread_mutex_lock(st_philo->megaphone);
	if (check_death(st_philo) && action != 4)
	{
		if(action != 4)
			pthread_mutex_unlock(st_philo->megaphone);
		return ;
	}
	ft_putstr_fd(ft_itoa(timestamp), 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd(ft_itoa(st_philo->who_am_i), 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd(actions[action], 1);
	ft_putstr_fd("\n", 1);
	pthread_mutex_unlock(st_philo->megaphone);
}

int	bury_philosophers(int nb_of_philos, pthread_t *thread)
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
	uint64_t	timestamp;

	st_philo = (t_philo*)(s);
	/* if (st_philo->who_am_i % 2 != 0) */
	/* 	meditate(eat); */
	/* if (st_philo->who_am_i % 2 == 0) */
	/* 	usleep(9000); */
	while (42)
	{
		print_philo_status(st_philo, get_time(), 3);
		int i = pthread_mutex_lock(st_philo->imma_eat);
		/* i = pthread_mutex_lock(st_philo->left_eat); */
		printf("i = %i by philo %i\n", i, st_philo->who_am_i);

		pthread_mutex_lock(st_philo->r_fork);
		/* print_philo_status(st_philo, get_time(), 0); */
		pthread_mutex_lock(st_philo->l_fork);
		/* print_philo_status(st_philo, get_time(), 0); */
		if (check_death(st_philo))
		{
			/* pthread_mutex_unlock(st_philo->r_fork); */
			/* pthread_mutex_unlock(st_philo->l_fork); */
			return NULL;
		}
		timestamp = get_time();
		pthread_mutex_lock(st_philo->update_time_of_death);
		st_philo->time_of_death = timestamp + die;
		pthread_mutex_unlock(st_philo->update_time_of_death);
		print_philo_status(st_philo, timestamp, 1);
		meditate(eat);
		pthread_mutex_unlock(st_philo->l_fork);
		pthread_mutex_unlock(st_philo->r_fork);

		/* pthread_mutex_unlock(st_philo->imma_eat); */
		print_philo_status(st_philo, get_time(), 2);
		meditate(sleep);
	}
	return (NULL);
}

int	grim_reaper(t_philo *st_philo, int nb_of_philos, pthread_t *thread)
{
	int i;
	uint64_t	timestamp;
	uint64_t	offset;

	while (42)
	{
		offset = get_time();
		i = 0;
		while (i < nb_of_philos)
		{
			timestamp = get_time();
			pthread_mutex_lock(st_philo[i].update_time_of_death);
			if (timestamp + (offset - timestamp) >= st_philo[i].time_of_death)
			{
				print_philo_status(&st_philo[i], timestamp, 4);
				pthread_mutex_unlock(st_philo[i].update_time_of_death);
				pthread_mutex_lock(st_philo[i].die_one_at_a_time);
				*st_philo[0].funeral_time = 1;
				pthread_mutex_unlock(st_philo[i].die_one_at_a_time);
				bury_philosophers(nb_of_philos, thread);
				return (0);
			}
			pthread_mutex_unlock(st_philo[i].update_time_of_death);
			i++;
		}
	}
	return (0);
}

int	birth_philosophers(int nb_of_philos, char **argv)
{
	pthread_t	*thread;
	t_philo		*st_philo;
	int			i = 0;

	(void)argv;
	thread = malloc(sizeof(pthread_t) * nb_of_philos + 1);
	st_philo = ft_calloc(sizeof(t_philo), nb_of_philos + 1);
	st_philo[nb_of_philos - 1].l_fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_philo[nb_of_philos - 1].l_fork, NULL);
	st_philo[0].r_fork = st_philo[nb_of_philos - 1].l_fork;
	st_philo[0].megaphone = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_philo[0].megaphone, NULL);
	st_philo[0].die_one_at_a_time = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_philo[0].die_one_at_a_time, NULL);
	st_philo[0].funeral_time = malloc(sizeof(int));
	*st_philo[0].funeral_time = 0;	

	pthread_mutex_t *tmp;
	tmp = malloc(sizeof(pthread_mutex_t));

	while (i < nb_of_philos)
	{
		/* if (i % 2 == 0) */
		/* { */
			st_philo[i].imma_eat = malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(st_philo[i].imma_eat, NULL);
		/* } */
		i++;
	}


	st_philo[1].right_eat = st_philo[0].imma_eat;
	st_philo[0].right_eat = st_philo[1].imma_eat;
	/* st_philo[1].imma_eat = st_philo[0].imma_eat; */
	/* st_philo[1].left_eat = st_philo[0].imma_eat; */

	i = 0;

	while (i < nb_of_philos)
	{

		st_philo[i].time_of_death = get_time() + die;
		if (i != 0)
		{
			st_philo[i].megaphone = st_philo[0].megaphone;
			st_philo[i].r_fork = st_philo[i - 1].l_fork;
			st_philo[i].funeral_time = st_philo[0].funeral_time;
			st_philo[i].die_one_at_a_time = st_philo[0].die_one_at_a_time;
		}
		if (i != nb_of_philos - 1)
		{
			st_philo[i].l_fork = malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(st_philo[i].l_fork, NULL);
		}
		if (i % 2 == 0)
		{
			tmp = st_philo[i].l_fork;
			st_philo[i].l_fork = st_philo[i].r_fork;
			st_philo[i].r_fork = tmp;
		}
		st_philo[i].update_time_of_death = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(st_philo[i].update_time_of_death, NULL);
		st_philo[i].who_am_i = i;
		i++;
	}
	i = 0;
	while (i < nb_of_philos)
	{
		if (pthread_create(&thread[i], NULL, &routine, &st_philo[i]))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		i++;
	}
	grim_reaper(st_philo, nb_of_philos, thread);
	i = 0;
	while (i < nb_of_philos)
		pthread_mutex_destroy(st_philo[i++].l_fork);
	pthread_mutex_destroy(st_philo[0].megaphone);
	return (0);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	birth_philosophers(2, argv);
	return (0);
}
