/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jescully <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 13:44:39 by jescully          #+#    #+#             */
/*   Updated: 2022/01/14 16:27:03 by jescully         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>

typedef struct s_philo {
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*megaphone;
	pthread_mutex_t	*update_time_of_death;
	pthread_mutex_t	*die_one_at_a_time;
	int				*funeral_time;
	uint64_t		time_of_death;
	int				eat;
	int				sleep;
	int				die;
	int				number_of_meals;
	int				who_am_i;
}	t_philo;

int			init_first_philo(t_philo *st_first_philo, t_philo *st_last_philo);
int			init_philos(t_philo *st_philo, int nb_of_philos, int *args);
void		meditate(int time_to_meditate);
uint64_t	get_time(void);
int			*parse_args(int argc, char **argv);
int			alloc_philos(t_philo **st_philo, pthread_t **thread, int nb_philos);
void		free_philos(pthread_t *threads, t_philo *st_philo, int nb_philos);
void		destroy_mutexes(t_philo *st_philo, int nb_of_philos);
int			grim_reaper(t_philo *st_philo, int nb_of_philos, pthread_t *thread);
int			check_death(t_philo *st_philo);
void		print_philo_status(t_philo *st_philo, uint64_t timestamp, int act);
int			eat(t_philo *st_philo);
void		*routine(void *s);
int			birth_philosophers(int nb_of_philos, int *args);
void		*routine(void *s);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_isdigit(int c);
void		ft_putstr_fd(char *s, int fd);
char		*ft_itoa(int n);
#endif
