/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 13:43:30 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 16:11:57 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	init_first_philo(t_philo *st_first_philo, t_philo *st_last_philo)
{
	st_last_philo->l_fork = ft_calloc(sizeof(pthread_mutex_t), 1);
	if (!st_last_philo->l_fork)
		return (1);
	pthread_mutex_init(st_last_philo->l_fork, NULL);
	st_first_philo->megaphone = malloc(sizeof(pthread_mutex_t));
	if (!st_first_philo->megaphone)
		return (1);
	pthread_mutex_init(st_first_philo[0].megaphone, NULL);
	st_first_philo->die_one_at_a_time = malloc(sizeof(pthread_mutex_t));
	if (!st_first_philo->die_one_at_a_time)
		return (1);
	pthread_mutex_init(st_first_philo[0].die_one_at_a_time, NULL);
	st_first_philo->funeral_time = malloc(sizeof(int));
	if (!st_first_philo->funeral_time)
		return (1);
	*st_first_philo->funeral_time = 0;
	return (0);
}

int	alloc_mutexes(t_philo *st_philo, int nb_of_philos, int i)
{
	if (i != nb_of_philos - 1)
	{
		st_philo[i].l_fork = ft_calloc(sizeof(pthread_mutex_t), 1);
		if (!st_philo[i].l_fork)
			return (1);
		pthread_mutex_init(st_philo[i].l_fork, NULL);
	}
	st_philo[i].update_time_of_death = malloc(sizeof(pthread_mutex_t));
	if (!st_philo[i].update_time_of_death)
		return (1);
	pthread_mutex_init(st_philo[i].update_time_of_death, NULL);
	return (0);
}

int	init_philos(t_philo *st_philo, int nb_of_philos, int *args)
{
	int	i;

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
		if (alloc_mutexes(st_philo, nb_of_philos, i))
			return (1);
		st_philo[i].who_am_i = i + 1;
		st_philo[i].die = args[0];
		st_philo[i].eat = args[1];
		st_philo[i].sleep = args[2];
		st_philo[i].number_of_meals = args[3];
	}
	return (0);
}
