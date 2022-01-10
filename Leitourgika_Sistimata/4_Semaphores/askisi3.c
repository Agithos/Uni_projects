#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "semun.h"

void semWait(int semId, int semIndex);
void semSignal(int semId, int semIndex);


char* outputString[3] = {   "eimai o KOKKINOS power ranger!\t\t",           // leipei A:
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


    // Create semaphores
    if ( (semGroup = semget(IPC_PRIVATE, 2, IPC_CREAT|0666)) == -1) // 0 gia A-B, 1 gia B-C
    {
        perror("Semaphore error");
        exit(-1);
    }
    // Init semaphores
    semUnion.array = initArray;
    semctl(semGroup, 0, SETALL, semUnion);

    check = semctl(semGroup, 0, GETVAL);
    printf("First is : %d\n", check);
    check = semctl(semGroup, 1, GETVAL);
    printf("Second is : %d\n", check);

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
                semWait(semGroup, i-1);
            }
            for (int j=0; j<100; j++)
            {
                if (i)
                {
                    check = semctl(semGroup, i-1, GETVAL);
                    if(check < 1)
                    {
                        printf("--Waiting %d--\n", i);
                    }
                    semWait(semGroup, i-1);
                }
                if (i<2)
                {
                    semSignal(semGroup, i);
                }
                printf("%s%d\n", outputString[i],j+1);
                fflush(NULL);
            }
            break;
        }
    }
    check = semctl(semGroup, 0, GETVAL);
    printf("First is : %d\n", check);
    check = semctl(semGroup, 1, GETVAL);
    printf("Second is : %d\n", check);

    return 0;
}

void semWait(int semId,int semIndex)
{
    sop.sem_num = semIndex;
    sop.sem_op = -1;
    sop.sem_flg = 0;
    if (semop(semId, &sop, 1) != 0)
    {
        perror("Semaphore error");
    } 
}

void semSignal(int semId, int semIndex)
{
    sop.sem_num = semIndex;
    sop.sem_op = 1;
    sop.sem_flg = 0;
    if (semop(semId, &sop, 1) != 0)
    {
        perror("Semaphore error");
    }
}