#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_c = PTHREAD_MUTEX_INITIALIZER;
int run = 1;

void *thread_a(void *arg)
{
    int local_run = 1;
    while (local_run) {
        pthread_mutex_lock(&mutex_a);
        local_run = run;
        pthread_mutex_lock(&mutex_b);
        pthread_mutex_unlock(&mutex_a);
        pthread_mutex_lock(&mutex_c);
        pthread_mutex_unlock(&mutex_c);
        pthread_mutex_unlock(&mutex_b);
    }
    return NULL;
}

void *thread_b(void *arg)
{
    int local_run = 1;
    while (local_run) {
        pthread_mutex_lock(&mutex_c);
        pthread_mutex_lock(&mutex_a);
        local_run = run;
        pthread_mutex_unlock(&mutex_a);
        pthread_mutex_unlock(&mutex_c);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t a, b;
    pthread_create(&a, NULL, thread_a, NULL);
    pthread_create(&b, NULL, thread_b, NULL);
    usleep(1000*1000);
    pthread_mutex_lock(&mutex_a);
    run = 0;
    pthread_mutex_unlock(&mutex_a);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    return 0;
}
