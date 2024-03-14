#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    
    int suma = 0;

    /* Loops */
    int i = 1; 

    while (i < argc) {
       suma = suma +  atoi(argv[i]);
       i = i + 1;
    }

    printf("%i\n", suma);

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
