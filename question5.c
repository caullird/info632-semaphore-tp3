#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.c"

/* Global variables */
sem_t queue_access, consom;
struct Queue* queue;
int k = 0;

/* Functions */

void action_deposer(int nb){
    printf("A depose: \t%d\n",nb);
    enqueue(queue, nb);
}

int action_retirer(){
    int nb = dequeue(queue);
    printf("retirer: \t%d\n",nb);
    return nb;
}

int _action_produire(int value){
    value = value + 1;
    printf("A produit: %d\n", nb);
    return value;
}

void action_consommer(int value)
{
    const struct timespec attente = {rand() % 1, rand() % 100};
    nanosleep(&attente, NULL);
    int i = value * -1;
    printf("A consommer : \t%d\n", i);
}


/* Threads functions */

void *consommation()
{
    for (int j = 0; j < 10; j++){
        sem_wait(&consom);
        sem_wait(&queue_access);
        int nb = action_retirer();
        sem_post(&queue_access);
        action_consommer(nb);
    }
}


void *production(void *key)
{
    for (int j = 0; j < 10; j++){
        int nb = produire((*(int*)key + 1)*10+j);
        sem_wait(&queue_access);
        action_deposer(nb);
        sem_post(&consom);
        sem_post(&queue_access);
    }
}


void main () {
    pthread_t prod_t[2], cons_t[2];

    queue = createQueue(2);

    /* semaphore sync should be initialized by 0 */
    if (sem_init(&queue_access, 0, 1) == -1) {
        perror("Could not initialize prod semaphore");
        exit(2);
    }
    if (sem_init(&consom, 0, 0) == -1) {
        perror("Could not initialize cons semaphore");
        exit(2);
    }

    for (int key = 0; key < 2; key++)
    {
        if (pthread_create(&prod_t[i], NULL, production, (void *)&i) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
        if (pthread_create(&cons_t[i], NULL, consommation, (void *)&i) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
    }
    
    for (int key = 0; key < 2; key++)
    {
        pthread_join(prod_t[i], NULL);
        pthread_join(cons_t[i], NULL);
    }
    
    sem_destroy(&queue_access);
    sem_destroy(&cons);

    free(queue);

    exit(0);
}