#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
  
int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("./tmp", 112); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key, sizeof(char) * 8196, 0666 | IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    // cout<<"Write Data : "; 
    printf("Enter name : ");
    char *name = (char *) malloc(256 * sizeof(char));
    gets(name);
    int bal;
    printf("Enter Acc Balance : ");
    scanf("%d", &bal);
    char *balanceStr = (char *) malloc(128 * sizeof(char));
    sprintf(balanceStr, "%d", bal);
    sprintf(str, "%s\n%s", name, balanceStr);
    printf("Sending %s\n", str);
    shmdt(str); 
    printf("Data written in memory sucessfully\nServer Will now shut down !"); 
    return 0; 
} 

