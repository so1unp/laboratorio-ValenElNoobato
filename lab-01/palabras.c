#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    int c;
    while((c = getchar()) != EOF && c != "^C") {
    	if(c == " "){
            printf("%c\n", c);
 	} else {
	    printf("%c", c);
	}        
    }     
    
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
