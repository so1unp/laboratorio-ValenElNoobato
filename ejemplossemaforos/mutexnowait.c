// mutexnowait.c 
// Similar a mutex.c pero los semáforos no esperan indefinidamente. 
// Intentan la lectura o esperan un timeout
//
// Primero intenta un "sem_trywait". Si no lo consigue emite mensaje
// y luego intenta un "sem_timedwait" durante 20 segundos. Si no lo consigue
// emite error y termina 
// 
// Para verlo en funcionamiento ejecutelo concurrentemente con mutex.c
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
#include <time.h>

int main (int argc, char *argv[ ])
{
   sem_t *semaforo;
   int seg;
   struct timespec tm;
   int status;

   // Controla parámetros de entrada
   if (argc<3)
   {   perror("uso: mutexnowait <semaforo> <segs zona critica>\n"); exit(1); }
   seg = atoi(argv[2]);
  
   /* Crea el semáforo nombrado */
   if((semaforo=sem_open(argv[1], O_CREAT, 0644, 1))==(sem_t *)-1)
   {   perror("No se puede crear el semáforo"); exit(1); }
   
   printf("Intenta entrar a la zona crítica\n");
   status = sem_trywait(semaforo);
   if (status<0){
     printf("El semáforo está ocupado, intenta nuevamente por 20 segundos\n");
     clock_gettime(CLOCK_REALTIME, &tm);
     tm.tv_sec += 20;
     status = sem_timedwait(semaforo,&tm);
     if (status<0){
       printf("El semáforo sigue ocupado. TERMINA sin entrar en zona critica\n");
       exit(1);
     }
   }
   printf("ENTRA a la zona crítica (demora %d segundos)\n",seg);
     sleep(seg);
   printf("SALE de la zona crítica\n");
   sem_post(semaforo);
   printf("Cierra semaforos y termina\n");
   sem_close(semaforo); 
   exit(0);
}

