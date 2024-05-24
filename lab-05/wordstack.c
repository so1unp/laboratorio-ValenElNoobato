#include <stdio.h>
#include <stdlib.h>

#define ITEMS       15
#define MAX_WORD    50

struct wordstack {
    int free;
    int items;
    int max_word;
    pthread_mutex_t mutex;
    sem_t full;
    sem_t empty;
    char heap[ITEMS][MAX_WORD];
};

typedef struct wordstack wordstack_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w pila palabra: agrega palabra en la pila\n");
    fprintf(stderr, "\t-r pila pos: elimina la palabra de la pila.\n");
    fprintf(stderr, "\t-p pila: imprime la pila de palabras.\n");
    fprintf(stderr, "\t-c pila: crea una zona de memoria compartida con el nombre indicado donde almacena la pila.\n");
    fprintf(stderr, "\t-d pila: elimina la pila indicada.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch(option) {
        case 'w':
            break;
        case 'r':
            break;
        case 'p':
            break;
        case 'c':
            break;
        case 'd':
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "-%c: opción desconocida.\n", option);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
