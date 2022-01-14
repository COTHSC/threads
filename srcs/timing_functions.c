/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 13:48:17 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 14:35:12 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/**
 * get_time() - gets a timestamp in ms using the gettimeofday function
 * returns a timestamp equivalent to the time elapsed since the beggining of 
 * the simulation, using the static int "begining of time" which is set the 
 * first time the function is called
 * Returns uint64_t: the timestamp.
 */
uint64_t	get_time(void)
{
	static struct timeval	tv;
	static int				begining_of_time;
	uint64_t				ret;

	if (!begining_of_time)
	{
		gettimeofday(&tv, NULL);
		begining_of_time = (tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000);
	}
	gettimeofday(&tv, NULL);
	ret = (tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000) - begining_of_time;
	return (ret);
}

/**
 * meditate() - this function is a reimplementation of the usleep function
 * @time_to_meditate: time in microseconds the function should pause execution
 * for.
 * The name is mostly to stay in the theme of this project.
 */
void	meditate(int time_to_meditate)
{
	uint64_t		current_time;
	uint64_t		meditate_start;

	meditate_start = get_time();
	current_time = meditate_start;
	while (current_time < meditate_start + time_to_meditate)
	{
		usleep(100);
		current_time = get_time();
	}
}
