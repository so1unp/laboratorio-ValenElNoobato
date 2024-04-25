#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variable global que incrementan los hilos.
static long glob = 0;

//void increment_glob(int loops)
void *increment_glob(void *p)
{
    long loops = (long) p;
    int loc, j;

    // incrementa glob
    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int loops;

    // Controla numero de argumentos.
    if (argc != 2) {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // Verifica argumentos.
    if (loops < 1) {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }

    int error;
    pthread_t id1, id2;

    error = pthread_create(&id1, NULL, increment_glob, (void *) (long) loops);
    if (error != 0) { perror("Error: thread creation.\n"); }

    error = pthread_create(&id2, NULL, increment_glob, (void *) (long) loops);
    if (error != 0) { perror("Error: thread creation.\n"); }

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    printf("%ld\n", glob);

    exit(EXIT_SUCCESS);
}
