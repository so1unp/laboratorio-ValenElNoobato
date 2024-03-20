#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    int c = 0, i = 0, j = 0, aux = 0;
    int MAXIMO_TAMANO = 10;
    int list[MAXIMO_TAMANO];

    // for para eliminar el residuo del array
    for(i = 0; i < MAXIMO_TAMANO; i++){
	list[i] = 0;
    }

    while((c = getchar()) != EOF) {
	if(c != ' ' && c != '\n' && c != EOF){
	    i++;
	} else {
	    list[j] = i;
	    j++;
	    i = 0;
	}
    }   

    //Un for simple para sacar la palabra con mas letras
    for(i = 0; i < MAXIMO_TAMANO; i++){
	if(aux < list[i]) { aux = list[i]; }
    }

    for(i = 1; i <= aux; i++){
	printf("%i",i," ");

	for(j = 0; j < MAXIMO_TAMANO; j++){
	    if(i == list[j]){
		printf("*");
	    }
	}

	printf("\n");
    }

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
