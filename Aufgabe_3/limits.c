#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

void handle_signal (int signal) {
    if (signal == SIGXCPU) {
        printf("CPU time limit exceeded\n");
    } else if (signal == SIGSEGV) {
        printf("Memory limit exceeded\n");
    } else if (signal == SIGXFSZ) {
        printf("File size limit exceeded\n");
    }
    exit(EXIT_FAILURE);
}

void set_limits () {
    struct rlimit limit;

    limit.rlim_cur = 1;
    limit.rlim_max = 1;
    setrlimit(RLIMIT_CPU, &limit);

    limit.rlim_cur = 64 * 1024;
    limit.rlim_max = 64 * 1024;
    setrlimit(RLIMIT_STACK, &limit); 

    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    setrlimit(RLIMIT_FSIZE, &limit);
}

int main () {
    signal(SIGXCPU, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGXFSZ, handle_signal);

    set_limits();

    // CPU time limit
    while (1) {
        // Stack size limit
        char large_stack[1024 * 1024];

        // File size limit
        FILE *file = fopen("file.txt", "w");
        if (file) {
            for (int i = 0; i < 1024; i++) {
                fprintf(file, "Exceeding file size limit!\n");
            }
            fclose(file);
        }
    }
    exit(EXIT_SUCCESS);
}