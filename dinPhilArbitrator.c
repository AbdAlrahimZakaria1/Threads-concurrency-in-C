/*dining philosopher, written in lecture*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/*
enum {
    hungry, eating, waiting,
} philstate;
struct phil{
    int id;
    enum pilstate state;
} phil;
*/
pthread_mutex_t chopsticks[5];

void *philosopher1(void *arg)
{
    long i = (long)arg;
    pthread_mutex_t *left = &chopsticks[i];
    pthread_mutex_t *right = &chopsticks[(i + 1) % 5];

    while (1)
    {
        printf("id %ld: state: %.10s \n", i, "thinking!");
        // sleep(2);/*think()*/
        printf("id %ld: state: %.10s \n", i, "hungry!");
        pthread_mutex_lock(left);
        // sleep(1);
        pthread_mutex_lock(right);

        // sleep(1); /*eat(left, right);*/

        printf("id %ld: state: %.10s \n", i, "eating");
        pthread_mutex_unlock(left);
        pthread_mutex_unlock(right);
        printf("id %ld: state: %.10s \n", i, "done!");
    }
}
void *philosopher2(void *arg)
{
    long i = (long)arg;
    pthread_mutex_t *left = &chopsticks[i];
    pthread_mutex_t *right = &chopsticks[(i + 1) % 5];

    while (1)
    {
        printf("id %ld: state: %.10s \n", i, "thinking!");
        // sleep(2);/*think()*/
        printf("id %ld: state: %.10s \n", i, "hungry!");
        int l = pthread_mutex_trylock(left);
        if (!l)
        {
            sleep(1);
            continue;
        }
        int r = pthread_mutex_trylock(right);
        if (!r)
        {
            pthread_mutex_unlock(left);
            sleep(1);
            continue;
        }
        // sleep(1); /*eat(left, right);*/

        printf("id %ld: state: %.10s \n", i, "eating");
        pthread_mutex_unlock(left);
        pthread_mutex_unlock(right);
        printf("id %ld: state: %.10s \n", i, "done!");
    }
}

/*
    Implemented using an arbitrator
    The arbitrator allows one philosopher to eat at a time 
    Example:
    philosopher (1) takes the lock (locks mutex), takes two chopsticks then eats and puts down the chopsticks and puts down the lock (unlock the mutex)
    philosopher (3) does the same and the loop goes on
*/
void *philosopher_with_arbitrator(void *arg)
{
    pthread_mutex_t waiter_mutex  = PTHREAD_MUTEX_INITIALIZER;
    long i = (long)arg;
    pthread_mutex_t *left = &chopsticks[i];
    pthread_mutex_t *right = &chopsticks[(i + 1) % 5];

    while (1)
    {
        printf("id %ld: state: %.10s \n", i, "thinking!");
        sleep(2);
        printf("id %ld: state: %.10s \n", i, "hungry!");
        pthread_mutex_lock (&waiter_mutex);
        pthread_mutex_lock (left);
        pthread_mutex_lock (right);
        sleep(1); /*eat(left, right);*/
        printf("id %ld: state: %.10s \n", i, "eating");
        pthread_mutex_unlock (left);
        pthread_mutex_unlock (right);
        pthread_mutex_unlock (&waiter_mutex);
        printf("id %ld: state: %.10s \n", i, "done!");
    }
}

void *djistrasalgorithm(void *arg)
{
    long i = (long)arg;
    int a = i, b = (i + 1) % 5;
    int f = (a < b) ? a : b;
    int s = (a < b) ? b : a;
    pthread_mutex_t *first = &chopsticks[f];
    pthread_mutex_t *second = &chopsticks[s];

    while (1)
    {
        printf("id %ld: state: %.10s \n", i, "thinking!");
        // sleep(2);/*think()*/
        printf("id %ld: state: %.10s \n", i, "hungry!");
        pthread_mutex_lock(first);
        // sleep(1);
        pthread_mutex_lock(second);

        // sleep(1); /*eat(left, right);*/

        printf("id %ld: state: %.10s \n", i, "eating");
        pthread_mutex_unlock(first);
        pthread_mutex_unlock(second);
        printf("id %ld: state: %.10s \n", i, "done!");
    }
}

int main()
{
    pthread_t threads[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_init(&chopsticks[i], NULL);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, philosopher_with_arbitrator, (void *)i);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
