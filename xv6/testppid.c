#include "types.h"
#include "user.h"

int main(void) {
    int pid = getpid();
    int ppid = getppid();

    printf(1, "PID: %d, PPID: %d\n", pid, ppid);

    exit();
}