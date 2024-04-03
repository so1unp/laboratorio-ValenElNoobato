/*
Este programa encripta/descencripta un archivo
invocar con:
  crypt arch_entrada arch_salida clave
Autor: Silberchatz
*/

#include<unistd.h>
#include<sys/types.h>
#include<stdio.h> 
int main(int argc, char* argv[])
{
  char buf[1];
  int fd1,fd2;
  int n,desp;
  fd1=open(argv[1],0);
  fd2=creat(argv[2],0644);

  printf("El valor de fd1 es [%i]\n",fd1);
  printf("El valor de fd2 es [%i]\n",fd2);

  desp=atoi(argv[3]);
 
  while((n=read(fd1,buf,1))>0){
	buf[0]=buf[0]+desp;
	write(fd2,buf,n);
  }
  close(fd1);
  close(fd2); 
  return 0;
}

