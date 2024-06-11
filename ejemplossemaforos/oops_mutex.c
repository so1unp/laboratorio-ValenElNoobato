/**
   Soluciona el problema de concurrencia de oops.c
 */

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h> 
#include <stdio.h>

int a_global; 
sem_t *mutex;

void *suma(void *vueltas); 

int main(int argc, char *argv[])
{
pthread_t tid1, tid2; /* identificadores de threads */
pthread_attr_t attr; /*  atributos de los threads */

if (argc != 2) {
	fprintf(stderr,"usage: oops <integer value>\n");
	return -1;
}

if (atoi(argv[1]) < 0) {
	fprintf(stderr,"el argumento debe ser positivo\n",atoi(argv[1]));
	return -1;
}

/* Crea el semáforo mutex */
   if((mutex=sem_open("mutex_oops", O_CREAT, 0644, 1))==(sem_t *)-1)
   {   perror("No se puede crear el semáforo"); exit(1); }

/* trae atributos default*/
pthread_attr_init(&attr);

/* crea dos threads que ejecutan suma(vueltas) */
pthread_create(&tid1,&attr,suma,argv[1]);
pthread_create(&tid2,&attr,suma,argv[1]);

/* espera que los threads terminen*/
pthread_join(tid1,NULL);
pthread_join(tid2,NULL);

printf("Valor de a_global = %d\n",a_global);
}

/**
 * El thread suma valores a la variable a_global
 */
void *suma(void *vueltas)
{
int i,b;

   for (i=0;i<atoi(vueltas);i++){
     sem_wait(mutex);
     b = a_global;
     a_global = b + 1;
     sem_post(mutex);
   }
   pthread_exit(0);
}


