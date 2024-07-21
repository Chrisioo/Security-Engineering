#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

void handle_signal(int signal) {
    if (signal == SIGXCPU) {
        // SIGXCPU = Makro für Signal, das gesendet wird, wenn das CPU-Zeitlimit überschritten wird
        printf("CPU-Zeitlimit überschritten\n");
    } else if (signal == SIGSEGV) {
        // SIGSEGV = Makro für Signal, das gesendet wird, wenn ein Speicherzugriffsfehler auftritt
        printf("Speicherlimit überschritten\n");
    } else if (signal == SIGXFSZ) {
        // SIGXFSZ = Makro für Signal, das gesendet wird, wenn das Dateigrößenlimit überschritten wird
        printf("Dateigrößenlimit überschritten\n");
    }
    fflush(stdout);
    exit(EXIT_FAILURE);
}

void set_limits() {
    struct rlimit limit;

    // Setzen der CPU-Zeitgrenze auf 1 Sekunde
    limit.rlim_cur = 1;
    limit.rlim_max = 2;
    setrlimit(RLIMIT_CPU, &limit);

    // Setzen der Stackgröße auf 1 KB
    limit.rlim_cur = 4 * 1024;
    limit.rlim_max = 8 * 1024;
    setrlimit(RLIMIT_STACK, &limit);

    // Setzen der maximalen Dateigröße auf 1 KB
    limit.rlim_cur = 1024;
    limit.rlim_max = 2048;
    setrlimit(RLIMIT_FSIZE, &limit);
}

void exceed_cpu_time() {
    while (1) {
        // Endlosschleife, um CPU-Zeitlimit zu überschreiten
    }
}

void exceed_stack() {
    exceed_stack();
}

void exceed_filesize() {
    FILE *file = fopen("test.txt", "w");
    while (1) {
        fputc('a', file);
    }
    fclose(file);
}

int main() {
    int input;
    signal(SIGXCPU, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGXFSZ, handle_signal);

    set_limits();

    printf("1: CPU-Zeitlimit überschreiten\n");
    printf("2: Speicherlimit überschreiten\n");
    printf("3: Dateigrößenlimit überschreiten\n");

    scanf("%d", &input);
    switch (input) {
        case 1:
            while (1) {
                exceed_cpu_time();  // Endlosschleife, um CPU-Zeitlimit zu überschreiten
            }
            break;
        case 2:
            while (1) {
                exceed_stack(); // Rekursiver Funktionsaufruf, um Speicherlimit zu überschreiten
            }
            break;
        case 3:
            while (1) {
                exceed_filesize(); // Endlosschleife, um Dateigrößenlimit zu überschreiten
            }
            break;
        default:
            printf("Ungueltige Auswahl\n");
            break;
    }
    return 0;
}