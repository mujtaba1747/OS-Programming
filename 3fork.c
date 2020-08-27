#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int c = 1;
int main(int argc, char *argv[])
{
  pid_t cpid;   
  printf("%d 1 %d\n", getpid(), getppid());
  sleep(1);

  cpid = fork();
  sleep(1);
  printf("%d 2 %d\n", getpid(), getppid());
  cpid = fork();
  printf("%d 3 %d\n", getpid(), getppid());
  sleep(1);
  cpid = fork();
  printf("%d 4 %d\n", getpid(), getppid());
  if (cpid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) {    /* Child reads from pipe */
    // printf("cpid = %d\n", cpid);
    // printf("pid = %d\n", getpid());
    // c++;
    // printf("c is %d\n", c);
    // sleep(7);
    // printf("\n Child Process . . . . .");
    // printf("inti\n");
    // printf("parent cpid = %d\n", cpid);
    // printf("pid parent = %d\n", getppid());
  } 
  else {
    // sleep(5);            /* Parent writes argv[1] to pipe */
  // sleep(10);
    // c++;
    // printf("c is in par %d\n", c);
    // printf("parent cpid = %d\n", cpid);
    // printf("pid parent = %d\n", getpid());
    // printf("\n Parent Process . . . . . !!!!!!!\n");
    // sleep(2);
    // printf("c is in par %d\n", c);


    }
}
