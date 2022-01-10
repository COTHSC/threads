#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int x = 0;
pthread_mutex_t mutx;
int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 27};

void*	routine(void *pp)
{
	int prime_to_print;
	int i = 0;
	int sum = 0;

	prime_to_print = *(int*)pp; 
	while (i < 5)
	{
		sum += primes[prime_to_print + i];
		i++;
	}
	*(int *)pp = sum;
	return (pp);
}

int main()
{
	pthread_t threads[10];
	int i = 0;
	pthread_mutex_init(&mutx, NULL);

	while (i < 2)
	{
		int *a = malloc(sizeof(int));
		*a = i * 5;
		if (pthread_create(&threads[i], NULL, &routine, a))
		{
			printf("error during thread cretion\n");
			return (1);
		}
		i++;
	}
	i = 0;
	int totsum = 0;
	while (i < 2)
	{
		int *r;
		if (pthread_join(threads[i], (void **)&r))
		{
			printf("error during thread join\n");
			return (2);
		}
		totsum += *r;
		free(r);
		i++;
	}
	printf("this is the total sum %d\n", totsum);
	pthread_mutex_destroy(&mutx);
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	printf("seconds : %ld\nmicro seconds : %ld",
    current_time.tv_sec, current_time.tv_usec);
	return (0);
}
