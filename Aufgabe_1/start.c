#include <stdlib.h>         // Für Standardbibliotheksfunktionen wie exit() und EXIT_FAILURE/SUCCESS
#include <stdio.h>          // Für Ein-/Ausgabefunktionen wie printf() und fprintf()
#include <unistd.h>         // Für POSIX-Funktionen wie fork() und execvp()
#include <sys/types.h>      // Für Datentyp pid_t 
#include <sys/resource.h>   // Für die Funktion setpriority()
#include <sys/wait.h>       // Für die Funktion waitpid() und Makros zur Statusüberprüfung
#include <string.h>         // Für die Funktion strsignal()

/**
 * Hauptfunktion, die einen Kindprozess mit einem übergebenen Programm startet
 * @param argc die Anzahl der Befehlszeilenargumente
 * @param argv ein Array von Zeichenketten, das die Befehlszeilenargumente enthält
 * @return EXIT_SUCCESS bei erfolgreicher Ausführung, andernfalls EXIT_FAILURE
 */
int main(int argc, char **argv) {
    // Überprüfen, ob mindestens ein Argument übergeben wurde
    if (argc < 2) {              
        // Fehlermeldung ausgeben, falls keine Argumente übergeben wurden
        fprintf(stderr, "Nutzung: %s <Programm> <Argumente>\n", argv[0]);     
        // Programm mit Fehlercode EXIT_FAILURE beenden                 
        exit(EXIT_FAILURE);                                                                 
    }

    // Starten eines neuen Prozesses mittels fork()
    pid_t pid = fork();                                                                     
    // pid_t = Prozess-ID, die durch fork() zurückgegeben wird und verwendet wird, um den Prozess zu identifizieren
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
        if (execvp(argv[1], &argv[1]) < 0) {                                               
            // Fehlermeldung ausgeben, falls execvp fehlschlägt
            // Dies wird durch einen Rückgabewert kleiner als 0 signalisiert, welcher z.B. durch einen Fehler bei der Ausführung des Programms entstehen kann 
            // Z.B. ungültiger Dateipfad, fehlende Berechtigungen, etc.
            perror("execvp");                                                               
            // Kindprozess beenden mit Fehlercode
            exit(EXIT_FAILURE);                                                             
        }
    } else {                                                                                
        // Ansonsten handelt es sich um den Elternprozess
        int status;                                                                         
        // Variable zum Speichern des Status des Kindprozesses
        printf("Prozess %d wurde gestartet\n", pid);  
        printf("Priorität des Kindprozesses: %d\n", getpriority(PRIO_PROCESS, pid));                                              
        // Die PID des gestarteten Kindprozesses ausgeben

        if (waitpid(pid, &status, 0) < 0) {                                                 
            // Auf das Ende des Kindprozesses warten
            // waitpid() sorgt dafür, dass der Elternprozess auf das Ende des Kindprozesses wartet
            // Wird statt wait() genutzt, da waitpid() auf spezifische Kindprozesse warten kann, wait() wartet auf beliebige Kindprozesse
            // Stellt sicher, dass der Elternprozess Informationen über die Beendigung des Kindprozesses erhalten kann
            perror("waitpid");                                                              
            // Fehlermeldung ausgeben, falls waitpid fehlschlägt
            // Programm wird mit Fehlercode beendet
            // Mögliche Ursachen dafür sind z.B. Empfangen eines Signals, das den waitpid-Aufruf unterbrochen hat oder
            // der Kindprozess bereits beendet wurde und dabei von einem anderen waitpid-Aufruf behandelt wurde
            exit(EXIT_FAILURE);                                                             
            // Programm beenden mit Fehlercode
        }
        if (WIFEXITED(status)) {                                                            
            // WIFEXITED(status) ist ein Boolean, überprüft ob der Prozess normal beendet wurde
            printf("Exit-Code: %d\n", WEXITSTATUS(status));                               
            // Den Rückgabecode des Prozesses ausgeben
            // WEXITSTATUS(status) ist ein Integer, gibt den Exitcode des Kindprozesses aus, falls dieser normal beendet wurde
        }
        if (WIFSIGNALED(status)) {                                                          
            // WIFSIGNALED(status) ist ein Boolean, prüft, ob der Kindprozess durch ein Signal beendet wurde
            int signal = WTERMSIG(status);                                                  
            // WTERMSIG(status) ist ein Integer, gibt das Signal aus, das den Kindprozess beendet hat
            printf("Prozess durch Signal %d beendet: %s\n", signal, strsignal(signal));
            // strsignal(signal) gibt die Bezeichnung des Signals aus, das den Kindprozess beendet hat
        }
    }
    exit(EXIT_SUCCESS);                                                                     
    // Programm erfolgreich beenden
}