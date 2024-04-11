#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>

#define CHILD_MAX 100
int hijos[CHILD_MAX];
int numHijos = 0;

void childhandler() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Child %d (nice %2d):\t%3li\n" , getpid(), getpriority(PRIO_PROCESS, (id_t) getpid()), usage.ru_utime.tv_sec);
    exit(0);
}

void killAll() {
    int i;
    for (i = 0; i < numHijos; i++) {
        kill(hijos[i], SIGTERM);
    }
    exit(0);
}

void blockKill() {
    return;
}

int busywork(void)
{
    struct tms buf;

    for (;;) {
        times(&buf);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 4 && argc < 4) {
        printf("Illegal arguments");
        exit(EXIT_FAILURE);
    }

    int numSeg = atoi(argv[2]), prio = atoi(argv[3]), i;
    numHijos = atoi(argv[1]);

    struct sigaction handlerChild;
    handlerChild.sa_handler = childhandler;

    struct sigaction terminateAll;
    terminateAll.sa_handler = killAll;

    struct sigaction noKill;
    noKill.sa_handler = blockKill;

    for(i = 1; i <= numHijos; i++) {
        hijos[i] = fork();
        
        if (hijos[i] == 0) {
            sigaction(SIGTERM, &handlerChild, NULL);
            if (prio == 1) {
                if (i > 19) {
                    nice(19);
                } else {
                    nice(i);
                }
            } else {
                nice(0);
            }
            sigaction(SIGINT, &noKill, NULL);
            busywork();
        } 
        if (hijos[i] < 0) {
            printf("Error creando el hijo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Hola soy el padre.\n");
    sigaction(SIGINT, &terminateAll, NULL);

    if (numSeg > 0) {
        sleep((unsigned int) numSeg);
        for(i = 1; i <= numHijos; i++) {
            kill(hijos[i], SIGTERM);
        }
    }
    if (numSeg == 0) {
        while (pause()) {

        }
    }

    exit(EXIT_SUCCESS);
}
