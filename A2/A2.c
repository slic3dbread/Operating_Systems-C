#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <string.h>      /* String Functions */

typedef struct theThread
{
    int processNum;
    int threadNum;
    int arrivalTime;
    int threadCPUTime;
    int threadIOTime;
    int turnaroundTime;
    int finishTime;
    struct theThread* next;
} Thread;

void scheduler(int dStatus,int vStatus,int rStatus, int rValue,int* init,int **data);
void printList(Thread*  t,int threadSwitch, int processSwitch, int detail,int verbose);
Thread*  createThread(int processNum,int threadNum,int arrivalTime,int threadCPUTime,int threadIOTime,int turnaroundTime,int finishTime);
Thread*  addList(Thread*  list,Thread*  temp);

int main ( int argc, char *argv[]) {
   
    int dStatus=0, vStatus=0, rStatus=0, rValue=0;
    int i=1,j=-1,k=0;
    FILE* inFile;
    char outline[255];
    int init[3];
    char* token;
    int **data=malloc(sizeof(int*)*255);
    
    /*Checks to make sure the right amount of arguments were added*/
    if(argc>6){
        printf("Too many arguments.\n");
        return 1;
    }
    
    /*Checks the arguments values*/
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],"-d")==0)
            dStatus=1;
        else if(strcmp(argv[i],"-v")==0)
            vStatus=1;
        else if(strcmp(argv[i],"-r")==0){
            if((argc-i)>2){
                if(atoi(argv[i+1])>0){
                    rStatus=1;
                    rValue=atoi(argv[i+1]);
                }
            }
        }
    }
    
    /*Checks to see if file was able to be created*/
    inFile=fopen(argv[argc-1],"r+");
    if(inFile==NULL){
        printf("Error opening file.\n");
        return 1;
    }
    
    i=0;
    /*parsing file*/
    while (!feof(inFile)){
        if(j==-1){
            fgets(outline,255,inFile);
            token = strtok(outline, " \t\r\a\n");
            init[0]=atoi(token);
            token = strtok(NULL, " \t\r\a\n");
            init[1]=atoi(token);
            token = strtok(NULL, " \t\r\a\n");
            init[2]=atoi(token);
            j++;
        }
        fgets(outline,255,inFile);
        token = strtok(outline, " \t\r\a\n");
        data[i]=malloc(sizeof(int)*3);
        while (token!=NULL){
            data[i][j] = atoi(token);
            j++;
            token = strtok(NULL, " \t\r\a\n");
            if(j<3)
                data[i][j]=0;
        }
        j=0;
        printf("\n");
        i++;
    }
    
    /*Scheduler*/
    scheduler(dStatus,vStatus,rStatus,rValue,init,data);
   
    /*frees the double pointer*/
    for(k=0;k<i;k++)
        free(data[k]);
    free(data);
    fclose(inFile);
    return 0;
}

