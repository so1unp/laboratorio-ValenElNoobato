#include "types.h"
#include "user.h"

int main(void) {
    int num_procesos = pscnt();
    printf(1, "Número de procesos en el sistema: %d\n", num_procesos);
    
    exit();
}