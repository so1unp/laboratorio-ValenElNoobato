#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[])
{
    size_t bufSize, numBytes, thisWrite, totWritten;
    char *buf;
    int sync, fd, openFlags;
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    if (argc != 5) {
        fprintf(stderr, "Uso: %s num-bytes buf-size sync archivo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numBytes = atoi(argv[1]);
    bufSize = atoi(argv[2]);
    sync = atoi(argv[3]);

    buf = malloc(bufSize);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY;

    fd = open(argv[4], openFlags, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite) {
        thisWrite = numBytes - totWritten > bufSize ? bufSize : numBytes - totWritten;

        if (sync == 0) {
            if (write(fd, buf, thisWrite) != thisWrite) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        // Realizar la operación de sincronización según el valor de sync
        if (sync == 1) {
            if (fsync(fd) == -1) {
                perror("fsync");
                exit(EXIT_FAILURE);
            }
        } 
        if (sync == 2) {
            if (fdatasync(fd) == -1) {
                perror("fdatasync");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("El programa tardo %f segundos en ejecutarse\n", cpu_time_used);
    exit(EXIT_SUCCESS);
}
