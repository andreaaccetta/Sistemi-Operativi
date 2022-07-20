#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>
 
void test(int);
void acquisisciFile(int);
void liberaFile(int);
void* agisci(void*);


#define N 5
#define ELABORA 2
#define ATTENDI 1
#define ELABORA_FILE 0
#define SINISTRA (phnum + 4) % N
#define DESTRA (phnum + 1) % N
 
int stato[N];
 
sem_t mutex;
sem_t file[N];
 
int main(){
 
    printf("+-----------------------------------------------------+\n");
    printf("+   PROBLEMA DEI FILOSOFI A CENA APPLICATO AI FILE    +\n");
    printf("+-----------------------------------------------------+\n");
    int n[N];
    pthread_t processi[N];
 
    
    sem_init(&mutex, 0, 1);
 
    for (int i = 0; i < N; i++)
 
        sem_init(&file[i], 0, 0);
 
    for (int i = 0; i < N; i++) {
 
        n[i] = i;
        pthread_create(&processi[i], NULL,
                       agisci, &n[i]);
    }
 
    for (int i = 0; i < N; i++){
 
        pthread_join(processi[i], NULL);
    }
}

void test(int phnum)
{
    if (stato[phnum] == ATTENDI
        && stato[SINISTRA] != ELABORA_FILE
        && stato[DESTRA] != ELABORA_FILE) {
        
        stato[phnum] = ELABORA_FILE;
        sleep(2);
        printf("Il processo %d sta effettuando operazioni sui file\n", phnum + 1);
 
        
        sem_post(&file[phnum]);
    }
}

void* agisci(void* num)
{
 
    while (true) {
 
        int* i = (int*)num;
        acquisisciFile(*i);
        sleep(0);
        liberaFile(*i);
    }
}
 
void acquisisciFile(int phnum)
{
 
    sem_wait(&mutex);
 
    
    stato[phnum] = ATTENDI;

    test(phnum);
 
    sem_post(&mutex);
    
    sem_wait(&file[phnum]);

}
 
void liberaFile(int phnum)
{
 
    sem_wait(&mutex);
 
    
    stato[phnum] = ELABORA;

    printf("Il processo %d sta effettuando elaborazioni secondarie\n", phnum + 1);
    sleep(2);
    test(SINISTRA);
    test(DESTRA);
 
    sem_post(&mutex);
}
