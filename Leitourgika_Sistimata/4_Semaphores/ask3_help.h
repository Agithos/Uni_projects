
union semun                     // Union gia merika commands ths semctl()
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void semWait(int semId, int semIndex, struct sembuf *sop);
void semSignal(int semId, int semIndex, struct sembuf *sop);
void printOutput(char* output, int fd, int strSize);            // print se file kai se stdout
