/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:00:37 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:54:25 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	alloc_philos(t_philo **st_philo, pthread_t **threads, int nb_philos)
{
	*threads = ft_calloc(sizeof(pthread_t), nb_philos + 1);
	if (!*threads)
		return (1);
	*st_philo = ft_calloc(sizeof(t_philo), nb_philos + 1);
	if (!*st_philo)
	{
		free(*threads);
		return (1);
	}
	return (0);
}

void	free_philos(pthread_t *threads, t_philo *st_philo, int nb_philos)
{
	int	i;

	(void)threads;
	free(st_philo->megaphone);
	free(st_philo->die_one_at_a_time);
	free(st_philo->funeral_time);
	i = -1;
	while (++i < nb_philos)
	{
		free(st_philo[i].l_fork);
		pthread_mutex_init(st_philo[i].l_fork, NULL);
		free(st_philo[i].update_time_of_death);
	}
	free(st_philo);
	free(threads);
}

void	destroy_mutexes(t_philo *st_philo, int nb_of_philos)
{
	int	i;

	i = 0;
	while (i < nb_of_philos)
		pthread_mutex_destroy(st_philo[i++].l_fork);
	pthread_mutex_destroy(st_philo[0].megaphone);
}
