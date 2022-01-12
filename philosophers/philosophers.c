#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

/* #define die 1500 */
/* #define eatvar 200 */
/* #define sleep 200 */

typedef struct s_philo {
	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *megaphone;
	pthread_mutex_t *update_time_of_death;
	pthread_mutex_t *die_one_at_a_time;
	int				*funeral_time;
	uint64_t		time_of_death;
	int 			eat;
	int 			sleep;
	int 			die;
	int 			number_of_meals;
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
	char	*actions[5] = {"has taken a fork", "is eating", "is sleeping", "is thinking", "died"};
	char	*temp;

	pthread_mutex_lock(st_philo->megaphone);
	if (check_death(st_philo) && action != 4)
	{
		if(action != 4)
			pthread_mutex_unlock(st_philo->megaphone);
		return ;
	}
	temp = ft_itoa(timestamp);
	ft_putstr_fd(temp, 1);
	free(temp);
	ft_putstr_fd(" ", 1);
	temp = ft_itoa(st_philo->who_am_i);
	ft_putstr_fd(temp, 1);
	free(temp);
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

void	take_forks(t_philo *st_philo)
{
		pthread_mutex_lock(st_philo->l_fork);
		pthread_mutex_lock(st_philo->r_fork);
		print_philo_status(st_philo, get_time(), 0);
}

void	drop_forks(t_philo *st_philo)
{
		pthread_mutex_unlock(st_philo->l_fork);
		pthread_mutex_unlock(st_philo->r_fork);
		print_philo_status(st_philo, get_time(), 0);
}

int	eat(t_philo *st_philo)
{
	uint64_t	timestamp;
	
	take_forks(st_philo);
	if (check_death(st_philo))
	{
		pthread_mutex_unlock(st_philo->r_fork);
		pthread_mutex_unlock(st_philo->l_fork);
		return (1);
	}
	timestamp = get_time();
	pthread_mutex_lock(st_philo->update_time_of_death);
	st_philo->time_of_death = timestamp + st_philo->die;
	pthread_mutex_unlock(st_philo->update_time_of_death);
	print_philo_status(st_philo, timestamp, 1);
	meditate(st_philo->eat);
	st_philo->number_of_meals--;
	drop_forks(st_philo);
	return (0);
}

void*	routine(void *s)
{
	t_philo *st_philo;

	st_philo = (t_philo*)(s);
	if (st_philo->who_am_i % 2 == 0)
		usleep(900);
	while (42)
	{
		print_philo_status(st_philo, get_time(), 3);
		if(eat(st_philo) != 0)
			return (NULL);
		print_philo_status(st_philo, get_time(), 2);
		meditate(st_philo->sleep);
	}
	return (NULL);
}

int	grim_reaper(t_philo *st_philo, int nb_of_philos, pthread_t *thread)
{
	int i;
	uint64_t	timestamp;

	while (42)
	{
		i = 0;
		while (i < nb_of_philos)
		{
			timestamp = get_time();
			pthread_mutex_lock(st_philo[i].update_time_of_death);
			if (st_philo[i].number_of_meals == 0)
			{
				pthread_mutex_unlock(st_philo[i].update_time_of_death);
				pthread_mutex_lock(st_philo[i].die_one_at_a_time);
				*st_philo[0].funeral_time = 1;
				pthread_mutex_unlock(st_philo[i].die_one_at_a_time);
				bury_philosophers(nb_of_philos, thread);
				return (0);
			}
			if (timestamp >= st_philo[i].time_of_death)
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
		usleep(100);
	}
	return (0);
}

void	init_first_philo(t_philo *st_first_philo, t_philo *st_last_philo)
{
	st_last_philo->l_fork = ft_calloc(sizeof(pthread_mutex_t), 1);
	pthread_mutex_init(st_last_philo->l_fork, NULL);
	st_first_philo->megaphone = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_first_philo[0].megaphone, NULL);
	st_first_philo->die_one_at_a_time = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(st_first_philo[0].die_one_at_a_time, NULL);
	st_first_philo->funeral_time = malloc(sizeof(int));
	*st_first_philo->funeral_time = 0;	
}

void	init_philos(t_philo *st_philo, int nb_of_philos, int *args)
{
	int i;

	i = -1;
	while (++i < nb_of_philos)
	{
		st_philo[i].time_of_death = get_time() + args[0];
		if (i != 0)
		{
			st_philo[i].megaphone = st_philo[0].megaphone;
			st_philo[i].r_fork = st_philo[i - 1].l_fork;
			st_philo[i].funeral_time = st_philo[0].funeral_time;
			st_philo[i].die_one_at_a_time = st_philo[0].die_one_at_a_time;
		}
		if (i != nb_of_philos - 1)
		{
			st_philo[i].l_fork = ft_calloc(sizeof(pthread_mutex_t), 1);
			pthread_mutex_init(st_philo[i].l_fork, NULL);
		}
		st_philo[i].update_time_of_death = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(st_philo[i].update_time_of_death, NULL);
		st_philo[i].who_am_i = i + 1;
		st_philo[i].die = args[0];
		st_philo[i].eat = args[1];
		st_philo[i].sleep = args[2];
		st_philo[i].number_of_meals = args[3];
	}
}

int	 start_threads(pthread_t *threads, t_philo *st_philo, int nb_of_philos)
{
	int i;

	i = 0;
	while (i < nb_of_philos)
	{
		if (pthread_create(&threads[i], NULL, &routine, &st_philo[i]))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void	destroy_mutexes(t_philo *st_philo, int nb_of_philos)
{
	int i;

	i = 0;
	while (i < nb_of_philos)
		pthread_mutex_destroy(st_philo[i++].l_fork);
	pthread_mutex_destroy(st_philo[0].megaphone);
}

int	alloc_philos_and_threads(t_philo **st_philo, pthread_t **threads, int nb_of_philos)
{
	*threads = ft_calloc(sizeof(pthread_t), nb_of_philos + 1);
	if (!*threads)
		return (1);
	*st_philo = ft_calloc(sizeof(t_philo), nb_of_philos + 1);
	if (!*st_philo)
	{
		free(*threads);	
		return (1);
	}
	return (0);
}

void	free_philo_souls(pthread_t *threads, t_philo *st_philo, int nb_of_philos)
{
	int i;

	(void)threads;
	free(st_philo->megaphone);
	free(st_philo->die_one_at_a_time);
	free(st_philo->funeral_time);
	i = -1;
	while (++i < nb_of_philos)
	{
		free(st_philo[i].l_fork);
		pthread_mutex_init(st_philo[i].l_fork, NULL);
		free(st_philo[i].update_time_of_death);
	}
	free(st_philo);
	free(threads);
}

int	birth_philosophers(int nb_of_philos, int *args)
{
	pthread_t	*threads;
	t_philo		*st_philo;

	if (alloc_philos_and_threads(&st_philo, &threads, nb_of_philos))
		return (1);
	init_first_philo(&st_philo[0], &st_philo[nb_of_philos - 1]);
	init_philos(st_philo, nb_of_philos, args);
	st_philo[0].r_fork = st_philo[nb_of_philos - 1].l_fork;
	if (start_threads(threads, st_philo, nb_of_philos) == 1)
		return (1);
	grim_reaper(st_philo, nb_of_philos, threads);
	destroy_mutexes(st_philo, nb_of_philos);
	free_philo_souls(threads, st_philo, nb_of_philos);
	return (0);
}

int	check_arg_count(int argc)
{
	if (argc < 5)
	{
		printf("Error: not enough arguments\n");
		return (1);
	}
	if (argc > 6)
	{
		printf("Error: too many arguments\n");
		return (1);
	}
	return (0);
}

int	check_args_value(char **argv)
{
	int i;
	int d;

	i = 0;
	while (argv[i])
	{
		d = 0;
		while (argv[i][d])
		{
			if (!ft_isdigit(argv[i][d]))
				return (1);
			d++;
		}
		i++;
	}
	return (0);
}

int	*parse_args(int argc, char **argv)
{
	int i;
	int	*parsed_args;

	if (check_arg_count(argc))
		return (NULL);
	if (check_args_value(&argv[1]))
		return (NULL);
	parsed_args = ft_calloc(sizeof(int), 7);
	parsed_args[4] = -1;
	i = 0;
	while (i < argc - 1)
	{
		parsed_args[i] = ft_atoi(argv[i + 1]);
		i++;
	}
	return (parsed_args);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int	*parsed_args;
	parsed_args = parse_args(argc, argv);
	if (parsed_args)
	{
		birth_philosophers(parsed_args[0], &parsed_args[1]);
		free(parsed_args);
	}
	return (0);
}
