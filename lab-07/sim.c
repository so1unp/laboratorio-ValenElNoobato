#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#define MAX_PROCESS 100  // Numero de maximo de procesos
#define TAM_PROCESS 16
#define TAM_PRIMARY 8
#define TAM_SECONDARY 16

struct process {
    int pid;
    int page[TAM_PROCESS];
    bool created;
};

typedef struct process process_t;

struct memory {
    int process;
    int page;
    int FIFO;
    int LRU;
};

typedef struct memory memory_t;

process_t processes[MAX_PROCESS];
memory_t primary[TAM_PRIMARY];
memory_t secondary[TAM_SECONDARY];
int cont_FIFO = 0;
int cont_LRU = 0;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-f FIFO: reemplazo de paginas de tipo FIFO.\n");
    fprintf(stderr, "\t-l LRU: reemplazo de paginas de tipo LRU.\n");
}

void create_process(int pid) {
    process_t process;
    int i;
    process.pid = pid;
    for(i = 0; i < TAM_PROCESS; i++) {
        process.page[i] = -1;
    }
    process.created = true;
    processes[pid] = process;
}

void modify_process(int pid, int page, char option) {
    int i;
    bool pass = false;
    bool swap = true;
    int pid_memory;
    process_t process = processes[pid];

    // Se corrobora si el proceso ya esta en la memoria principal.
    for(i = 0; i < TAM_PRIMARY; i++) {
        if(primary[i].process == pid && primary[i].page == page) {
            pass = true;
            primary[i].LRU = cont_LRU;
            cont_LRU++;
        }
    }

    // Si el proceso ya se encuentra no se lo ingresa, en caso contrario, ingresa el proceso en la memoria principal.
    if (pass == false) {
        for(i = 0; i < TAM_PRIMARY; i++) {
            if (primary[i].process == -1) {
                primary[i].process = pid;
                primary[i].page = page;
                pid_memory = i;
                swap = false;
                primary[i].FIFO = cont_FIFO;
                cont_FIFO++;
                primary[i].LRU = cont_LRU;
                cont_LRU++;
                break;
            }
        }
        // Si la memoria principal esta llena, entonces se realiza un intercambio, segun como se comenzo el programa.
        if (swap == true) {
            memory_t rem_memory;
            if (option == 'f') {
                // Se busca al proceso con el contador FIFO mas pequeño, para remplazarlo en la memoria secundaria.
                int cont = TAM_PRIMARY;
                for (i = 0; i < TAM_PRIMARY; i++) {
                    if (primary[i].FIFO < cont) {
                        cont = primary[i].FIFO;
                        rem_memory = primary[i];
                    }
                }
                // Se reemplaza en la memoria principal por el proceso recien creado.
                for (i = 0; i < TAM_PRIMARY; i++) {
                    if (primary[i].FIFO == cont) {
                        primary[i].process = pid;
                        primary[i].page = page;
                        primary[i].FIFO = cont_FIFO;
                        cont_FIFO++;
                        break;
                    }
                }
                for (i = 0; i < TAM_SECONDARY; i++) {
                    if (secondary[i].process == -1) {
                        secondary[i].process = rem_memory.process;
                        secondary[i].page = rem_memory.page;
                        secondary[i].FIFO = rem_memory.FIFO;
                        pid_memory = i;
                        break;
                    }
                }
            }
            if (option == 'l') {
                // Se busca al proceso con el contador LRU mas pequeño, para remplazarlo en la memoria secundaria.
                int cont = TAM_PRIMARY;
                for (i = 0; i < TAM_PRIMARY; i++) {
                    if (primary[i].LRU < cont) {
                        cont = primary[i].LRU;
                        rem_memory = primary[i];
                    }
                }
                // Se reemplaza en la memoria principal por el proceso recien creado.
                for (i = 0; i < TAM_PRIMARY; i++) {
                    if (primary[i].LRU == cont) {
                        primary[i].process = pid;
                        primary[i].page = page;
                        primary[i].LRU = cont_LRU;
                        cont_LRU++;
                        break;
                    }
                }
                for (i = 0; i < TAM_SECONDARY; i++) {
                    if (secondary[i].process == -1) {
                        secondary[i].process = rem_memory.process;
                        secondary[i].page = rem_memory.page;
                        secondary[i].LRU = rem_memory.LRU;
                        pid_memory = i;
                        break;
                    }
                }
            }
        }
        process.page[page - 1] = pid_memory;
        processes[pid] = process;
    }  
}

void write_all() {
    int i, j;
    printf("\n");
    // Se leen todos los procesos creados
    for (i = 0; i < MAX_PROCESS; i++) {
        if (processes[i].created == true) {
            process_t process = processes[i];
            printf("Proceso %d: ", process.pid);
            for(j = 0; j < TAM_PROCESS; j++) {
                if (process.page[j] == -1) {
                    printf("- ");
                } else {
                    printf("%d ", process.page[j]);
                }
            }
            printf("\n");
        }
    }

    // Se lee la memoria principal
    printf("Memoria fisica: ");
    for (i = 0; i < TAM_PRIMARY; i++) {
        if(primary[i].process == -1) {
            printf("- ");
        } else {
            printf("%d.%d ", primary[i].process, primary[i].page);
        }
    }
    printf("\n");

    // Se lee la memoria secundaria
    printf("Memoria secundaria: ");
    for (i = 0; i < TAM_SECONDARY; i++) {
        if(secondary[i].process == -1) {
            printf("- ");
        } else {
            printf("%d.%d ", secondary[i].process, secondary[i].page);
        }
    }
    printf("\n");
}

void handle_sigint(int sig) {
    write_all();
    exit(0);
}

int main(int argc, char* argv[])
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

    if (option != 'f' && option != 'l') {
        fprintf(stderr, "Comando desconocido\n");
        exit(EXIT_FAILURE);
    }

    int number1;
    int number2;
    int i;
    int count = 0;

    for (i = 0; i < TAM_PRIMARY; i++) {
        primary[i].process = -1;
    }

    for (i = 0; i < TAM_SECONDARY; i++) {
        secondary[i].process = -1;
    }

    processes->created = false;

    signal(SIGINT, handle_sigint);

    while(1) {
        int num;
        scanf("%d", &num);

        if (count == 0) {
            number1 = num;
            count++;
        } else if (count == 1) {
            if (num > TAM_PROCESS) {
                count = 0;
                printf("Error: number of page excedeed\n");
            } else {
                bool create = true;
                number2 = num;
                count = 0;

                // Se recorre el Array de Procesos para ver si tiene que crear un nuevo proceso o no.
                for (i = 0; i < MAX_PROCESS; i++) {
                    if(processes[i].pid == number1) {
                        create = false;
                    }
                }
                // En caso de que se haya recorrido el Array de Procesos y se haya encontrado el proceso, entonces se le asigna un contenido a la pagina.
                if (create == false) {
                    modify_process(number1, number2, option);
                } else 
                // En caso de que se haya recorrido el Array de Procesos y no se haya encontrado el proceso, se crea y se le asigna un contenido al nuevo proceso.
                {
                    create_process(number1);
                    modify_process(number1, number2, option);
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
