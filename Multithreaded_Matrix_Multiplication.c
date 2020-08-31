// Author : Syed Mujtaba
// TE-11
// Matrix Multplication using Threads
// Compile using gcc file.c -lpthread
// Don't declare matrices of the likes of 1000 * 1000 if you don't want a segfault.
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
struct pass // A struct to pass to thread function
{
	int **m1, **m2;
	int ro, co, k;
};
void *thread_function(void *arg);
void init(int ***arr, int n, int m) // Use pointer to double pointer or return a double pointer
{
	*arr = (int **)malloc(n * sizeof(int *));
	for(int i = 0; i < n; i++)
	{
		(*arr)[i] = (int *)malloc(m * sizeof(int));
	}
	printf("Enter Matrix Values for %d * %d\n", n, m);
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			scanf("%d", &((*arr)[i][j]));
		}
	}
}
int main()
{
	int m, k, n, res;
	printf("Enter 3 Dimensions : ");
	scanf("%d%d%d", &m, &k, &n);
	assert(m > 0);
	assert(k > 0);
	assert(n > 0);

	int **m1;
	int **m2;
	int ***i = &m1;
	int **ANS;
	init(&m1, m, k);
	init(&m2, k, n);
	ANS = (int **)malloc(m * sizeof(int *));
	for(int i = 0; i < m; i++)
		ANS[i] = (int *)malloc(n * sizeof(int));
	printf("Enter choice of execution : \n1. Threads in parallel\n2. Single Thread\n");
	int ch;
	scanf("%d", &ch);
	clock_t b = clock(); // Remove clock to remove Seg Error in case of huge size
	if(ch == 2)
	{
		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				int el = 0;
				for(int w = 0; w < k; w++)
				{
					el += m1[i][w]*m2[w][j];
				}
				ANS[i][j] = el;
			}
		}	
	}
	else
	{
		pthread_t test, thread[m][n];
		void *thread_result;
		struct pass P[m][n];
		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				P[i][j].m1 = m1;
				P[i][j].m2 = m2;
				P[i][j].k  = k;
				P[i][j].ro = i;
				P[i][j].co = j;
			}
		}

		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				int res = pthread_create(&thread[i][j], NULL, thread_function, (void *)(&P[i][j]));
			}
		}
		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				res = pthread_join(thread[i][j], &thread_result);
				ANS[i][j] = *(int *)(thread_result);
			}
		}
	}
	clock_t en = clock();
	float time = (long double)(en - b);
	printf("Result of matrix multiplication is : \n");
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			printf("%d ", ANS[i][j]);
		}
		printf("\n");
	}
	printf("Time taken : %f\n", time);
	exit(EXIT_SUCCESS);
}
void *thread_function(void *arg)
{
	// sleep(1); // Uncomment in case of buggy output
	int ret = 0;
	struct pass temp = *((struct pass *)arg);
	for(int i = 0; i < (temp).k; i++)
	{
		int val1 = temp.m1[temp.ro][i];
		int val2 = temp.m2[i][temp.co];
		ret += val1*val2;
	}
	(*(struct pass *)(arg)).k = ret; // Storing result in k of *arg from main // Do not return pointer to object created in thread, as it shall cease to exist after thread terminates. 
	void *u = &((*(struct pass *)arg).k);
	pthread_exit(u);
	// pthread_exit(NULL); // to-do
}
