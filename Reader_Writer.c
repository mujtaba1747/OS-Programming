// Assignment-5 Reader Writer's problem using Semaphore && Mutex
// Author : Syed Mujtaba 
// TE - 11
// http://p.ip.fi/zwLo
// Compile using gcc file.c -lpthread

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct 
{
	sem_t wrt;
	pthread_mutex_t mu;
	int readcount;
	int iter;
	int readers;
	int writers;
}pass;

typedef struct 
{
	int id;
}Index;

void* arg;
void* reader_thr(void* ar);
void* writer_thr(void* ar);

int main()
{
	srand(time(0));
	pass P;
	int res;
	pthread_t *R, *W;
	P.readcount = 0;
	res = pthread_mutex_init(&P.mu, NULL);
	if(res != 0)
		exit(EXIT_FAILURE);
	res = sem_init(&P.wrt, 0, 1);
	if(res != 0)
		exit(EXIT_FAILURE);
	printf("Enter No. of Readers : ");
	scanf("%d", &P.readers);
	assert(P.readers > 0);
	printf("Enter No. of Writers : ");
	scanf("%d", &P.writers);
	assert(P.writers > 0);
	R = (pthread_t *)malloc(P.readers * sizeof(pthread_t));
	W = (pthread_t *)malloc(P.writers * sizeof(pthread_t));
	
	int c1 = P.readers;
	int c2 = P.writers;
	arg = &P;
	Index iw[c2+1];
	Index ir[c1+1];
	for(int i = 0; i <= c2; i++)
		iw[i].id = i;
	for(int i = 0; i <= c1; i++)
		ir[i].id = i;
	while(c1 > 0 || c2 > 0)
	{
		if(rand() % 2)
		{
			if(c1 > 0)
			{
				c1--;
				res = pthread_create(&R[c1], NULL, reader_thr, (void *)(&ir[c1]));
				if(res != 0)
					exit(EXIT_FAILURE);
			}
		}
		else
		{
			if(c2 > 0)
			{
				c2--;
				res = pthread_create(&W[c2], NULL, writer_thr, (void *)(&iw[c2]));
				if(res != 0)
					exit(EXIT_FAILURE);
			}
		}
	}
	for(int i = 0; i < P.readers; i++)
	{
		res = pthread_join(R[i], NULL);
		if(res != 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < P.writers; i++)
	{
		res = pthread_join(W[i], NULL);
		if(res != 0)
		{
			exit(EXIT_FAILURE);
		}
	}
}
void* reader_thr(void* ar)
{
	if(rand() % 2) // Doing a random sleep to demonstrate Process Synchronization
	{
		float sleeper = ((float)(rand() % 20)) / 10.00;
		sleep(sleeper);
	}
	pthread_mutex_lock(&(((pass *)(arg))->mu));
	((pass *)(arg))->readcount++;
	if(((pass *)(arg))->readcount == 1)
	{
		sem_wait(&(((pass *)(arg))->wrt)); 
	}
	pthread_mutex_unlock(&(((pass *)(arg))->mu));

	// Reading section
	FILE *fp;
	fp = fopen("data.txt", "r");	
	char ch;
	if(fp == NULL)
	{
		printf("RRR\n");
		exit(EXIT_FAILURE);
	}
	printf("Reader Count = %d\n", ((pass *)(arg))->readcount);
	printf("Reader%d is reading ...\n", ((Index *)(ar))->id);
	while((ch = getc(fp)) != EOF)
	{
		printf("%c", ch);
	}
	printf("\n");
	fclose(fp);

	pthread_mutex_lock(&(((pass *)(arg))->mu));
	((pass *)(arg))->readcount--;
	printf("Reader%d is leaving ...\n", ((Index *)(ar))->id);
	printf("Reader Count = %d\n", ((pass *)(arg))->readcount);
	if(((pass *)(arg))->readcount == 0)
	{
		printf("All Readers have left the critical section\n");
		sem_post(&(((pass *)(arg))->wrt)); 
	}
	pthread_mutex_unlock(&(((pass *)(arg))->mu));
	pthread_exit(NULL);
}
void* writer_thr(void* ar)
{	
	if(rand() % 2) // Doing a random sleep to demonstrate Process Synchronization
	{
		float sleeper = ((float)(rand() % 20)) / 10.00;
		sleep(sleeper);
	}
	sem_wait(&(((pass *)(arg))->wrt)); 
	char ch = (char)(rand() % 26 + 'a');
	printf("Writer%d is entering the critical section and writing %c\n", ((Index *)(ar))->id, ch);
	FILE *fp;
	fp = fopen("data.txt", "a");
	putc(ch, fp);
	fclose(fp);
	printf("Writer%d is leaving the critical section\n", ((Index *)(ar))->id);
	sem_post(&(((pass *)(arg))->wrt)); 
	pthread_exit(NULL);
}