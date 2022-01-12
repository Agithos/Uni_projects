#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

void semWait(int semId, int semIndex, struct sembuf *sop)
{
    sop->sem_num = semIndex;
    sop->sem_op = -1;
    sop->sem_flg = 0;
    if (semop(semId, sop, 1) != 0)
    {
        perror("Semaphore error");
    } 
}

void semSignal(int semId, int semIndex, struct sembuf *sop)
{
    sop->sem_num = semIndex;
    sop->sem_op = 1;
    sop->sem_flg = 0;
    if (semop(semId, sop, 1) != 0)
    {
        perror("Semaphore error");
    }
}

// Output se file kai se stdin
void printOutput(char* output, int fd, int strSize)
{
    if (write (fd, output, strSize) != strSize)
    {
        perror("Write Error");
        exit(-1);
    }
    printf("%s", output);
    fflush(NULL);
}