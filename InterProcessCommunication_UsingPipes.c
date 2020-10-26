// http://p.ip.fi/QvHK
// Author : Syed Mujtaba
// TE - 11
// Assignment 7 : Duplex Communication using pipes

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int c = 1;
int main(int argc, char *argv[])
{
	int pipeFD1[2];
	int pipeFD2[2];
	pipe(pipeFD1);
	printf("Pipe1 with FD's : %d %d Created\n", pipeFD1[0], pipeFD1[1]);
	pipe(pipeFD2);
	printf("Pipe2 with FD's : %d %d Created\n", pipeFD2[0], pipeFD2[1]);
	switch( fork() )
	{
		case -1:
			// Fork Error
			return 1;
		case 0: // In child
		{
			// int count = 1;
			// while(count--)
			{
				int pid = getpid();
				close(pipeFD1[0]); // Close pipe1 read side
				close(pipeFD2[1]); // Close pipe2 write side
				char readMsg[512];
				read(pipeFD2[0], readMsg, sizeof(readMsg));
				while(strlen(readMsg) == 0) 
					read(pipeFD2[0], readMsg, sizeof(readMsg));

				printf("In processID = %d\tMessage Recieved : %s\n", pid, readMsg);

				char msg[512] = "Sure, here you go\t";
				char inp[512];
				printf("Enter Student Details : ");
				scanf("\n");
				gets(inp);
				strcat(msg, inp);
				write(pipeFD1[1], msg, sizeof(msg));
				break;
			}
		}
		default: // In parent
		{
			// int count = 1;
			// while(count--)
			{
				int pid = getpid();
				close(pipeFD2[0]); // Close pipe2 read side
				close(pipeFD1[1]); // Close pipe1 write side
				char msg[512] = "Give me name, roll no and city\n";
				write(pipeFD2[1], msg, sizeof(msg));
				sleep(2);
				char readMsg[512];
				read(pipeFD1[0], readMsg, sizeof(readMsg));

				while(strlen(readMsg) == 0) 
					read(pipeFD1[0], readMsg, sizeof(readMsg));

				printf("In processID = %d\tMessage Recieved : %s\n", pid, readMsg);
			}
		}

	}

}