/* Used to create thread struct of arguments 
http://man7.org/linux/man-pages/man3/pthread_create.3.html*/
/*Used to to create threads and loops
https://docs.oracle.com/cd/E19205-01/820-0619/gepji/index.html*/    
#define _BSD_SOURCE 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*This struct holds the argumenst passed to eat thread*/
typedef struct ARG{
    int id;
    int numPhilo;
    int numEat;
}Arguments;

void *philosopher (void *id);

pthread_mutex_t chopstick[1000];
pthread_t philo[1000];
	
int main (int argn,char **argv)
{
    int i=0,timesEatting=0,numPhilo=0;
    Arguments* args;
    
    /*Checks to see if all argumnets are on the command line*/
    if (argn < 3||argn > 3){
        printf("Wrong amount of arguments. Try again with Program name then number of philosophers then how many times each of them eat.\n");
        return 1;
    }
    /*callocing for the stucts of arguments to be passed to the threads*/
    args=calloc(atoi(argv[1]),sizeof(Arguments));
    
    if(args==NULL){
        printf("error in making struct for threads\n");
        return 1;
    }
    
    numPhilo=atoi(argv[1]);
    timesEatting=atoi(argv[2]);
    
    /*Initilizes the chopsticks*/
    for (i = 0; i < numPhilo; i++)
        pthread_mutex_init (&chopstick[i], NULL);
    /*Creates each thread with the variables being passed through args[i]*/
    for (i = 0; i < numPhilo; i++){
        args[i].id=i;
        args[i].numPhilo=numPhilo;
        args[i].numEat=timesEatting;
        pthread_create (&philo[i], NULL, &philosopher, &args[i]);
    }
    for (i = 0; i < numPhilo; i++)
        pthread_join (philo[i], NULL);
    
    free(args);
    return 0;
}
void *philosopher (void *num)
{
    Arguments* args = num;
    int id=0, numPhilo=0;
    int timesNeededToEat=0, left_chopstick=0, right_chopstick=0;
    id = args->id;
    timesNeededToEat=args->numEat;
    numPhilo=args->numPhilo;
    
    right_chopstick = id+1;
    left_chopstick = id;
    
    /* When the right chopstick reaches the last philosopher it resets to 0. */
    if (right_chopstick == numPhilo)
        right_chopstick = 0;
    
    /*while loop that runs until this philosopher eats the designated amount of times*/
    while(timesNeededToEat!=0){
        printf ("Philosopher %d thinking\n", (id+1));
        /*Picking up chopsticks*/
        pthread_mutex_lock (&chopstick[right_chopstick]);
        pthread_mutex_lock (&chopstick[left_chopstick]);

        printf ("Philosopher %d eating\n", (id+1));
        usleep (5000-(timesNeededToEat+1));
        /*Putting down chopsticks*/
        pthread_mutex_unlock (&chopstick[left_chopstick]);
        pthread_mutex_unlock (&chopstick[right_chopstick]);
        timesNeededToEat--;
    }
    
    printf ("Philosopher %d is done eating.\n", (id+1));
    return (NULL);
}
