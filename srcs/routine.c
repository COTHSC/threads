/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:14:24 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:15:06 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*routine(void *s)
{
	t_philo	*st_philo;

	st_philo = (t_philo *)(s);
	if (st_philo->who_am_i % 2 == 0)
		usleep(900);
	while (42)
	{
		print_philo_status(st_philo, get_time(), 3);
		if (eat(st_philo) != 0)
			return (NULL);
		print_philo_status(st_philo, get_time(), 2);
		meditate(st_philo->sleep);
	}
	return (NULL);
}
