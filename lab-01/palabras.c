#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    int c;
    while((c = getchar()) != "^C") {
    	if(c == ' '){
            printf("\n");
 	} else {
	    printf("%c", c);
	}        
    }     
    
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
