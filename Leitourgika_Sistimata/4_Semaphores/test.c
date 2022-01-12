// theloume kai se ena arxeio


#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
union semun{
    int val;
    struct semid_ds *buf;
    short *array;
    struct seminfo *__buf;
};

union semun g;
short v[2] = {5,3};
struct sembuf sop;

void semWait(int semid, int index)
{
    sop.sem_num = index;
    sop.sem_flg = 0;
    sop.sem_op = -7;
    if (semop(semid, &sop, 1) != 0)
    {
        printf("Error");
    }
}
void semSignal(int semid, int index)
{
    sop.sem_num = index;
    sop.sem_flg = 0;
    sop.sem_op = 1;
    if (semop(semid, &sop, 1) != 0)
    {
        printf("Error");
    }
}

/*
  check = semctl(semGroup, i-1, GETVAL);
                    if(check < 1)
                    {
                        printf("--Waiting %d--\n", i);
                    }
/*


int main(int argc, char argv[])
{


    union semun z;
    z.array = v;
    
    int x,y;
    int cid;
    
   int semGroup = semget(IPC_PRIVATE, 2, IPC_CREAT|0666);
   z.val = -1;
   semctl(semGroup, 0, SETVAL, z);
   z.val = 1;
   semctl(semGroup, 1, SETVAL, z);
   
//    semctl(semGroup, 0, SETALL, z);
//     cid = fork();
//     if (cid == 0)
//     {
//         sleep(3);
//          x = semctl(semGroup, 0, GETVAL);
//             y = semctl(semGroup, 1, GETVAL);
//             printf("%d , %d", x,y);
//         sleep(3);
//         semSignal(semGroup, 0);
//          x = semctl(semGroup, 0, GETVAL);
//             y = semctl(semGroup, 1, GETVAL);
//             printf("%d , %d", x,y);

//          sleep(3);
//         semSignal(semGroup, 0);
//          x = semctl(semGroup, 0, GETVAL);
//             y = semctl(semGroup, 1, GETVAL);
//             printf("%d , %d", x,y);

//     }
//     else
//     {
//         semWait(semGroup, 0);
//         printf("Telos");
//     }

   
   semctl(semGroup, 0, GETVAL);
   x = semctl(semGroup, 0, GETVAL);
   y = semctl(semGroup, 1, GETVAL);

   printf("%d , %d", x,y);



    return 0;
}

