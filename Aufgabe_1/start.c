#include <stdlib.h>         // Für Standardbibliotheksfunktionen wie exit() und EXIT_FAILURE/SUCCESS
#include <stdio.h>          // Für Ein-/Ausgabefunktionen wie printf() und fprintf()
#include <unistd.h>         // Für POSIX-Funktionen wie fork() und execvp()
#include <sys/types.h>      // Für Datentyp pid_t 
#include <sys/resource.h>   // Für die Funktion setpriority()

/**
 * Hauptfunktion, die einen Kindprozess erstellt und ein angegebenes Programm mit Argumenten ausführt.
 * @param argc die Anzahl der Befehlszeilenargumente
 * @param argv ein Array von Zeichenketten, das die Befehlszeilenargumente enthält
 * @return EXIT_SUCCESS bei erfolgreicher Ausführung, andernfalls EXIT_FAILURE
 */
int main(int argc, char **argv) {
    if (argc < 2) {                                                                         // Überprüfen, ob genügend Argumente übergeben wurden
        fprintf(stderr, "usage: %s <program> <arguments>\n", argv[0]);                      // Fehlermeldung ausgeben
        exit(EXIT_FAILURE);                                                                 // Programm beenden mit Fehlercode
    }

    pid_t pid = fork();                                                                     // Einen neuen Prozess mittels fork() erstellen

    if (pid < 0) {                                                                          // Überprüfen, ob fork() fehlgeschlagen ist
        perror("fork");                                                                     // Fehlermeldung ausgeben
        exit(EXIT_FAILURE);                                                                 // Programm beenden mit Fehlercode
    } else if (pid == 0) {                                                                  // Kindprozess erstellen
        if (setpriority(PRIO_PROCESS, 0, 19) < 0) {                                         // Priorität des Kindprozesses auf 19 setzen -> niedrige Priorität
            perror("setpriority");                                                          // Fehlermeldung ausgeben, falls setpriority fehlschlägt
            exit(EXIT_FAILURE);                                                             // Kindprozess beenden mit Fehlercode
        }
        execvp(argv[1], &argv[1]);                                                          // Das Programm mit den angegebenen Argumenten ausführen
        perror("execvp");                                                                   // Fehlermeldung ausgeben, falls execvp fehlschlägt
        exit(EXIT_FAILURE);                                                                 // Kindprozess beenden mit Fehlercode
    } else {                                                                                // Elternprozess
        int status;                                                                         // Variable zum Speichern des Status des Kindprozesses
        printf("Process %d started\n", pid);                                                // Die PID des gestarteten Prozesses ausgeben

        if (waitpid(pid, &status, 0) < 0) {                                                 // Auf das Ende des Kindprozesses warten
            perror("waitpid");                                                              // Fehlermeldung ausgeben, falls waitpid fehlschlägt
            exit(EXIT_FAILURE);                                                             // Programm beenden mit Fehlercode
        }
        if (WIFEXITED(status)) {                                                            // Überprüfen, ob der Prozess normal beendet wurde
            printf("Return code: %d\n", WEXITSTATUS(status));                               // Den Rückgabecode des Prozesses ausgeben
        }
        if (WIFSIGNALED(status)) {                                                          // Prüfen, ob der Prozess durch ein Signal beendet wurde
            int signal = WTERMSIG(status);                                                  // Das Signal, das den Prozess beendet hat
            printf("Process terminated by signal %d (%s)\n", signal, strsignal(signal));    // Ausgabe des Signals und seiner Beschreibung
        }
    }
    exit(EXIT_SUCCESS);                                                                     // Programm erfolgreich beenden
}