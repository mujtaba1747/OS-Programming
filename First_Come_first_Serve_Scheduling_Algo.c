// FCFS CPU Scheduling Algorithm
// Author : Syed Mujtaba
// http://p.ip.fi/JU4C
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct process
{
	int arrival_time;
	int turnaround_time;
	int wait_time;
	int burst_time;
    int idx;
	process()
	{
		turnaround_time = 0;
        wait_time = 0;
	}
};
int compare(const void *a, const void *b)
{
    struct process *pr1 = (process *)(a);
    struct process *pr2 = (process *)(b);
    return (pr1 -> arrival_time > pr2 -> arrival_time);
}
void findWaitingTime(struct process*, size_t);
float findAvgTime(struct process*, size_t);
void findTurnAroundTime(struct process*, size_t );

int main()
{
    int n,avwt=0,avtat=0,i,j;
    printf("Enter total number of processes (more than 0) : ");
    scanf("%d",&n);
    assert(n > 0);
    struct process P[n];

    printf("\nEnter Arrival times of processes : \n");
    for(int i = 0; i < n; i++)
    {
        printf("Process%d -", i+1);
    	scanf("%d", &P[i].arrival_time);
        P[i].idx = i + 1;
    }

    printf("\nEnter Burst Times of Processes : \n");
    for(int i = 0; i < n; i++) 
    {
        printf("Process%d -", i+1);
        scanf("%d", &P[i].burst_time);
    }

    qsort(P, n, sizeof(struct process), compare);
    printf("\nAverage turn around time is : %f\n", findAvgTime(P, n));
    printf("PID\tTAT\tWT\tBT\n");
    for(int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\n", P[i].idx, P[i].turnaround_time, P[i].wait_time, P[i].burst_time);
    }
    return 0;
}

void findWaitingTime(struct process* P, size_t len)
{
    P[0].wait_time = 0;
    for(int i = 1; i < len; i++)
    {
        P[i].wait_time = P[i - 1].wait_time + P[i - 1].burst_time;
    }
}
void findTurnAroundTime(struct process *P, size_t n)
{
    for(int i = 0; i < n; i++)
    {
        P[i].turnaround_time = P[i].wait_time + P[i].burst_time;
    }
}
float findAvgTime(struct process *P, size_t n)
{
    findWaitingTime(P, n);
    findTurnAroundTime(P, n);
    float avg = 0.0;
    for(int i = 0; i < n; i++)
    {
        avg += P[i].turnaround_time;
    } 
    avg /= n;
    return avg;
}
