#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    int MAXTAMANO = 20;
    int list[MAXTAMANO];
    int c = 0;
    int i = 0;
    int j = 0;
    int aux = 0;

    // for para eliminar el residuo del array
    for(i = 0; i < MAXTAMANO; i++){ list[i] = 0; }

    i = 0;

    while((c = getchar()) != EOF) {
	    if(c != ' ' && c != '\n'){
	        i++;
	    } else {
	        list[j] = i;
	        j++;
	        i = 0;
	    }
    }   

    //Un for simple para sacar la palabra con mas letras
    for(i = 0; i < MAXTAMANO; i++){
	    if(aux < list[i]) { aux = list[i]; }
    }

    for(i = 1; i <= aux; i++){
	    printf("%i",i," ");

	    for(j = 0; j < MAXTAMANO; j++){
	        if(i == list[j]){
		    printf("*");
	        }
	    }

	    printf("\n");
    }

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
