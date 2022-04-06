#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int algo;      
    int process;
    int quant = 0;
    char str[51];
    int status;
    int p;
    char command;

    fgets(str, sizeof(str), stdin);
    printf("%s",str);


    return 0;
}