// down.c 
//
// Autor: CB - UNPSJB 
// 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <pthread.h> 
#include <string.h>
#include <fcntl.h>

int main (int argc, char *argv[ ])
{
   sem_t *semaforo;
   int seg;

   // Controla parámetros de entrada
   if (argc<2)
   {   perror("uso: down <semaforo> \n"); exit(1); }
  
   /* Crea el semáforo nombrado */
   if((semaforo=sem_open(argv[1], O_CREAT, 0644, 1))==(sem_t *)-1)
   {   perror("No se puede crear el semáforo"); exit(1); }
   
   printf("Hace DOWN del semáforo\n");
   sem_wait(semaforo);
   printf("Cierra semaforos y termina\n");
   sem_close(semaforo); 
   exit(0);
}

