#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <string.h>      /* String Functions */

typedef struct Pro
{
    char pid;
    int timeIn;
    int pSize;
    int numOfTimesIn;
    struct Pro* next;
} Processes;

void addToList(Processes* head, char pid, int pSize);
void fit(Processes* head, int proTotal);
int  checkHoles(int* mem);

int main(int argn, char** argv){
    int totalPro=0,check=0;
    FILE * inFile;
    char * token;
    char outline[255];
    Processes* head = malloc(sizeof(Processes));
    
    if(head==NULL){
        return 1;
    }
    
    inFile=fopen(argv[argn-1],"r+");
    if(inFile==NULL){
        printf("Error opening file.\n");
        free(head);
        return 1;
    }
    
    /*parsing file*/
    while (!feof(inFile)){
        if(check==0){
            fgets(outline,255,inFile);
            token=strtok(outline, " \t\r\a\n");
            head->pid=token[0];
            token=strtok(NULL, " \t\r\a\n");
            head->pSize=atoi(token);
            head->timeIn=0;
            head->numOfTimesIn=0;
            head->next=NULL;
            check++;
        }
        else{
            fgets(outline,255,inFile);
            token=strtok(outline, " \t\r\a\n");
            addToList(head,token[0],atoi(strtok(NULL, " \t\r\a\n")));
        }
        totalPro++;
    }
    fit(head,totalPro);
    free(head);
    fclose(inFile);
    return 0;
}

void fit(Processes* head, int proTotal){
    Processes* temp = NULL;
    int fit=0,next=0,nextCheck=0;
    int mem[128];
    
    for(fit=0;fit<4;fit++){
        temp=head;
        int time=1,i=0,j=0,k=0,d=0,c=0,w=0,completedPro=0,buff=0,proAdd=0,load=0,proNum=0, holes=0,check=0;
        float memUsage=0,cumMem=0,div =0,avgPro=0, avgHoles=0;
        int memLeft=128;
        while(temp!=NULL){
            /*Clears memory*/
            for(i=0;i<128;i++)
                mem[i]=0;
            temp->timeIn=0;
            temp->numOfTimesIn=0;
            temp=temp->next;
        }
        if(fit==0)        
            printf("First Fit:\n");
        else if(fit==1)
            printf("Best Fit:\n");
        else if(fit==2)
            printf("Worst Fit:\n");
        else if(fit==3)
            printf("Next Fit:\n");
            
        while(completedPro<proTotal){
            temp = head;
            completedPro=0;
            while(temp!=NULL){
                /*checks to see if the process has ran through 3 times*/
                j=0;
                i=0;
                if(temp->numOfTimesIn<3){
                    proAdd=0;
                    holes=0;
                    check=0;
                    nextCheck=0;
                    do{ /*This loop reoccurs if the process couldn't be added and then resulted in a delete*/
                        k=0;
                        do{ /*This loop reoccurs if the first space was too small and there are more left to check*/
                            if(fit==3&&nextCheck==0){
                                if(next==128)
                                    next=0;
                                j=next;
                            }
                            else{
                                j=k;
                                nextCheck=0;
                            }
                            /*used to find the first empty location in memory*/
                            
                            while(j<128){
                                if(mem[j]==0)
                                    break;
                                j++;
                            }
                            /*Checks size of empty location*/
                            k=j;
                            while(k<128){
                                if(mem[k]!=0)
                                    break;
                                k++;
                            }
                            
                            if(fit==0){
                                c=k;
                                d=j;
                            }
                            else if(fit==1){
                                if((k-j)>=temp->pSize&&((k-j)<=(c-d)||check==0)){
                                    c=k;
                                    d=j;
                                    check=1;
                                }
                            }
                            else if(fit==2){
                                if((k-j)>=temp->pSize&&((k-j)>=(c-d)||check==0)){
                                    c=k;
                                    d=j;
                                    check=1;
                                }
                            }
                            else if(fit==3){
                                next=j;
                                d=j;
                                c=k;
                            }
                            
                            /*Verifies the size of the empty location is bigger than or equal to the process*/
                            if((c-d)>=temp->pSize&&((k==128||j==128)||fit==0||fit==3)){
                                load++;
                                temp->timeIn=load;
                                proNum++;
                                buff=temp->pSize;
                                memLeft-=buff;
                                temp->numOfTimesIn+=1;
                                div=temp->pSize;
                                memUsage=(div/128)*100;
                                cumMem=cumMem+memUsage;
                                /*Used to fill the empty location in memory*/
                                for(i=0;i<temp->pSize;i++){
                                    mem[i+d]=temp->timeIn;
                                    if(fit==3){
                                        next=i+d+1;
                                        if(next>=128)
                                            next=0;
                                    }
                                    nextCheck=0;
                                }
                                /*Calculates number of holes*/
                                holes=checkHoles(mem);
                                /*Sets # of holes to zero if the memory is full*/
                                if((memLeft-temp->pSize)==0)
                                    holes=0;
                                printf("pid %c loaded, #processes = %d, #holes = %d, %%memusage = %.2f, cumulative %%mem = %.2f\n",temp->pid,proNum,holes,memUsage,cumMem/load);
                                /*Variables that holdthe total amount through every process add*/
                                avgPro=avgPro+proNum;
                                avgHoles=avgHoles+holes;
                                /*process is successfully added so now proAdd=1*/
                                proAdd=1;
                                break;
                            }
                            else{
                                if(fit==3)
                                    nextCheck=1;
                            }
                        }while(k<128||j<128);
                        /*Deletion of the earliest added process from memory*/
                        if(proAdd==0){
                            
                            w=0;
                            for(i=0;i<128;i++){
                                if(mem[i]==time){
                                    mem[i]=0;
                                    w++;
                                }
                            }
                            memLeft+=w;
                            time++;
                            proNum--;
                        }
                    }while(proAdd==0);
                }
                else /*Accounts for the process being completed*/
                    completedPro++;
                temp = temp->next;
            }
        }
        printf("Total loads = %d, average #processes = %.2f, average #holes = %.2f, cummulative %%mem = %.2f\n\n",load,avgPro/load,avgHoles/load,cumMem/load);
    }
    free(temp);
}

int checkHoles(int* mem){
    int a=0, b=0, i=0;
    int holes=0;
    if(mem[0]!=0)
        b=1;
    for(i=0;i<128;i++){
        if(mem[i]==0){
            a=1;
        }
        else{
            a=0;
        }
        if(b!=a&&a==1){
            holes++;
        }
        b=a;
    }
    return holes;
}

void addToList(Processes* head, char pid, int pSize){
    Processes* temp = head;
    while(temp->next!=NULL){
        temp = temp->next;
    }
    temp->next=malloc(sizeof(Processes));
    temp->next->pid=pid;
    temp->next->pSize=pSize;
    temp->next->timeIn=0;
    temp->next->numOfTimesIn=0;
    temp->next->next=NULL;
}
