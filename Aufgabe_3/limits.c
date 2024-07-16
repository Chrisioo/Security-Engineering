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

    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    setrlimit(RLIMIT_STACK, &limit); 

    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    setrlimit(RLIMIT_FSIZE, &limit);
}

int main () {
    int input;
    signal(SIGXCPU, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGXFSZ, handle_signal);

    set_limits();

    printf("1: Exceed CPU time limit\n");
    printf("2: Exceed memory limit\n");
    printf("3: Exceed file size limit\n");
    scanf("%d", &input);
    switch (input) {
        case 1:
            while (1) {}
            break;
        case 2:
            while (1) {
                malloc(1024);
            }
            break;
        case 3:
            FILE *file = fopen("file.txt", "w");
            while (1) {
                fputc('a', file);
            }
            fclose(file);
            break;
        default:
            break;
    }
    exit(EXIT_SUCCESS);
}