void scheduler(int dStatus,int vStatus,int rStatus, int rValue,int* init,int **data){
    int i=0,j=0,k=0,lineNum=0,check=0;
    int processNum=0, numOfThreads=0;
    int threadNum=0, arrivalTime=0, CPUBurst=0;
    int totalCPUTime=0,totalIOTime=0;
    int threadCPUTime=0,threadIOTime=0;
    int numIO=0,numCPU=0;
    float averageTime=0, CPUUsage=100;
    int turnaroundTime=0,finishTime=0;
    Thread* list= (Thread*)malloc(sizeof(Thread));
    Thread* temp= (Thread*)malloc(sizeof(Thread));
    
    /*init[0] is the numer of processes*/
    for(i=0;i<init[0];i++){
        processNum=data[lineNum][0];
        numOfThreads=data[lineNum][1];
        lineNum++;
        for(j=0;j<numOfThreads;j++){
            threadNum=data[lineNum][0];
            arrivalTime=data[lineNum][1];
            CPUBurst=data[lineNum][2]; 
            threadCPUTime=0;
            threadIOTime=0;
            for(k=0;k<CPUBurst;k++){
                lineNum++;
                threadCPUTime=threadCPUTime+data[lineNum][1];
                    numCPU++;
                if(data[lineNum][2]!=0)
                    numIO++;
                threadIOTime=threadIOTime+data[lineNum][2];
            }
            totalCPUTime=totalCPUTime+threadCPUTime;
            totalIOTime=totalIOTime+threadIOTime;
            turnaroundTime=threadIOTime + threadCPUTime +arrivalTime;
            finishTime=turnaroundTime+arrivalTime;
            /*thread creation*/
            if(check==0)
                list=createThread(processNum,threadNum,arrivalTime,threadCPUTime,threadIOTime,turnaroundTime,finishTime);
            else{
                temp=createThread(processNum,threadNum,arrivalTime,threadCPUTime,threadIOTime,turnaroundTime,finishTime);
                list=addList(list,temp);
            }
            check=1;
            lineNum++;
        }
        
    }
    averageTime = ((totalCPUTime+totalIOTime)/(numCPU+numIO));
    if(rStatus==0){
        printf("FCFS Scheduling\n\n");
    }
    else{
        printf("Round Robin Scheduling(quatum = %d time units)\n\n", rValue);
    }
    printf("Total Time required is %d time units\nAverage Turnaround Time is %.2f time units\nCPU Utilization is %.0f%%\n\n",(totalCPUTime+totalIOTime),averageTime,CPUUsage);
    printList(list,init[1],init[2],dStatus,vStatus);
    /*
    while(list->next!=NULL)
    {
        temp=list;
        list=list->next;
        free(temp);
    }*/
    free(temp);
    free(list);
}

Thread*  addList(Thread*  list,Thread*  temp)
{
    Thread*  theList;
    theList = (Thread*)malloc(sizeof(Thread));
    theList = list;
    while(theList->next!=NULL)
    {
        theList=theList->next;
    }
    theList->next = temp;
    temp->next=NULL;
    return list;
}

void printList(Thread*  t,int threadSwitch, int processSwitch, int detail,int verbose)
{
    int p1=t->processNum;
    int p2=0;
    int time=0;
    int check=0;
   
    if(detail!=1&&verbose!=1)
    {
        return;
    }
   
    while(t != NULL)
    {
       if(verbose==1)
       {
           if(p1!=p2)
           {
               printf("At time %d: Thread %d of Process %d moves from new to ready\n",time,t->threadNum,t->processNum);
               p2=p1;
               if(check==0){
                   time=time-processSwitch-t->threadCPUTime;;
                   check=1;
               }
               time = time + processSwitch+t->threadCPUTime;
           }
           printf("At time %d: Thread %d of Process %d moves from ready to running\n",time,t->threadNum,t->processNum);
           time= time + threadSwitch+t->threadIOTime;
           p1 = t->processNum;
       }
       if(detail==1)
       {
           printf("Thread %d of Process: %d\n arrival time: %d\n service time: %d units, IO time: %d units, turnaround time: %d units, finish time: %d units\n\n",t->threadNum,t->processNum,t->arrivalTime,t->threadCPUTime,t->threadIOTime,t->turnaroundTime,t->finishTime);
       }
       t = t->next;
    }
    return;
}



Thread*  createThread(int processNum,int threadNum,int arrivalTime,int threadCPUTime,int threadIOTime,int turnaroundTime,int finishTime)
{
    Thread*  t;
    t = (Thread*)malloc(sizeof(Thread));
    t->processNum=processNum;
    t->threadNum=threadNum;
    t->arrivalTime=arrivalTime;;
    t->threadCPUTime=threadCPUTime;
    t->threadIOTime=threadIOTime;
    t->turnaroundTime=turnaroundTime;
    t->finishTime=finishTime;
    t->next=NULL;
    return t;
}