#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void sighandler(int sig) {
	printf("%s\n", strsignal(sig));
}

void set() {
	int i;
	for(i = 1; i <= 31; i++) {
		signal(i, sighandler);
		//sigaction(i, sighandler, NULL);
	}
}

int	main(void)
{
	set();

	while(1) {
		pause();
	}
}