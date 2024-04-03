/*
  Lee del archivo ejwrite.dat
  Autor C.B.
*/

#include<string.h> 
#include<stdio.h> 

int main(int argc, char* argv[])
{
 char buf[100];
  int buf;
  int fd;

  fd = open("ejwrite.dat",0);

  read(fd,buf,100);

 printf("leyó %s\n",buf);
  printf("leyó %i\n",buf);

  close(fd);

  return 0;
}

