/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:04:38 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:32:46 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_death(t_philo *st_philo)
{
	pthread_mutex_lock(st_philo->die_one_at_a_time);
	if (*st_philo->funeral_time || !st_philo->number_of_meals)
	{
		pthread_mutex_unlock(st_philo->die_one_at_a_time);
		return (1);
	}
	pthread_mutex_unlock(st_philo->die_one_at_a_time);
	return (0);
}

int	bury_philosophers(int nb_of_philos, pthread_t *thread)
{
	int	i;

	i = 0;
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

int	prepare_to_die(t_philo *st_philo, int nb_of_philos, pthread_t *th, int i)
{
	print_philo_status(&st_philo[i], get_time(), 4);
	pthread_mutex_unlock(st_philo[i].update_time_of_death);
	pthread_mutex_lock(st_philo[i].die_one_at_a_time);
	*st_philo[0].funeral_time = 1;
	pthread_mutex_unlock(st_philo[i].die_one_at_a_time);
	bury_philosophers(nb_of_philos, th);
	return (0);
}

int	grim_reaper(t_philo *st_philo, int nb_of_philos, pthread_t *thread)
{
	int			i;
	uint64_t	timestamp;

	while (42)
	{
		i = 0;
		while (i < nb_of_philos && nb_of_philos)
		{
			timestamp = get_time();
			pthread_mutex_lock(st_philo[i].update_time_of_death);
			if (st_philo[i].number_of_meals == 0)
			{
				bury_philosophers(nb_of_philos, thread);
				return (0);
			}
			else if (timestamp >= st_philo[i].time_of_death)
				return (prepare_to_die(st_philo, nb_of_philos, thread, i));
			pthread_mutex_unlock(st_philo[i].update_time_of_death);
			i++;
		}
		usleep(100);
	}
	return (0);
}
