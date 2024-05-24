#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>

#define HEIGHT  25  // Altura en caracteres de la pizarra
#define WIDTH   25  // Ancho en caracteres de la pizarra

struct canvas {
    char canvas[HEIGHT*WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void create_canvas(const char *name) {
    umask(0000);
    canvas_t cs;
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error al crear el canvas");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, sizeof(cs)) == -1) {
        perror("Error al configurar el tamaño del canvas");
        close(fd);
        exit(EXIT_FAILURE);
    }
    canvas_t *address = mmap(0, sizeof(cs), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (address == MAP_FAILED) {
        perror("Error al mapear el canvas");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void print_canvas(const char *name) {
    int i, j;
    canvas_t canvas;
    int fd = shm_open(name, O_RDONLY, 0666);
    if (fd == -1) {
        perror("Error al abrir el canvas");
        exit(EXIT_FAILURE);
    }
    canvas_t *address = mmap(0, sizeof(canvas), PROT_READ, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("Error al mapear el canvas");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (address->canvas[(HEIGHT * i) + j] == '\0') {
                printf(" ");
            } else {
                printf("%c", address->canvas[(HEIGHT * i) + j]);
            }
        }
        printf("\n");
    }

    if (munmap(address, sizeof(canvas)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void modify_canvas(char *value, char *name, int x, int y, char mode) {
    int i = 0;
    canvas_t cs;
    int fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    canvas_t *address = mmap(NULL, sizeof(cs), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (mode == 'h') {
        strncpy(&(address->canvas[(HEIGHT * x) + y]) , value, strlen(value));
    } else {
        int length = strlen(value);
        for (i = 0; i < length; i++) {
            char character = value[i];
            char *pointer = &character;
            strncpy(&(address->canvas[(x + i) * HEIGHT + y]) , pointer, strlen(value));
        }
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void delete_canvas(char *name) {
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
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
            if (argc != 7) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            if (argv[6][0] != 'v' && argv[6][0] != 'h') {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Escribe %s en el canvas %s en la posición (%d, %d) de forma %s.\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]), argv[6]);
            modify_canvas(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), argv[6][0]);
            break;
        case 'p':
            if (argc < 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Imprime canvas.\n");
            print_canvas(argv[2]);
            break;
        case 'c':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Crea canvas.\n");
            create_canvas(argv[2]);
            break;
        case 'd':
            if (argc != 3) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            printf("Borra canvas.\n");
            delete_canvas(argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido\n");
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}