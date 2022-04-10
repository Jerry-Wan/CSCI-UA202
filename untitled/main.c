#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char* algo;
    char* process;
    char* quant;
    char str[51];
    char** processes = NULL;


    fgets(str, sizeof(str), stdin);
    //printf("%s",str);
    str[strlen(str) - 1] = 0;

    char ** res  = NULL;
    char * p    = strtok (str, " ");
    int n_spaces = 0, i;

    while (p) {
        //printf("%s ",p);
        res = realloc (res, sizeof (char*) * ++n_spaces);

        if (res == NULL)
            exit (-1); /* memory allocation failed */

        res[n_spaces-1] = p;
        //printf("%s    ", res[n_spaces-1]);
        //printf("%d\n",n_spaces-1);
        p = strtok (NULL, " ");

    }


    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;

    process = res[0];
    algo = res[1];

    //printf("%d %s\n",n_spaces-1,res[n_spaces-1]);

    if(*algo == '2'){
        quant = res[2];
        for(int a = 3; a < n_spaces; a++){
            printf("%d",a);
            printf("   %s\n",res[a]);
        }
    }
    else if(*algo == '1'){
        for(int a = 2; a < n_spaces; a++){
            printf("%d",a);
            printf("   %s\n",res[a]);
        }
    }
    else{
        printf("Invalid number, please try again later");
        exit(-1);
    }

    printf("%s, %s, %s",process,algo, quant);



    return 0;
}