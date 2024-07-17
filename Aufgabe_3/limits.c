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
    limit.rlim_max = 1;
    setrlimit(RLIMIT_CPU, &limit);

    // Setzen der Stackgröße auf 1 KB
    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    setrlimit(RLIMIT_STACK, &limit);

    // Setzen der maximalen Dateigröße auf 1 KB
    limit.rlim_cur = 1024;
    limit.rlim_max = 1024;
    setrlimit(RLIMIT_FSIZE, &limit);
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
                // Endlosschleife, um CPU-Zeitlimit zu überschreiten
            }
            break;
        case 2:
            while (1) {
                // Speicherlimit überschreiten durch Endlosschleife, in der Speicher allokiert wird 
                void *ptr = malloc(4096); //  Allokieren von 4 KB Speicher
            }
            break;
        case 3:
            // Überschreiten des Dateigrößenlimits durch Schreiben in eine Datei
            FILE *file = fopen("file.txt", "w");    // Öffnen der Datei im Schreibmodus
            while (1) {
                // Endlosschleife, um Dateigrößenlimit zu überschreiten 
                fputc('a', file); // Schreiben in die Datei
            }
            fclose(file);   // Schließen der Datei
            break;
        default:
            printf("Ungueltige Auswahl\n");
            break;
    }
    return 0;
}