/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   birth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:12:06 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 16:10:43 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	start_threads(pthread_t *threads, t_philo *st_philo, int nb_of_philos)
{
	int	i;

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

int	birth_philosophers(int nb_of_philos, int *args)
{
	pthread_t	*threads;
	t_philo		*st_philo;

	if (alloc_philos(&st_philo, &threads, nb_of_philos))
		return (1);
	if (init_first_philo(&st_philo[0], &st_philo[nb_of_philos - 1]))
		return (1);
	if (init_philos(st_philo, nb_of_philos, args))
		return (1);
	st_philo[0].r_fork = st_philo[nb_of_philos - 1].l_fork;
	if (start_threads(threads, st_philo, nb_of_philos) == 1)
		return (1);
	grim_reaper(st_philo, nb_of_philos, threads);
	destroy_mutexes(st_philo, nb_of_philos);
	free_philos(threads, st_philo, nb_of_philos);
	return (0);
}
