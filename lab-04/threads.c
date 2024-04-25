#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HILOS_MAX 100
int seconds = 0;
pthread_t hilos[HILOS_MAX];

void *threadHandler(void *p) {
    long id = (long) p;

    int num = (rand() % seconds) + 1;

    printf("Hilo %ld : dormire %d\n", id, num);

    sleep((unsigned int) num);

    pthread_exit((void *) (long) num);
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("1° argumento: N° de hilos.\n");
        printf("2° argumento: N° maximo de segundos que duermen los hilos.\n");
        exit(EXIT_FAILURE);
    }

    srand(getpid());

    int i, number = atoi(argv[1]), aux;
    void * aux1;
    seconds = atoi(argv[2]);

    for (i = 0; i < number; i++) {
        pthread_t id;

        aux = pthread_create(&id, NULL, threadHandler, (void *) (long) i);

        if (aux != 0) {
            perror("Error: thread creation.\n");
        }

        hilos[i] = id;
    }

    i = 0;

    for(i = 0; i < number; i++) {  
        pthread_join(hilos[i], &aux1);

        printf("Hilo %d termino en: %ld\n", i, (long) aux1);
    }

    exit(EXIT_SUCCESS);
}

