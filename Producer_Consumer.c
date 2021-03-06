// Assignment-4 Producer-Consumer problem using Mutex && Semaphores
// Author : Syed Mujtaba 
// TE - 11
// Compile using gcc file.c -lpthread
// http://p.ip.fi/ElZ2
// Refer : https://docs.oracle.com/cd/E19455-01/806-5257/sync-31/index.html

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
void* producer(void *arg);
void* consumer(void *arg);

typedef struct 
{
	int in;
	int out;
	int *buffer;
	int co;
	int max;
	int n;
	sem_t full, empty;
	pthread_mutex_t mu;
}Buffer;

int main()
{
	Buffer P;
	P.in = 0;
	P.out = 0;
	P.co = 0;
	int res;
	srand(time(0));
	printf("Enter size of Buffer : ");
	scanf("%d", &P.n);
	assert(P.n > 0);

	printf("Enter the number of Iterations : ");
	scanf("%d", &P.max);
	assert(P.max > 0);

	P.buffer = (int *)malloc(P.n * sizeof(int));

	res = sem_init(&P.full, 0, 0); // Full Semaphore
	if(res != 0)
		exit(EXIT_FAILURE);

	res = sem_init(&P.empty, 0, P.n); // Empty Semaphore
	if(res != 0)
		exit(EXIT_FAILURE);

	res = pthread_mutex_init(&P.mu, NULL); // Mutex
	if(res != 0)
		exit(EXIT_FAILURE);

	pthread_t Producer, Consumer;  // Producer and Consumer Threads

	
	res = pthread_create(&Producer, NULL, producer, (void *)(&P));
	if(res != 0)
	{
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&Consumer, NULL, consumer, (void *)(&P));
	if(res != 0)
	{
		exit(EXIT_FAILURE);
	}
	res = pthread_join(Producer, NULL);
	if(res != 0)
	{
		printf("Thread join failed\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(Consumer, NULL);
	if(res != 0)
	{
		printf("Thread join failed\n");
		exit(EXIT_FAILURE);
	}
	sem_destroy(&P.full);
	sem_destroy(&P.empty);
	pthread_mutex_destroy(&P.mu);
	exit(EXIT_SUCCESS);
}
void* producer(void *arg)
{
	while(1)
	{
		sem_wait(&(((Buffer *)(arg)) -> empty));
		pthread_mutex_lock(&(((Buffer *)(arg)) -> mu)); // Entering Critical Section 

		int idx = ((Buffer *)(arg)) -> in;
		(((Buffer *)(arg)) -> buffer)[idx] = (rand() % 15);
		printf("%d is being inserted\n", (((Buffer *)(arg)) -> buffer)[idx]); 
		((Buffer *)(arg)) -> in = (((Buffer *)(arg))-> in + 1)%(((Buffer *)(arg))->n + 1);
		((Buffer *)(arg)) -> co = (((Buffer *)(arg))-> co + 1);
	
		sem_post(&(((Buffer *)(arg)) -> full));

		printf("%d - %d", ((Buffer *)(arg)) -> out, ((Buffer *)(arg)) -> in);
		printf("Buffer State : \n");
		for(int i = ((Buffer *)(arg)) -> out; i != ((Buffer *)(arg)) -> in; i = (i + 1) % (((Buffer *)(arg)) -> n + 1))
			printf("%d ", (((Buffer *)(arg)) -> buffer)[i]);
		printf("\n");

		pthread_mutex_unlock(&(((Buffer *)(arg)) -> mu)); // Leaving Critical Section
		
		if(((Buffer *)(arg)) -> co >= ((Buffer *)(arg)) -> max)
			break;
		if(rand() % 2) // Doing a random sleep to demonstrate Process Synchronization
		{
			float sleeper = rand()%20 / 10.00;
			sleep(sleeper);
		}
	}
	pthread_exit((void *)(NULL));
}
void* consumer(void *arg)
{	
	while(1)
	{
		sem_wait(&(((Buffer *)(arg)) -> full));
		pthread_mutex_lock(&(((Buffer *)(arg)) -> mu)); // Entering Critical Section

		int idx = ((Buffer *)(arg)) -> out;
		int ele = (((Buffer *)(arg)) -> buffer)[idx];
		printf("%d is being consumed\n", ele); 
		((Buffer *)(arg)) -> out = (((Buffer *)(arg))->out + 1)%(((Buffer *)(arg))->n + 1);
		((Buffer *)(arg)) -> co = (((Buffer *)(arg))->co + 1);

		sem_post(&(((Buffer *)(arg)) -> empty));

		printf("Buffer State : \n");
		for(int i = ((Buffer *)(arg)) -> out; i != ((Buffer *)(arg)) -> in; i = (i + 1) % (((Buffer *)(arg)) -> n + 1))
			printf("%d ", (((Buffer *)(arg)) -> buffer)[i]);
		printf("\n");

		pthread_mutex_unlock(&(((Buffer *)(arg)) -> mu)); // Leaving Critical Section

		if(((Buffer *)(arg)) -> co >= ((Buffer *)(arg)) -> max)
			break;
		if(rand() % 2) // Doing a random sleep to demonstrate Process Synchronization
		{
			float sleeper = ((rand() % 20))/10.00;
			sleep(sleeper);
		}
	}
	pthread_exit((void *)(NULL));
}

