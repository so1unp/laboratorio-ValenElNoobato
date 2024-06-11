// semgetval.c 
// Recupera el valor actual de un semáforo POSIX nombrado
// 
// Recibe como parÃ¡metro el nombre del semáforo a recuperar
//
// Autor: CB - UNPSJB 
// 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <string.h>
#include <fcntl.h>

int main (int argc, char *argv[ ])
{
   sem_t *semaforo;
   int value;

   // Controla parámetros de entrada
   if (argc==1)
   {   perror("uso: semgetval <semaforo_nombrado>\n"); exit(1); }
  
   /* se asocia Al semáforo nombrado */
   if((semaforo=sem_open(argv[1], O_CREAT, 0755, 1))==(sem_t *)-1)
   {   printf("No se encuentra el semáforo %s",argv[1]); exit(1); }
   
   sem_getvalue(semaforo,&value);
   printf("El valor del semáforo %s es %d \n",argv[1],value);
   sem_close(semaforo); 
   exit(0);
}

