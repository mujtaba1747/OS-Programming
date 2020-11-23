
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
    char *ba; 
    char *name;
    char *delim = ",.-\n";
    // sscanf(str, "%s", name);
    // sscanf(str, "%s", ba);
    // printf("%s %s\n", name, ba);
    char *x = (char *) malloc(sizeof(char) * 8196);
    strcpy(x, str);
    char *words = strtok(x, delim);
    name = words;
    words = strtok(NULL, delim);
    ba = words;
    int bal = atoi(ba);
    printf("%s, welcome to the bank !\n", name);
    int ch = 1;
    while(ch < 4)
    {            
        printf("Enter your choice :\n1. Current Balance\n2. Add Balance\n3. Withdraw\n4. Exit\n");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                printf("Current Balance is : %d\n", bal);
                break;
            case 2:
                printf("Enter amt to be added : \n");
                int tmp;
                scanf("%d", &tmp);
                bal += abs(tmp);
                break;
            case 3:
                printf("Enter amount to be Withdrawn : \n");
                scanf("%d", &tmp);
                if(tmp > bal) {
                    printf("Insufficient Balance\n");
                } else {
                    bal -= tmp;
                    printf("Cash Withdrawn sucessfully, new Balance = %d\n", bal);
                }
                break;
            case 4: 
                break;
        }
    }
    

    //detach from shared memory  
    shmdt(str);
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
     
    return 0; 
} 
