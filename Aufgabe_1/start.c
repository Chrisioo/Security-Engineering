#include <stdlib.h>         // Für Standardbibliotheksfunktionen wie exit() und EXIT_FAILURE/SUCCESS
#include <stdio.h>          // Für Ein-/Ausgabefunktionen wie printf() und fprintf()
#include <unistd.h>         // Für POSIX-Funktionen wie fork() und execvp()
#include <sys/types.h>      // Für Datentyp pid_t 
#include <sys/resource.h>   // Für die Funktion setpriority()
#include <sys/wait.h>       // Für die Funktion waitpid() und Makros zur Statusüberprüfung

/**
 * Hauptfunktion, die einen Kindprozess erstellt und ein angegebenes Programm mit Argumenten ausführt.
 * @param argc die Anzahl der Befehlszeilenargumente
 * @param argv ein Array von Zeichenketten, das die Befehlszeilenargumente enthält
 * @return EXIT_SUCCESS bei erfolgreicher Ausführung, andernfalls EXIT_FAILURE
 */
int main(int argc, char **argv) {
    // Überprüfen, ob genügend Argumente übergeben wurden
    if (argc < 2) {              
        // Fehlermeldung ausgeben falls nicht
        fprintf(stderr, "usage: %s <program> <arguments>\n", argv[0]);     
        // Programm mit Fehlercode EXIT_FAILURE beenden                 
        exit(EXIT_FAILURE);                                                                 
    }

    // Einen neuen Prozess mittels fork() erstellen
    pid_t pid = fork();                                                                     
    // pid_t = Prozessidentifikator, wird verwendet, um festzustellen, ob ein Prozess erfolgreich erstellt wurde
    // Der Wert von pid wird durch die PID des durch fork erzeugten Prozess gesetzt                                                                                       

    // Überprüfen, ob fork() fehlgeschlagen ist
    // In diesem Fall wird eine PID kleiner als 0 geliefert
    if (pid < 0) {                                                                          
    // Fehlermeldung ausgeben, falls fork() fehlgeschlagen ist                                                                               
    perror("fork");                    
    // Programm mit Fehlercode EXIT_FAILURE beenden                                                 
    exit(EXIT_FAILURE);    

    // Überprüfen, ob fork() erfolgreich war                                                              
    } else if (pid == 0) {                                                                  
    // Kindprozess: Der Rückgabewert von fork() ist 0 im Kindprozess
    if (setpriority(PRIO_PROCESS, 0, 19) < 0) {                                         
        // Priorität des Kindprozesses auf 19 setzen -> niedrige Priorität
        // Fehlermeldung ausgeben, falls setpriority fehlschlägt
        perror("setpriority");                                                          
        // Kindprozess beenden mit Fehlercode
        exit(EXIT_FAILURE);                                                             
    }
    // Das Programm mit den angegebenen Argumenten ausführen
    execvp(argv[1], &argv[1]);                                                          
    // Fehlermeldung ausgeben, falls execvp fehlschlägt
    perror("execvp");                                                                   
    // Kindprozess beenden mit Fehlercode
    exit(EXIT_FAILURE);                                                                 
    } else {                                                                                
        // Elternprozess
        int status;                                                                         
        // Variable zum Speichern des Status des Kindprozesses
        printf("Process %d started\n", pid);                                                
        // Die PID des gestarteten Kindprozesses ausgeben

        if (waitpid(pid, &status, 0) < 0) {                                                 
            // Auf das Ende des Kindprozesses warten
            // waitpid() sorgt dafür, dass der Elternprozess auf das Ende des Kindprozesses wartet
            // Stellt sicher, dass der Elternprozess Informationen über die Beendigung des Kindprozesses erhalten kann
            perror("waitpid");                                                              
            // Fehlermeldung ausgeben, falls waitpid fehlschlägt
            exit(EXIT_FAILURE);                                                             
            // Programm beenden mit Fehlercode
        }
        if (WIFEXITED(status)) {                                                            
            // Überprüfen, ob der Prozess normal beendet wurde
            // WIFEXITED(status) prüft, ob der Kindprozess normal beendet wurde
            printf("Return code: %d\n", WEXITSTATUS(status));                               
            // Den Rückgabecode des Prozesses ausgeben
            // WEXITSTATUS(status) gibt den Exitcode des Kindprozesses aus, falls dieser normal beendet wurde
        }
        if (WIFSIGNALED(status)) {                                                          
            // Prüfen, ob der Prozess durch ein Signal beendet wurde
            // WIFSIGNALED(status) prüft, ob der Kindprozess durch ein Signal beendet wurde
            int signal = WTERMSIG(status);                                                  
            // Das Signal, das den Prozess beendet hat
            // WTERMSIG(status) gibt das Signal aus, das den Kindprozess beendet hat
            printf("Process terminated by signal %d (%s)\n", signal, strsignal(signal));    
            // Ausgabe des Signals und seiner Beschreibung
            // strsignal(signal) gibt die Bezeichnung des Signals aus, das den Kindprozess beendet hat
        }
    }
    exit(EXIT_SUCCESS);                                                                     
// Programm erfolgreich beenden
}