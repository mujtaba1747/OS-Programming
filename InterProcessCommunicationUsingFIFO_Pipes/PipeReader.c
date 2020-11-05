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
int main()
{
    unlink("./Pipe1"); 
    char *delim = " ,.-\n";
    char * myPIPE = "./Pipe1";

    char * sentence = malloc(2048 * sizeof(char));
    char * result = malloc(2048 * sizeof(char));

    if(mkfifo(myPIPE, 0666)) {
        printf("Pipe Error\n");
        exit(0);
    }

    int fd = open(myPIPE, O_RDONLY);
    read(fd, sentence, sizeof(char) * 2048);
    
    printf("Reader Received String : %s\n", sentence);
    int wc = 0, cc = strlen(sentence), lc = 0; 
    for(int i = 0; i < strlen(sentence); i++) {
        if(sentence[i] == '\n') {
            lc++;
        }
    }   
    char *words = strtok(sentence, delim);
    while (words) {
        printf("Word[%d] : ", wc++);
        printf("%s\n", words);
        words = strtok(NULL, delim);
    }
    printf("Word Count : %d\tChar Count : %d\n", wc, cc);
    close(fd);

    sleep(2);

    fd = open(myPIPE, O_WRONLY);
    sprintf(result, "Word Count : %d\tChar Count : %d\tLine Count : %d\n", wc, cc, ++lc);
    write(fd, result, strlen(result) + 1);
    FILE *fp = fopen("Result.txt", "w");
    fprintf(fp, "%s", result);
    close(fd);

    return 0;
}
