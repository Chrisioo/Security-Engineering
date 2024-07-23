#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

/**
 * Funktion zum Behandeln von Signalen
 * @param signal Signal, das empfangen wurde
 * Gibt eine Fehlermeldung aus und beendet das Programm
 */
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
    exit(EXIT_FAILURE);
}

/**
 * Funktion zum Setzen der Ressourcenlimits
 * CPU-Zeitlimit: 1 Sekunde aktuelle CPU-Zeit, 2 Sekunden maximale CPU-Zeit
 * Stackgröße: 4 KB aktuelle Stackgröße, 8 KB maximale Stackgröße
 * Dateigröße: 1 KB aktuelle Dateigröße, 2 KB maximale Dateigröße
 */
void set_limits() {
    struct rlimit limit;                // Struktur für Ressourcenlimits

    limit.rlim_cur = 1;
    limit.rlim_max = 2;
    setrlimit(RLIMIT_CPU, &limit);      // RLIMIT_CPU = Makro für CPU-Zeitlimit

    limit.rlim_cur = 4096;
    limit.rlim_max = 8192;
    setrlimit(RLIMIT_STACK, &limit);    // RLIMIT_STACK = Makro für Stackgröße

    limit.rlim_cur = 1024;
    limit.rlim_max = 2048;
    setrlimit(RLIMIT_FSIZE, &limit);    // RLIMIT_FSIZE = Makro für Dateigröße
}

/**
 * Funktion zum Überschreiten des CPU-Zeitlimits
 * Endlosschleife, um CPU-Zeitlimit zu überschreiten
 */
void exceed_cpu_time() {
    while (1) {
        // Endlosschleife, um CPU-Zeitlimit zu überschreiten
    }
}

/**
 * Funktion zum Überschreiten des Speicherlimits
 * Rekursiver Funktionsaufruf, um Speicherlimit zu überschreiten
 */
void exceed_stack() {
    exceed_stack();
}

/**
 * Funktion zum Überschreiten des Dateigrößenlimits
 * Endlosschleife, um Dateigrößenlimit zu überschreiten
 * Schreibt 'a' in eine Datei, bis das Dateigrößenlimit überschritten wird
 */
void exceed_filesize() {
    FILE *file = fopen("test.txt", "w");
    while (1) {
        fputc('a', file);
    }
    fclose(file);
}

/**
 * Hauptfunktion
 * Fragt nach einer Auswahl und ruft die entsprechende Funktion auf
 */
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