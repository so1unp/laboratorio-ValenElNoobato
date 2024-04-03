/*
  Abre arch UNO y copia primeros 10 bytes en arch DOS y segundos en arch TRES
*/

#include<unistd.h>
#include<sys/types.h>
#include<stdio.h> 

int main(int argc, char* argv[])
{
  char buf[100];
  int fd1,fd2,fd3;
  int n,desp;

  // Abre archivo UNO
  fd1=open("UNO",0);
  if (fd1<0) {
      printf("ERROR: no existe el archivo UNO\n");
  }
  printf("fd1 %i\n",fd1);
  // Crea los archivos de salida
  fd2=creat("DOS",0644);
  fd3=creat("TRES",0644);

  // lee y escribe los primeros 10 bytes
  n = read(fd1,buf,10);
  write(fd2,buf,n);

  // lee y escribe los segundos 10 bytes
  n = read(fd1,buf,10);
  write(fd3,buf,n);

  close(fd1);
  close(fd2); 
  close(fd3); 
  return 0;
}

