#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void print_file_info(const char *path) {
    struct stat fileStat;

    if (stat(path, &fileStat) < 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // ID del usuario propietario
    printf("ID del usuario propietario: %d\n", fileStat.st_uid);

    // Tipo de archivo y modo
    printf("Tipo de archivo: ");
    switch (fileStat.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
    }

    // Número de i-nodo
    printf("Número de i-nodo: %ld\n", (long)fileStat.st_ino);

    // Tamaño en bytes
    printf("Tamaño en bytes: %lld\n", (long long)fileStat.st_size);

    // Número de bloques que ocupa en disco
    printf("Número de bloques: %lld\n", (long long)fileStat.st_blocks);

    // Tiempo de última modificación
    printf("Última modificación: %s", ctime(&fileStat.st_mtime));

    // Tiempo de último acceso
    printf("Último acceso: %s", ctime(&fileStat.st_atime));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ruta_del_archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    print_file_info(argv[1]);

    exit(EXIT_SUCCESS);
}
