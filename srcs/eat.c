/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:07:19 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:07:57 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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
