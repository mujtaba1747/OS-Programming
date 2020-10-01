// Author : Syed Mujtaba
// TE-11
// Dining Philosophers using Semaphores
// Compile using -lpthread flag
// http://p.ip.fi/p8MF
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

int N, res;
sem_t *foo;
pthread_t *phil;
int *Index;
void* phil_fun(void *arg); 
int main()
{
	printf("Enter number of Philosophers : \n");
	scanf("%d", &N);
	assert(N > 1);
	foo = (sem_t *)malloc(N * sizeof(sem_t));
	phil = (pthread_t *)malloc(N * sizeof(pthread_t));
	Index = (int * )malloc(N * sizeof(int));
	for(int i = 0; i < N; i++)
		Index[i] = i;
	for(int i = 0; i < N; i++)
	{
		res = sem_init(&foo[i], 0, 1);
		if(res != 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < N; i++)
	{
		res = pthread_create(&phil[i], NULL, phil_fun, (void *)(&Index[i]));
		if(i == 0) // To make sure Philosopher[0] gets to eats first
			sleep(1);
		if(res != 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < N; i++)
	{
		res = pthread_join(phil[i], NULL);
		if(res != 0)
		{
			exit(EXIT_FAILURE);
		}
	}
}
void* phil_fun(void *arg)
{
	int i = *(int *)(arg);
	printf("Philosopher%d is Thinking\n", i);
	sleep(1);
	if(i == 0) // Deadlock Avoidance - Making i=0 the wierdo // Can randomize weirdness in philosophers also
	{
		sem_wait(&foo[i]);
		printf("Philosopher%d is Hungry\n", i);
		sem_wait(&foo[(i + 1)%N]);
	}
	else
	{
		sem_wait(&foo[(i + 1)%N]);
		printf("Philosopher%d is Hungry, he wants Chopstick%d \n", i, i);
		sem_wait(&foo[i]);
	}
	printf("Philosopher%d is Eating\n", i);
	sleep(1);
	printf("Philosopher%d is releasing Chopstick%d\n", i, (i+1)%N);
	sem_post(&foo[(i+1)%N]);
	sleep(1);
	printf("Philosopher%d is releasing Chopstick%d\n", i, i);
	sem_post(&foo[i]);
	printf("Philosopher%d is Done Eating\n", i);
	pthread_exit(NULL);
}
