#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

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

void create_queue(const char *name) {
    umask(0000);
    wordstack_t qe;
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error al crear el queue");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, sizeof(qe)) == -1) {
        perror("Error al configurar el tamaño del queue");
        close(fd);
        exit(EXIT_FAILURE);
    }

    wordstack_t *address = mmap(0, sizeof(qe), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    address->max_word = MAX_WORD;
    address->items = 0;
    address->free = ITEMS;
    pthread_mutex_init(&address->mutex, NULL);
    sem_init(&address->full, 1, 0);
    sem_init(&address->empty, 1, ITEMS);

    if (address == MAP_FAILED) {
        perror("Error al mapear el queue");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void print_queue(const char *name) {
    int i, j;
    wordstack_t qe;
    int fd = shm_open(name, O_RDONLY, 0666);
    if (fd == -1) {
        perror("Error al abrir el queue");
        exit(EXIT_FAILURE);
    }
    wordstack_t *address = mmap(0, sizeof(qe), PROT_READ, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("Error al mapear el queue");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < ITEMS; i++) {
        for (j = 0; j < MAX_WORD; j++) {
            if (address->heap[i][j] == '\0') {
                printf(" ");
            } else {
                printf("%c", address->heap[i][j]);
            }
        }
        printf("|\n");
    }

    if (munmap(address, sizeof(qe)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void modify_canvas(char *value, char *name) {
    int i = 0;
    wordstack_t qe;
    int fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *address = mmap(NULL, sizeof(qe), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_wait(&address->empty);
    pthread_mutex_lock(&address->mutex); 
    for (i = 0; i < strlen(value); i++) {
        address->heap[address->items][i] = value[i];
    }
    address->free--;
    address->items++;
    pthread_mutex_unlock(&address->mutex);
    sem_post(&address->full);

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void delete_queue(char *name) {
    wordstack_t qe;
    int fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *address = mmap(NULL, sizeof(qe), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }   

    pthread_mutex_destroy(&address->mutex);
    sem_destroy(&address->full);
    sem_destroy(&address->empty); 

    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}

void remove_queue(char *name) {
    int i = 0;
    wordstack_t qe;
    int fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    wordstack_t *address = mmap(NULL, sizeof(qe), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_wait(&address->full);
    pthread_mutex_lock(&address->mutex);
    for (i = 0; i < address->max_word; i++) {
        address->heap[address->items - 1][i] = putchar('\0');
    }
    address->free++;
    address->items--; 
    pthread_mutex_unlock(&address->mutex);
    sem_post(&address->empty);

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }   
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
            if (argc != 4) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Añade %s en la queue %s.\n", argv[2], argv[3]);
            modify_canvas(argv[2], argv[3]);
            break;
        case 'r':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Se remueve la palabra del tope de la queue %s.\n", argv[2]);
            remove_queue(argv[2]);
            break;
        case 'p':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Imprime queue %s.\n", argv[2]);
            print_queue(argv[2]);
            break;
        case 'c':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Crea la cola.\n");
            create_queue(argv[2]);
            break;
        case 'd':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Borra la cola.\n");
            delete_queue(argv[2]);
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
