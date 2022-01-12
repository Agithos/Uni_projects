#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "ask3_help.h"

#define BFSIZE 64

char* outputString[3] = {   "eimai o KOKKINOS power ranger!\t\tA:",     // gia ta outputs apo ta A,B,C Processes
                            "eimai o MPLE power ranger!   \t\tB:",
                            "eimai o MAVROS power ranger! \t\tC:"};

union semun semUnion;
struct sembuf sop;

int main(int argc, char* argv[])
{
    int c_pid[3] = {0,0,0};                                 // array me child_pIDs
    int semGroup;                                           // semaphore group
    unsigned short initArray[2] = {0,0};                    // gia SETALL initial values (meta kanw kai ena wait gia na arxizei apo -1)
    char* path = "output.txt";                              // output file path
    char outputSpecific[BFSIZE];                            // buffer gia write
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
        perror("Semaphore creation error");
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
                semWait(semGroup, i-1, &sop);               // Initial Value semaphore -1 gia ton kathena
            }
            for (int j=0; j<100-i; j++)
            {
                if (i)                                                      
                {
                    semWait(semGroup, i-1, &sop);           // gia thn B,C process exoume semWait
                }
                sprintf(outputSpecific, "%s%d\n", outputString[i],j+1);
                printOutput(outputSpecific, fd, strlen(outputSpecific));
                memset(outputSpecific, 0, strlen(outputSpecific));
                if (i<2)                                                   
                {
                    semSignal(semGroup, i, &sop);           // gia thn A,B process exoume semSignal
                }
            }
            break;                                          // break gia na mi sinexisei kai kanei fork
        }
    }
   
    if (c_pid[2] != 0)          // parent wait kai IPC_RMID
    {
        wait(0);
        wait(0);
        wait(0);
        if (semctl(semGroup, 0,IPC_RMID) < 0)
        {
            perror("Remove Semaphore error");
            exit(-1);
        }
    }
    if ( close(fd) != 0)
    {
        perror("Closing error");
        exit(-1);
    }
    return 0;
}