//
// Pará qué puede servir este programa?
//

#include <unistd.h> 
#include <sys/types.h> 
main()
{ 
    char buf[1]; 
    int n; 
    while ((n = read(0, buf, 1)) > 0) 
         write(1, buf, n); 
    return 0; 
} 

