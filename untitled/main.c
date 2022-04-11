#include <stdio.h>
#include<stdlib.h>
#include <string.h>

//process struct
struct Process{
    int R1;
    int B1;
    int R2;
    int B2;
    int q;
    int currentPhase;
    int hasStayed;
    int hasStayedChangedIn;
    int ready_time;
    int processNumber;
    int blockedInCycle;
    char status[100];
    int phase_time[100];
};

int main(int argc, char **argv){

    int n=atoi(argv[1]);
    int SA=atoi(argv[2]);

    if(SA==1){
        //FIFO
//-------------------------------------------------------------------------------------------------------------------------------------------------------
        struct Process processes[100];
        int currentTime=0;
        int maxCycle=0;
        int isIdle=1;
        int done=0;
        int hasReadyProcess;
        struct Process *process_chosen_to_run;

        for (int i=0; i<n; i++){
            struct Process p;
            p.processNumber=i;
            p.R1=atoi(argv[3+i*4]);
            p.B1=atoi(argv[3+1+i*4]);
            p.R2=atoi(argv[3+2+i*4]);
            p.B2=atoi(argv[3+3+i*4]);
            maxCycle=maxCycle+p.R1+p.R2+p.B1+p.B2;
            //set every process to be ready
            strcpy(p.status,"Ready");
            p.ready_time=0;
            p.phase_time[0]=p.R1;
            p.phase_time[1]=p.B1;
            p.phase_time[2]=p.R2;
            p.phase_time[3]=p.B2;
            p.currentPhase=-1;
            p.hasStayed=0;
            processes[i]=p;
        }

        process_chosen_to_run=&processes[0];

        for(int i=0;i<maxCycle;i++){//loop thru each cycle
            if(done==n){//if no process is left
                break;
            }
            printf("Cycle %d\n",i+1);
            hasReadyProcess=-1;
            if(isIdle==0){//if there is a process running
                for(int j=0;j<n;j++){//find the running process
                    if(strcmp(processes[j].status,"Terminate")==0){
                        continue;
                    }
                    if(strcmp(processes[j].status,"Run")==0){
                        if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process has finished running
                            strcpy(processes[j].status,"Blocked");
                            processes[j].blockedInCycle=i;
                            processes[j].hasStayed=0;
                            // printf("P%d blocked\n",j+1);
                            isIdle=1;
                            processes[j].hasStayed++;
                            processes[j].currentPhase++;
                        }
                        else{//if the process is still running
                            processes[j].hasStayed++;//increment the progress
                        }
                    }
                    else if(strcmp(processes[j].status,"Blocked")==0){
                        if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process finished blocking
                            if(processes[j].currentPhase==3){
                                // if(isIdle){
                                //compare with other ready processes
                                strcpy(processes[j].status,"Terminate");
                                printf("P%d state: %s \n",j+1,processes[j].status);
                                done++;
                                // }
                            }
                            else{
                                strcpy(processes[j].status,"Ready");
                                processes[j].ready_time=i;//set the process ready time to be current cycle
                                processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                            }
                        }
                        else{
                            processes[j].hasStayed++;
                            // printf("P%d blocked +1, now blocked for %d\n",j+1,processes[j].hasStayed);
                        }
                    }
                    // printf("P%d state: %s \n",j+1,processes[j].status);
                }
            }
            if(isIdle==1){//if there is no process running
                for(int j=0;j<n;j++){//find a ready process to run
                    if(strcmp(processes[j].status,"Terminate")==0){//disregard the terminated process
                        continue;
                    }
                    if((strcmp(processes[j].status,"Ready")==0) && isIdle==1){//if there's a ready process
                        //if it is the first ready process found, set process_chosen_to_run
                        if(hasReadyProcess==-1){
                            hasReadyProcess=1;
                            isIdle=0;
                            process_chosen_to_run=&processes[j];
                        }
                        else{
                            // compare the ready time and choose a process to run
                            if(processes[j].ready_time>process_chosen_to_run->ready_time){//if current process_to_run is created earlier
                                continue;//do nothing
                            }
                                // if they have equal creation times
                            else if(processes[j].ready_time==process_chosen_to_run->ready_time){
                                //choose alphabetically
                                if(processes[j].processNumber>=process_chosen_to_run->processNumber){//if current process_to_run is alphabetically smaller
                                    continue;
                                }
                                else{//if current process_to_run is bigger
                                    process_chosen_to_run=&processes[j];//set new process_to_run
                                }
                            }
                            else{//
                                process_chosen_to_run=&processes[j];
                            }
                        }
                    }
                    // printf("P%d state: %s \n",j+1,processes[j].status);
                }
                // printf("has ready process: %d\n",hasReadyProcess);
                //now run the process and update the attributes
                if(hasReadyProcess==1){
                    strcpy(process_chosen_to_run->status,"Run");//problem: shouldn't rerun the blocked processes
                    process_chosen_to_run->currentPhase++;
                    process_chosen_to_run->hasStayed++;
                }
                // if no ready process is found and the cpu is idle, update the blocked processes
                if(hasReadyProcess==-1){
                    for(int j=0;j<n;j++){
                        if((strcmp(processes[j].status,"Blocked")==0)&& (processes[j].blockedInCycle!=i)){//find the blocked processes
                            if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process finished blocking
                                if(processes[j].currentPhase==3){
                                    strcpy(processes[j].status,"Terminate");
                                    printf("P%d state: %s \n",j+1,processes[j].status);
                                    done++;
                                }
                                else{
                                    strcpy(processes[j].status,"Ready");
                                    processes[j].ready_time=i;//set the process ready time to be current cycle
                                    processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                                }
                            }
                            else{
                                processes[j].hasStayed++;
                                // printf("P%d blocked +1, now blocked for %d\n",j+1,processes[j].hasStayed);
                            }
                        }
                    }
                }
            }
            for(int j=0;j<n;j++){
                if(strcmp(processes[j].status,"Terminate")==0){
                    continue;
                }
                printf("P%d state: %s \n",j+1,processes[j].status);
            }
        }
    }

    else{
        //RR
//-------------------------------------------------------------------------------------------------------------------------------------------------------
        int q=atoi(argv[3]);
        struct Process processes[100];
        int currentTime=0;
        int maxCycle=0;
        int isIdle=1;
        int done=0;
        int hasReadyProcess;
        int mark=-1;
        struct Process *process_chosen_to_run;

        for (int i=0; i<n; i++){
            struct Process p;
            p.processNumber=i;
            p.R1=atoi(argv[4+i*4]);
            // printf("R1: %d\n",p.R1);
            p.B1=atoi(argv[4+1+i*4]);
            // printf("B1: %d\n",p.B1);
            p.R2=atoi(argv[4+2+i*4]);
            // printf("R2: %d\n",p.R2);
            p.B2=atoi(argv[4+3+i*4]);
            // printf("B2: %d\n",p.B2);
            maxCycle=maxCycle+p.R1+p.R2+p.B1+p.B2;
            //set every process to be ready
            strcpy(p.status,"Ready");
            p.ready_time=0;
            p.phase_time[0]=p.R1;
            p.phase_time[1]=p.B1;
            p.phase_time[2]=p.R2;
            p.phase_time[3]=p.B2;
            p.currentPhase=-1;
            p.hasStayed=0;
            processes[i]=p;
        }

        process_chosen_to_run=&processes[0];

        for(int i=0;i<maxCycle;i++){//loop thru each cycle
            if(done==n){//if no process is left
                break;
            }
            printf("Cycle %d\n",i+1);
            hasReadyProcess=-1;
            if(isIdle==0){//if there is a process running
                for(int j=0;j<n;j++){//find the running process
                    if(strcmp(processes[j].status,"Terminate")==0){
                        continue;
                    }
                    if(strcmp(processes[j].status,"Run")==0){
                        // printf("p%d has stayed %d\n",j+1,processes[j].hasStayed);
                        // printf("p%d current phase %d\n",j+1,processes[j].currentPhase);
                        if(processes[j].hasStayed!=processes[j].phase_time[processes[j].currentPhase]){//if the process is still running
                            if (processes[j].hasStayed%q==0){//if the process should preempt
                                strcpy(processes[j].status,"Ready");
                                processes[j].ready_time=i;
                                printf("P%d preempted, ready time %d\n",j+1,processes[j].ready_time+1);
                                isIdle=1;
                                mark=j;
                            }
                            else{//the process shouldve preempt, but there isn't other ready process so keep this process running
                                processes[j].hasStayed++;//increment the progress
                                processes[j].hasStayedChangedIn=i;
                            }
                        }
                        else if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process has finished running
                            // printf("P%d has stayed %d, current phase is %d\n",j+1,processes[j].hasStayed,processes[j].currentPhase);
                            strcpy(processes[j].status,"Blocked");
                            processes[j].blockedInCycle=i;
                            processes[j].hasStayed=0;
                            // printf("P%d blocked\n",j+1);
                            isIdle=1;
                            processes[j].hasStayed++;
                            processes[j].hasStayedChangedIn=i;
                            processes[j].currentPhase++;
                        }
                    }
                    else if(strcmp(processes[j].status,"Blocked")==0){
                        if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process finished blocking
                            if(processes[j].currentPhase==3){
                                if(isIdle){
                                    strcpy(processes[j].status,"Terminate");
                                    // printf("P%d state: %s \n",j+1,processes[j].status);
                                    done++;
                                }
                            }
                            else{
                                strcpy(processes[j].status,"Ready");
                                processes[j].ready_time=i;//set the process ready time to be current cycle
                                processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                                processes[j].hasStayedChangedIn=i;
                                printf("P%d changed to %s, ready time %d\n",j+1,processes[j].status,processes[j].ready_time+1);
                            }
                        }
                        else{
                            processes[j].hasStayed++;
                            processes[j].hasStayedChangedIn=i;
                            // printf("P%d blocked +1, now blocked for %d\n",j+1,processes[j].hasStayed);
                        }
                    }
                    // printf("P%d state: %s \n",j+1,processes[j].status);
                }
            }
            if(isIdle==1){//if there is no process running
                for(int j=0;j<n;j++){//find a ready process to run
                    // printf("has ready process: %d\n",hasReadyProcess);
                    if(strcmp(processes[j].status,"Terminate")==0){//disregard the terminated processes
                        continue;
                    }
                    //if there's a ready process
                    if((strcmp(processes[j].status,"Ready")==0) && isIdle==1){
                        // printf("P%d is ready to run\n",j+1);
                        // printf("P%d status is %s\n",j+1,processes[j].status);
                        if(mark==j){//if it was just-preempted
                            continue;
                        }
                        if(hasReadyProcess==-1){//if it is the first ready process found, set process_chosen_to_run
                            hasReadyProcess=1;
                            isIdle=0;
                            process_chosen_to_run=&processes[j];
                            printf("p%d chosen to run\n",j+1);
                        }
                        else{
                            // compare the ready time and choose a process to run
                            if(processes[j].ready_time>process_chosen_to_run->ready_time){//if current process_to_run is created earlier
                                continue;//do nothing
                            }
                                // if they have equal creation times
                            else if(processes[j].ready_time==process_chosen_to_run->ready_time){
                                //choose alphabetically
                                if(processes[j].processNumber>=process_chosen_to_run->processNumber){//if current process_to_run is alphabetically smaller
                                    continue;
                                }
                                else{//if current process_to_run is bigger
                                    process_chosen_to_run=&processes[j];//set new process_to_run
                                }
                            }
                            else{//
                                process_chosen_to_run=&processes[j];
                            }

                        }
                    }
                    // printf("P%d state: %s \n",j+1,processes[j].status);
                }
                // printf("has ready process: %d\n",hasReadyProcess);
                //now run the process and update the attributes
                if(hasReadyProcess==1){
                    int run_id=process_chosen_to_run->processNumber;
                    for(int j=0;j<n;j++){//compare again
                        if(processes[j].ready_time<processes[run_id].ready_time){
                            run_id=j;
                        }
                        if(processes[j].ready_time==processes[run_id].ready_time){
                            if(processes[j].processNumber<processes[run_id].processNumber){
                                run_id=j;
                            }
                        }
                    }
                    process_chosen_to_run=&processes[run_id];
                    strcpy(process_chosen_to_run->status,"Run");
                    // printf("P%d chosen to run\n",process_chosen_to_run->processNumber+1);
                    if(process_chosen_to_run->currentPhase==-1 || //if the process hasn't been started
                       process_chosen_to_run->currentPhase==1 || //if the process was previously blocked
                       process_chosen_to_run->currentPhase==3){//if the process was previously blocked
                        // printf("current phase +1\n");
                        // printf("mark: %d\n",mark);
                        // printf("process num: %d\n",process_chosen_to_run->processNumber);
                        process_chosen_to_run->currentPhase++;
                    }
                    process_chosen_to_run->hasStayed++;
                    process_chosen_to_run->hasStayedChangedIn=i;
                    // printf("P%d has stayed %d\n",process_chosen_to_run->processNumber+1,process_chosen_to_run->hasStayed);
                }
                // if no ready process is found and the cpu is idle, update the blocked processes
                if(hasReadyProcess==-1){
                    // printf("no ready process\n");
                    for(int j=0;j<n;j++){
                        if((strcmp(processes[j].status,"Blocked")==0)&& (processes[j].blockedInCycle!=i)){//find the blocked processes
                            // printf("found p%d, blocked\n",j+1);
                            if(processes[j].hasStayedChangedIn==i){//if the process hasstayed has just +1 in the same cycle
                                continue;
                            }
                            if(processes[j].hasStayed==processes[j].phase_time[processes[j].currentPhase]){//if the process finished blocking
                                if(processes[j].currentPhase==3 && isIdle==1){
                                    strcpy(processes[j].status,"Terminate");
                                    // printf("P%d state: %s \n",j+1,processes[j].status);
                                    done++;
                                }
                                else{
                                    if(isIdle==0){//if is not idle
                                        strcpy(processes[j].status,"Ready");
                                        processes[j].ready_time=i;//set the process ready time to be current cycle
                                        processes[j].hasStayed=0;//clear out the phase hasStayed attribute
                                        processes[j].hasStayedChangedIn=i;
                                    }
                                    else{
                                        strcpy(processes[j].status,"Run");
                                        processes[j].hasStayed=0;
                                        processes[j].currentPhase++;
                                        processes[j].hasStayed++;
                                        processes[j].hasStayedChangedIn++;
                                        processes[j].ready_time=i;
                                        printf("P%d changed to %s, ready time %d\n",j+1,processes[j].status,processes[j].ready_time+1);
                                        isIdle=0;
                                    }
                                }
                            }
                            else{
                                processes[j].hasStayed++;
                                processes[j].hasStayedChangedIn=i;
                                // printf("P%d blocked +1, now blocked for %d\n",j+1,processes[j].hasStayed);
                            }
                        }
                        if(strcmp(processes[j].status,"Ready")==0 && mark==j){//find the ready process if no process is running
                            // printf("ready\n");
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
            for(int j=0;j<n;j++){
                if(strcmp(processes[j].status,"Terminate")==0){
                    continue;
                }
                printf("P%d state: %s \n",j+1,processes[j].status);
            }
        }
    }
    return 0;
}

