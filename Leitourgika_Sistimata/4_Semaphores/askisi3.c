#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "ask3_help.h"

#define BFSIZE 64

char* outputString[3] = {   "eimai o KOKKINOS power ranger!\t\tA:",
                            "eimai o MPLE power ranger!\t\tB:",
                            "eimai o MAVROS power ranger!\t\tC:"};

union semun semUnion;
struct sembuf sop;

int main(int argc, char* argv[])
{
    int c_pid[3];          // child pid
    int semGroup;
    int check;
    unsigned short initArray[2] = {0,0};          // gia SETVAL
    char* path = "output.txt";
    char outputSpecific[BFSIZE];
    memset(outputSpecific, 0, BFSIZE);

    // Open file
    int fd;
    if ( (fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, 0666)) < 0 )
    {
        perror("File open failed");
        exit(-1);
    }

    // Create semaphores
    if ( (semGroup = semget(IPC_PRIVATE, 2, IPC_CREAT|0666)) == -1) // 0 gia A-B, 1 gia B-C
    {
        perror("Semaphore error");
        exit(-1);
    }
    // Init semaphores
    semUnion.array = initArray;
    semctl(semGroup, 0, SETALL, semUnion);

    // Create Forked Processes
    for (int i=0; i<3; i++)
    {
        // Fork
        if( (c_pid[i] = fork()) < 0)
        {
            perror("Fork error");
            exit(-1);
        } 

        if (c_pid[i] == 0)
        {
            /* CHILDREN */
            if(i)
            {
                semWait(semGroup, i-1, &sop);
            }
            for (int j=0; j<100-i; j++)
            {
                if (i)
                {
                    semWait(semGroup, i-1, &sop);
                }
                sprintf(outputSpecific, "%s%d\n", outputString[i],j+1);
                printOutput(outputSpecific, fd, strlen(outputSpecific));
                memset(outputSpecific, 0, strlen(outputSpecific));
                if (i<2)
                {
                    semSignal(semGroup, i, &sop);
                }
               
            }
            break;              // break gia na mi sinexisei kai kanei fork
        }
    }
    if (c_pid[0] != 0)      // parent wait
    {
        wait(0);
        wait(0);
        wait(0);
    }
    if ( close(fd) != 0)
    {
        perror("Closing error");
        exit(-1);
    }
    return 0;
}