#include <stdlib.h>         // exit() und EXIT_FAILURE/SUCCESS
#include <stdio.h>          // printf() und fprintf()
#include <unistd.h>         // fork() und execvp()
#include <sys/types.h>      // Datentyp pid_t 
#include <sys/resource.h>   // setpriority()
#include <sys/wait.h>       // waitpid() und Makros zur Statusüberprüfung
#include <string.h>         // strsignal()

#define LOWEST_PRIORITY 19  // Niedrigste Priorität für Kindprozesse
/**
 * Hauptfunktion, die einen Kindprozess erstellt und ein angegebenes Programm mit Argumenten ausführt.
 * @param argc die Anzahl der Befehlszeilenargumente
 * @param argv ein Array von Zeichenketten, das die Befehlszeilenargumente enthält
 * @return EXIT_SUCCESS bei erfolgreicher Ausführung, andernfalls EXIT_FAILURE
 */
int main(int argc, char **argv) {
    // Prüfen, ob mindestens ein Argument übergeben wurde
    if (argc < 2) {              
        // Fehlermeldung ausgeben, falls keine Argumente übergeben wurden
        fprintf(stderr, "Nutzung: %s <Programm> <Argumente>\n", argv[0]);     
        // Programm mit Fehlercode EXIT_FAILURE beenden                 
        exit(EXIT_FAILURE);                                                                 
    }

    // Starten eines neuen Prozesses mittels fork()
    pid_t pid = fork();                                                                     
    // pid_t = Prozess-ID, die durch fork() zurückgegeben wird und verwendet wird, um den Prozess zu identifizieren                                                                                   

    if (pid == 0) {                                                                  
        if (setpriority(PRIO_PROCESS, 0, LOWEST_PRIORITY) == -1) {                                         
            // Priorität des Kindprozesses auf 19 setzen -> niedrige Priorität
            // Fehlermeldung ausgeben, falls setpriority fehlschlägt
            perror("setpriority");                                                          
            // Kindprozess beenden mit Fehlercode
            exit(EXIT_FAILURE);                                                             
        }
        // Das Programm mit den angegebenen Argumenten ausführen
        execvp(argv[1], &argv[1]);          
        // execvp gibt nur zurück, wenn ein Fehler auftritt                                                
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {    // Ansonsten handelt es sich um den Elternprozess                                                       
        int status;                                                                         
        // Variable zum Speichern des Status des Kindprozesses
        printf("Kindprozess %d wurde gestartet\n", pid);  
        printf("Priorität des Kindprozesses: %d\n", getpriority(PRIO_PROCESS, pid));                                              
        // Die PID des gestarteten Kindprozesses ausgeben
        if (waitpid(pid, &status, 0) < 0) {                                                 
            // waitpid() sorgt dafür, dass der Elternprozess auf das Ende des Kindprozesses wartet
            // Wird statt wait() genutzt, da waitpid() auf spezifische Kindprozesse warten kann, wait() wartet auf beliebige Kindprozesse
            // Stellt sicher, dass Elternprozess Informationen über die Beendigung des Kindprozesses erhalten kann
            perror("waitpid");                                                              
            // Fehlermeldung ausgeben, falls waitpid fehlschlägt
            exit(EXIT_FAILURE);                                                             
            // Programm beenden mit Fehlercode
        }
        if (WIFEXITED(status)) {                                                            
            // WIFEXITED(status) prüft, ob der Kindprozess normal beendet wurde
            // WEXITSTATUS(status) gibt den Exitcode des Kindprozesses aus, falls dieser normal beendet wurde
            printf("Exit-Code: %d\n", WEXITSTATUS(status));                               
        } else if (WIFSIGNALED(status)) {                                                          
            // Durch WIFSIGNALLED(status) prüfen, ob der Kindprozess durch ein Signal beendet wurde
            int signal = WTERMSIG(status);                                                  
            // WTERMSIG(status) gibt das Signal aus, das den Kindprozess beendet hat
            printf("Prozess durch Signal %d beendet: %s\n", signal, strsignal(signal));
            // strsignal(signal) gibt eine verständliche Bezeichnung des Signals aus, das den Kindprozess beendet hat
        } else if (WIFSTOPPED(status)) {
            // Durch WIFSTOPPED(status) prüfen, ob der Kindprozess gestoppt wurde
            // Ausgabe des Signals, das den Kindprozess gestoppt hat durch WSTOPSIG(status)
            printf("Kindprozess wurde durch Signal %d gestoppt\n", WSTOPSIG(status));
        } 
    }
    exit(EXIT_SUCCESS);                                                                     
    // Programm erfolgreich beenden
}