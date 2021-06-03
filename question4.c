#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <string.h>

char depot[255];
char chain_string[255];

int count = 0;
sem_t cons;
sem_t prod;

void action_produire(char* chain_string){
    count++;
    printf("Message : %d", count);
}

void action_commencer(char* chain_string){
    char *chain = chain_string;
    chain_string += toupper(*chain);
    printf("\n");
}

void action_deposer(char* chain_string){
    strcpy(depot, chain_string);
    
}

void action_retirer(char* chain_string) {
    strcpy(chain_string, depot);
    strcpy(depot, "");
}

void *consommateur(){
    for (int value = 0; value < 20; value++){
            sem_wait(&cons);
            action_commencer(chain_string);
            action_retirer(chain_string);
            sem_post(&prod);
    }
}

void *producteur(){
    for (int value = 0; value < 20; value++){
            sem_wait(&prod);
            action_produire(chain_string);
            action_deposer(chain_string);
            sem_post(&cons);
    }
}

void main () {
    
    pthread_t prod_thread, cons_thread;
    
    if (sem_init(&prod, 0, 1) == -1) {
        perror("Could not initialize cons semaphore");
        exit(2);
    }
    if (sem_init(&cons, 1, 0) == -1) {
        perror("Could not initialize prod semaphore");
        exit(2);
    }
    
    if (pthread_create(&prod_thread, NULL, producteur, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    
    if (pthread_create(&cons_thread, NULL, consommateur, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    
    
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    sem_destroy(&prod);
    sem_destroy(&cons);
    
    
    exit(0);
}