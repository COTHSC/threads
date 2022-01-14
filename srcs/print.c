/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:09:22 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:39:32 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_philo_status(t_philo *st_philo, uint64_t timestamp, int action)
{
	static char	*actions[5] = {"has taken a fork", "is eating", "is sleeping", \
		"is thinking", "died"};
	char		*temp;

	pthread_mutex_lock(st_philo->megaphone);
	if (check_death(st_philo) && action != 4)
	{
		if (action != 4)
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
