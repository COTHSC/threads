#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

typedef struct s_philo {
	pthread_mutex_t r_fork;
	pthread_mutex_t l_fork;
	int 			who_am_i;
} t_philo;

uint64_t    get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
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
    	current_time = get_time();
    }
}

void	print_list(t_list *table)
{
	t_philo *temp_philo;
	while (table->next)
	{
		temp_philo = (t_philo *)table->content;
		printf("%s\n", ft_itoa(temp_philo->who_am_i));
		table = table->next;
	}
}

int main(int argc, char **argv)
{
	t_list *table;
	char	*str;
	t_philo *philos;

	str = "youhou";

	philos = malloc(sizeof(t_philo) * 5);

	philos[0].who_am_i = 0;
	table = ft_lstnew(&philos[0]);
	int i = 1;
	while (i < 5)
	{
		philos[i].who_am_i = i;
		ft_lstadd_back(&table, ft_lstnew(&philos[i]));
		i++;
	}
	/* ft_lstadd_back(&table, table); */
	print_list(table);
	return (0);
}
