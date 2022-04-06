#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int algo;      
    int process;
    char str[51];
    int status;
    int p;
    char command;
    printf("Enter the number of processes\n");
    while(fgets( process, 50, stdin) != NULL){
        if(process <= 0) {
            printf("Enter the number of processes\n");
        }
        fgets( process, 50, stdin);
    }
    printf("Enter the scheduling algorithm to use. 1 signifies FIFO; 2 signifies RR with quantum q\n");
    while(fgets( algo, 50, stdin) != 1 && fgets( algo, 50, stdin) != 2 && fgets( algo, 50, stdin)!= NULL){
        printf("Enter the scheduling algorithm to use. 1 signifies FIFO; 2 signifies RR with quantum q\n");
    }

        printf("%d %d",process,algo);
    return 0;
}