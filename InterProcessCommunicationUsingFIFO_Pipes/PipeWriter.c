// Author : Syed Mujtaba
// TE - 11
// Assignment 7.2 : Duplex Communication using named pipes

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
char *sentence;
char *temp;
int main()
{
	sentence = malloc(sizeof(char) * 2048);
	temp = malloc(sizeof(char) * 2048);

	char * myPIPE = "./Pipe1";
	FILE *fp = fopen("./input_message.txt", "r");
	while( !feof(fp) ) {
		fgets(temp, 2048, fp);
		strcat(sentence, temp);
	}
	fclose(fp);
	printf("%s\n", sentence);
	int fd = open(myPIPE, O_WRONLY);
	write(fd, sentence, (sizeof(char))*(strlen(sentence) + 1));
	close(fd);

	fd = open(myPIPE, O_RDONLY);
	read(fd, sentence, 2048 * sizeof(char));
	printf("Result from Other Program : \n%s", sentence);

	unlink(myPIPE);
	return 0;
}
