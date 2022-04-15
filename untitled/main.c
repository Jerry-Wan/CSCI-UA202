#include <stdio.h>
#include<stdlib.h>
#include <string.h>

struct Process{
    int Run1;
    int Block1;
    int Run2;
    int Block2;
    int currentPhase;
    int hasStayed;
    int hasStayedChangedIn;
    int readyTime;
    int processNumber;
    int blockedInCycle;
    char status[100];
    int phaseTime[100];
};

int main(){

    char str[51];
    int  a,i,j,n,algo;
    int blockedProcess;
    struct Process processes[100];
    int maxCycle=0;
    int isIdle=1;
    int done=0;
    int hasReadyProcess;
    int mark=-1;
    struct Process *processChosenToRun;
    char ** argv  = NULL;
    char * p;
    int n_spaces = 0;

    fgets(str, sizeof(str), stdin);

    p  = strtok (str, " ");


    while (p) {
        argv = realloc (argv, sizeof (char*) * ++n_spaces);

        if (argv == NULL)
            exit (-1);

        argv[n_spaces-1] = p;

        p = strtok (NULL, " ");
    }


    argv = realloc (argv, sizeof (char*) * (n_spaces+1));
    argv[n_spaces] = 0;


    n=atoi(argv[0]);
    algo=atoi(argv[1]);

    if(algo <1 || algo >2){
        printf("Sorry, invalid algo detected");
        exit(1);
    }
    if(n <1){
        printf("Sorry, invalid number of process detected");
        exit(1);
    }
    if(algo == 1){
        if(argv[n*4+1] == NULL ||argv[n*4+2] != NULL ){
            printf("Sorry, invalid number of process time detected");
            exit(1);
        }
    }
    else{
        if(argv[n*4+2] == NULL ||argv[n*4+3] != NULL){
            printf("Sorry, invalid number of process time detected");
            exit(1);
        }
    }

    printf("Cycle       ");
    for(a = 0; a<n;a++){
        printf("P%d State       ",a+1);
    }
        printf("Comment\n");



    if(algo == 1){

        for ( i=0; i<n; i++){
            struct Process p;
            p.processNumber=i;
            p.Run1=atoi(argv[2+i*4]);
            p.Block1=atoi(argv[2+1+i*4]);
            p.Run2=atoi(argv[2+2+i*4]);
            p.Block2=atoi(argv[2+3+i*4]);
            maxCycle=maxCycle+p.Run1+p.Run2+p.Block1+p.Block2;
            strcpy(p.status,"Ready");
            p.readyTime=0;
            p.phaseTime[0]=p.Run1;
            p.phaseTime[1]=p.Block1;
            p.phaseTime[2]=p.Run2;
            p.phaseTime[3]=p.Block2;
            p.currentPhase=-1;
            p.hasStayed=0;
            processes[i]=p;
        }

        processChosenToRun=&processes[0];

        for( i=0;i<maxCycle;i++){
            if(done==n){
                break;
            }
            printf("%-12d",i+1);
            hasReadyProcess=-1;
            if(isIdle==0){
                for( j=0;j<n;j++){
                    if(strcmp(processes[j].status,"Terminate")==0){
                        continue;
                    }
                    else if(strcmp(processes[j].status,"Run")==0){
                        if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){
                            strcpy(processes[j].status,"Blocked");
                            processes[j].blockedInCycle=i;
                            processes[j].hasStayed=0;
                            isIdle=1;
                            processes[j].hasStayed++;
                            processes[j].currentPhase++;
                        }
                        else{
                            processes[j].hasStayed++;
                        }
                    }
                    else if(strcmp(processes[j].status,"Blocked")==0){
                        if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){
                            if(processes[j].currentPhase==3){
                                strcpy(processes[j].status,"Terminate");
                                printf("%-15s",processes[j].status);
                                if(j == n-1){
                                    printf("\n");
                                }
                                done++;
                            }
                            else{
                                strcpy(processes[j].status,"Ready");
                                processes[j].readyTime=i;
                                processes[j].hasStayed=0;
                            }
                        }
                        else{
                            processes[j].hasStayed++;
                        }
                    }
                }
            }
            if(isIdle==1){
                for( j=0;j<n;j++){
                    if(strcmp(processes[j].status,"Terminate")==0){
                        printf("               ");
                        continue;
                    }
                    if((strcmp(processes[j].status,"Ready")==0) && isIdle==1){
                        if(hasReadyProcess==-1){
                            hasReadyProcess=1;
                            isIdle=0;
                            processChosenToRun=&processes[j];
                        }
                        else{
                            if(processes[j].readyTime>processChosenToRun->readyTime) {
                                continue;
                            }
                            else if(processes[j].readyTime==processChosenToRun->readyTime){
                                if(processes[j].processNumber>=processChosenToRun->processNumber){
                                    continue;
                                }
                                else{
                                    processChosenToRun=&processes[j];
                                }
                            }
                            else{
                                processChosenToRun=&processes[j];
                            }
                        }
                    }
                }
                if(hasReadyProcess==1){
                    strcpy(processChosenToRun->status,"Run");
                    processChosenToRun->currentPhase++;
                    processChosenToRun->hasStayed++;
                }
                if(hasReadyProcess==-1){
                    for( j=0;j<n;j++){
                        if((strcmp(processes[j].status,"Blocked")==0)&& (processes[j].blockedInCycle!=i)){
                            if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){
                                if(processes[j].currentPhase==3){
                                    strcpy(processes[j].status,"Terminate");
                                    printf("%-15s",processes[j].status);
                                    done++;
                                }
                                else{
                                    strcpy(processes[j].status,"Ready");
                                    processes[j].readyTime=i;
                                    processes[j].hasStayed=0;
                                }
                            }
                            else{
                                processes[j].hasStayed++;
                            }
                        }
                    }
                }
            }
            for( j=0;j<n;j++){
                if(strcmp(processes[j].status,"Terminate")==0){
                    continue;
                }
                printf("%-15s",processes[j].status);
                if(j == n-1 && i!= 0){
                    printf("\n");
                }
                if(i == 0 && j == n-1){
                    printf("Both created; P1 wins tiebreak\n");
                }
            }
        }
    }

    else{
        int q=atoi(argv[2]);

        for ( i=0; i<n; i++){
            struct Process p;
            p.processNumber=i;
            p.Run1=atoi(argv[3+i*4]);
            p.Block1=atoi(argv[3+1+i*4]);
            p.Run2=atoi(argv[3+2+i*4]);
            p.Block2=atoi(argv[3+3+i*4]);
            maxCycle=maxCycle+p.Run1+p.Run2+p.Block1+p.Block2;
            strcpy(p.status,"Ready");
            p.readyTime=0;
            p.phaseTime[0]=p.Run1;
            p.phaseTime[1]=p.Block1;
            p.phaseTime[2]=p.Run2;
            p.phaseTime[3]=p.Block2;
            p.currentPhase=-1;
            p.hasStayed=0;
            processes[i]=p;
        }

        processChosenToRun=&processes[0];
        for( i=0;i<maxCycle;i++){//loop thru each cycle
            if(done==n){//if no process is left
                break;
            }
            printf("%-12d",i+1);
            hasReadyProcess=-1;
            if(isIdle==0){//if there is a process running
                for( j=0;j<n;j++){//find the running process
                    if(strcmp(processes[j].status,"Terminate")==0){
                        continue;
                    }
                    if(strcmp(processes[j].status,"Run")==0){
                        if(processes[j].hasStayed!=processes[j].phaseTime[processes[j].currentPhase]){//if the process is still running
                            if (processes[j].hasStayed%q==0){//if the process should preempt
                                strcpy(processes[j].status,"Ready");
                                processes[j].readyTime=i;
                                isIdle=1;
                                mark=j;
                            }
                            else{//the process shouldve preempt, but there isn't other ready process so keep this process running
                                processes[j].hasStayed++;//increment the progress
                                processes[j].hasStayedChangedIn=i;
                            }
                        }
                        else if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){//if the process has finished running
                            strcpy(processes[j].status,"Blocked");
                            processes[j].blockedInCycle=i;
                            processes[j].hasStayed=0;
                            isIdle=1;
                            processes[j].hasStayed++;
                            processes[j].hasStayedChangedIn=i;
                            processes[j].currentPhase++;
                        }
                    }
                    else if(strcmp(processes[j].status,"Blocked")==0){
                        if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){//if the process finished blocking
                            if(processes[j].currentPhase==3){
                                if(isIdle){
                                    strcpy(processes[j].status,"Terminate");
                                    done++;
                                }
                            }
                            else{
                                strcpy(processes[j].status,"Ready");
                                processes[j].readyTime=i;//set the process ready time to be current cycle
                                processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                                processes[j].hasStayedChangedIn=i;
                            }
                        }
                        else{
                            processes[j].hasStayed++;
                            processes[j].hasStayedChangedIn=i;
                        }
                    }
                }
            }
            if(isIdle==1){//if there is no process running
                for( j=0;j<n;j++){//find a ready process to run
                    if(strcmp(processes[j].status,"Terminate")==0){//disregard the terminated processes
                        printf("               ");
                        continue;
                    }
                    //if there's a ready process
                    if((strcmp(processes[j].status,"Ready")==0) && isIdle==1){
                        if(mark==j){//if it was just-preempted
                            continue;
                        }
                        if(hasReadyProcess==-1){//if it is the first ready process found, set process_chosen_to_run
                            hasReadyProcess=1;
                            isIdle=0;
                            processChosenToRun=&processes[j];
                        }
                        else{
                            // compare the ready time and choose a process to run
                            if(processes[j].readyTime>processChosenToRun->readyTime){//if current process_to_run is created earlier
                                continue;//do nothing
                            }
                                // if they have equal creation times
                            else if(processes[j].readyTime==processChosenToRun->readyTime){
                                //choose alphabetically
                                if(processes[j].processNumber>=processChosenToRun->processNumber){//if current process_to_run is alphabetically smaller
                                    continue;
                                }
                                else{//if current process_to_run is bigger
                                    processChosenToRun=&processes[j];//set new process_to_run
                                }
                            }
                            else{//
                                processChosenToRun=&processes[j];
                            }

                        }
                    }
                }
                if(hasReadyProcess==1){
                    int run_id=processChosenToRun->processNumber;
                    for( j=0;j<n;j++){//compare again
                        if(processes[j].readyTime<processes[run_id].readyTime){
                            run_id=j;
                        }
                        if(processes[j].readyTime==processes[run_id].readyTime){
                            if(processes[j].processNumber<processes[run_id].processNumber){
                                run_id=j;
                            }
                        }
                    }
                    processChosenToRun=&processes[run_id];
                    strcpy(processChosenToRun->status,"Run");
                    if(processChosenToRun->currentPhase==-1 ||processChosenToRun->currentPhase==1 ||processChosenToRun->currentPhase==3){
                        processChosenToRun->currentPhase++;
                    }
                    processChosenToRun->hasStayed++;
                    processChosenToRun->hasStayedChangedIn=i;
                }
                // if no ready process is found and the cpu is idle, update the blocked processes
                if(hasReadyProcess==-1){
                    for( j=0;j<n;j++){
                        if((strcmp(processes[j].status,"Blocked")==0)&& (processes[j].blockedInCycle!=i)){//find the blocked processes
                            if(processes[j].hasStayedChangedIn==i){//if the process hasstayed has just +1 in the same cycle
                                continue;
                            }
                            if(processes[j].hasStayed==processes[j].phaseTime[processes[j].currentPhase]){//if the process finished blocking
                                if(processes[j].currentPhase==3 && isIdle==1){
                                    strcpy(processes[j].status,"Terminate");
                                    done++;
                                }
                                else{
                                    if(isIdle==0){//if is not idle
                                        strcpy(processes[j].status,"Ready");
                                        processes[j].readyTime=i;//set the process ready time to be current cycle
                                        processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                                        processes[j].hasStayedChangedIn=i;
                                    }
                                    else{
                                        strcpy(processes[j].status,"Run");
                                        processes[j].hasStayed=0;
                                        processes[j].currentPhase++;
                                        processes[j].hasStayed++;
                                        processes[j].hasStayedChangedIn++;
                                        processes[j].readyTime=i;
                                        isIdle=0;
                                    }
                                }
                            }
                            else{
                                processes[j].hasStayed++;
                                processes[j].hasStayedChangedIn=i;
                            }
                        }
                        if(strcmp(processes[j].status,"Ready")==0 && mark==j){//find the ready process if no process is running
                            strcpy(processes[mark].status,"Run");//run the just preempted process
                            processes[mark].hasStayed++;
                            processes[mark].hasStayedChangedIn=i;
                            isIdle=0;
                            hasReadyProcess=1;
                            if(processes[mark].currentPhase==-1 || //if the process hasn't been started
                               processes[mark].currentPhase==1 || //if the process was previously blocked
                               processes[mark].currentPhase==3){//if the process was previously blocked
                                processes[mark].currentPhase++;
                            }
                        }
                    }
                }
            }
            for( j=0;j<n;j++){
                blockedProcess = 0;
                if(strcmp(processes[j].status,"Terminate")==0){
                    continue;
                }
                if(strcmp(processes[j].status,"Blocked")==0){
                    blockedProcess+=1;
                }
                printf("%-15s",processes[j].status);
                if(i == 0 && j == n-1){
                    printf("Both created; P1 wins tiebreak\n");
                }
                else if(blockedProcess == n-1 && j == n-1){
                    printf("CPU idle\n");
                }
                else  if(j == n-1 && i!= 0){
                    printf("\n");
                }
            }
        }
    }
    return 0;
}

