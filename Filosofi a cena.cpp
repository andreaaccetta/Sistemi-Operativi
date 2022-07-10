#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>
 
void test(int);
void prendiBacchette(int);
void lasciaBacchette(int);
void* agisci(void*);


#define N 5
#define PENSA 2
#define FAME 1
#define MANGIA 0
#define SINISTRA (phnum + 4) % N
#define DESTRA (phnum + 1) % N
 
int stato[N];
 
sem_t mutex;
sem_t bacchette[N];
 
int main(){
 
    printf("+------------------------------------------------------+\n");
    printf("+             PROBLEMA DEI FILOSOFI A CENA             +\n");
    printf("+------------------------------------------------------+\n");
    int n[N];
    pthread_t filosofi[N];
 
    // initialize the semaphores
    sem_init(&mutex, 0, 1);
 
    for (int i = 0; i < N; i++)
 
        sem_init(&bacchette[i], 0, 0);
 
    for (int i = 0; i < N; i++) {
 
        n[i] = i;
        pthread_create(&filosofi[i], NULL,
                       agisci, &n[i]);
    }
 
    for (int i = 0; i < N; i++){
 
        pthread_join(filosofi[i], NULL);
    }
}

void test(int phnum)
{
    if (stato[phnum] == FAME
        && stato[SINISTRA] != MANGIA
        && stato[DESTRA] != MANGIA) {
        
        stato[phnum] = MANGIA;
        sleep(2);
        printf("Il filosofo %d sta mangiando\n", phnum + 1);
 
        
        sem_post(&bacchette[phnum]);
    }
}

void* agisci(void* num)
{
 
    while (true) {
 
        int* i = (int*)num;
        prendiBacchette(*i);
        sleep(0);
        lasciaBacchette(*i);
    }
}
 
void prendiBacchette(int phnum)
{
 
    sem_wait(&mutex);
 
    
    stato[phnum] = FAME;

    test(phnum);
 
    sem_post(&mutex);
    
    sem_wait(&bacchette[phnum]);

}
 
void lasciaBacchette(int phnum)
{
 
    sem_wait(&mutex);
 
    // state that thinking
    stato[phnum] = PENSA;

    printf("Il filosofo %d sta pensando\n", phnum + 1);
    sleep(2);
    test(SINISTRA);
    test(DESTRA);
 
    sem_post(&mutex);
}
