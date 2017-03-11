#define _GNU_SOURCE
#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <string.h>      /* String Functions */
#include <signal.h>      /* Used for signals */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */

char **parseLine();
int sh_launch(char **args,int flag,int pos, int argPos);
int sh_execute(char **args);
void handler(int sig);

int main ( int argc, char **argv) {
   
    int status;
    
    do{
        printf("> ");
        status = sh_execute(parseLine());
    }while(status);/*Checks if status equals 1 or 0. If 0 exit.*/
    
    return 0;
}

char **parseLine(){
    char *line=NULL;
    /*The variable that holds the size of the buffer*/
    size_t length =0;
    int i = 0;
    char *token;
    char **tokens;
    /*mallocs for the amount of characters in the line*/
    tokens= malloc(getline(&line,&length,stdin) * sizeof(char*));
    token = strtok(line, " \t\r\n\a");
    while (token!=NULL){
        tokens[i]=malloc(sizeof(char)*strlen(token)+1);
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \t\r\n\a");
        tokens[i]=NULL;
    }
    return tokens;
}
/*The launch function handles the functionality of the commands inputted into the shell*/
int sh_launch(char **args, int flag, int pos, int argPos){
    pid_t pid;
    int status, i, pipeBusy=0;
    int pd[2];
    char **tempArgs =malloc(sizeof(char*)*argPos+1);
    /*Makes a duplicate of args until it reaches one of the characters <,>,|, or &*/
    for(i=0;i<pos;i++){
        tempArgs[i]=malloc(sizeof(char)*strlen(args[i])+1);
        strcpy(tempArgs[i],args[i]);
    }
    /*Piping*/
    pipe(pd);
    /*forking*/
    pid = fork();
    if(pid==0){
        /*Child process*/
        if(flag==1){
            if(strcmp(args[pos],"&")==0){
                tempArgs[pos]=NULL;
                execvp(tempArgs[0], tempArgs);
            }
            else if(strcmp(args[pos],">")==0){
                /*freopen opens the argument(file) to the right of > to write to*/
                FILE * file;
                file = freopen(args[pos+1],"w",stdout);
                execvp(tempArgs[0],tempArgs);
                fclose(file);
            }
            else if(strcmp(args[pos],"<")==0){
                /*freopen opens the argument(file) to the right of < to read from*/
                FILE * file;
                file = freopen(args[pos+1],"r",stdin);
                fclose(file);
                tempArgs[pos]=malloc(sizeof(char)*strlen(args[pos+1])+1);
                /*adds the files output into the temp array*/
                strcpy(tempArgs[pos],args[pos+1]);
                execvp(tempArgs[0],tempArgs);
            }
            else if(strcmp(args[pos],"|")==0){
                dup2(pd[0],0);
                /*the input and output are closed after use*/
                close(pd[0]);
                close(pd[1]);
                /*the pipe accepts 3 arguments after | decending down*/
                execlp(args[pos+1],args[pos+1],args[pos+2],args[pos+3], NULL);
                execlp(args[pos+1],args[pos+1],args[pos+1], NULL);
                execlp(args[pos+1],args[pos+1], NULL);
                perror("exec");
            }
        }
        else if(execvp(args[0], args)==-1){
            printf("shell: Command not found\n");
        }
        exit(1);
    }
    else if(pid<0){
        /*Error forking*/
        printf("shell: Error forking\n");
        exit(1);
    }
    else{
        if(strcmp(args[pos],"|")==0&&pipeBusy==0){
            dup2(pd[1],1);
            /*the input and output are closed after use*/
            close(pd[0]);
            close(pd[1]);
            /*the pipe accepts 3 arguments before | decending down*/
            execlp(tempArgs[0],tempArgs[0],tempArgs[1],tempArgs[2],NULL);
            execlp(tempArgs[0],tempArgs[0],tempArgs[1],NULL);
            execlp(tempArgs[0],tempArgs[0],NULL);
            perror("exec");
            pipeBusy++;
        }
        if(strcmp(args[pos],"&")==0){
            printf("[1] %d\n",pid);
            /*Calls the handler function to wait for pid and display the output*/
            signal(SIGCHLD, handler);
        }
        else{
            do{
                /*waits for the process to exit or kill itself*/
                waitpid(pid,&status,WUNTRACED);
            }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
        }
    }
    for(i=0;i<=pos;i++){ /*Free's the allocated duplicate of args*/
        free(tempArgs[i]);
    }
    free(tempArgs);
    return 1;
}
/*The execution function used to check what arguments are coming into the shell and return based on the situation*/
int sh_execute(char **args){
    int pos=0, argPos=0, flag=0;
    /*Checks for blank line's and exit/kill*/
    if(args[0]==NULL){
        printf("shell: No command entered\n");
        return 1;
    }
    /*returns an error if the first character is | or &*/
    else if(strcmp(args[argPos],"&")==0||strcmp(args[argPos],"|")==0){
        printf("shell: must include argument before: %s\n",args[argPos]);
        return 1;
    }
    /*Exits the shell if exit or kill are typed in*/
    else if(strcmp(args[0],"exit")==0||strcmp(args[0],"kill")==0){
        printf("logout\n\n[Process completed]");
        return 0;
    }
    /*Checks for &,<,>,| inside the input args*/
    while(args[argPos+1]!=NULL){
        argPos++;
        if(strcmp(args[argPos],"&")==0||strcmp(args[argPos],"<")==0||strcmp(args[argPos],">")==0||strcmp(args[argPos],"|")==0){
            pos=argPos;
            flag=1;
        }
    } 
    return sh_launch(args, flag, pos, argPos);
}

/*The handler function used for &*/
void handler(int sig){
    pid_t pid;
    pid=wait(NULL);
    printf("Pid %d exited.\n",pid);
    signal(SIGCHLD, SIG_IGN);
}