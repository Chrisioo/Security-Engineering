#include <stdio.h>                              // Für Ein-/Ausgabefunktionen wie printf() und fprintf()
#include <stdlib.h>                             // Für Standardbibliotheksfunktionen wie exit() und EXIT_FAILURE/SUCCESS
#include <time.h>                               // Für Zeitfunktionen wie time()
#include <sys/shm.h>                            // Für Systemaufrufe zur Verwaltung von Shared Memory       
#include <sys/sem.h>                            // Für Systemaufrufe zur Verwaltung von Semaphoren
#include <unistd.h>                             // Für POSIX-Standardfunktionen wie fork()
#include <sys/wait.h>                           // Für Systemaufrufe zur Prozesskontrolle wie wait()

#define N_DATA 2000000                          // Anzahl der zu generierenden Zufallszahlen
#define N_SHARED 20000                          // Anzahl der Zufallszahlen, die pro Schritt in den Shared Memory geschrieben werden

/**
 * Union für Semaphoren
 * Unionen dienen dazu, Speicherplatz zu sparen, da nur ein Feld zur gleichen Zeit verwendet wird
 * Gleichzeitig werden alle Felder an der gleichen Speicheradresse gespeichert
 */
union semaphor {
    int value;
    struct semid_ds *buffer;
    unsigned short *array;
};

/**
 * Funktion zum Ausführen von semop
 * @param sem_id ID des Semaphors
 * @param sem_num Nummer des Semaphors
 * @param sem_op Operation, die auf den Semaphor angewendet wird
 */
void ex_semop(int sem_id, int sem_num, int sem_op) {
    // Struktur für semop, die die Nummer des Semaphors, die Operation und die Anzahl der Operationen enthält
    struct sembuf sop;
    // Setzen der Nummer des Semaphors
    sop.sem_num = sem_num;
    // Setzen der Operation
    sop.sem_op = sem_op;
    if (semop(sem_id, &sop, 1) == -1) {
        // Prüfen, ob semop erfolgreich war
        // Dabei wird geprüft, ob der Rückgabewert -1 ist, was bedeutet, dass ein Fehler aufgetreten ist
        // Z.B. wenn der Semaphor nicht existiert oder die Operation nicht erlaubt ist
        // Ausgabe einer Fehlermeldung
        perror("semop");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    }
}

/**
 * Hauptfunktion der Anwendung
 */
int main() {
    // Anlegen eines Arrays für die Zufallszahlen mit der Größe N_DATA
    int numbers[N_DATA];
    // Füllen des Arrays mit Nullen
    for (int i = 0; i < N_DATA; i++) {
        numbers[i] = 0;
    }

    // Erzeugen eines Shared Memory Segments für die Zufallszahlen
    // shm_id: ID des Shared Memory Segments
    // shmget gibt die ID des Shared Memory Segments mit den gegebenen Parametern zurück
    // IPC_PRIVATE: Erzeugt ein neues, einzigartiges Shared Memory Segment
    // IPC_CREAT: Erzeugt ein neues Shared Memory Segment, wenn noch kein Segment mit der gegebenen ID existiert
    // 0666: Zugriffsrechte für das Shared Memory Segment (Lesen und Schreiben für alle Benutzer)
    int shm_id = shmget(IPC_PRIVATE, (sizeof(int) * N_SHARED), IPC_CREAT | 0666);
    // Prüfen, ob shmget erfolgreich war
    // Kann z.B. fehlschlagen, wenn der Speicher nicht ausreicht oder die Zugriffsrechte nicht gesetzt werden können
    if (shm_id == -1) {
        // Ausgabe einer Fehlermeldung
        perror("shmget");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    }
    // Erzeugen eines Shared Memory Segments für die Semaphoren
    // sem_id: ID des Semaphors
    // semget gibt die ID des Semaphors mit den gegebenen Parametern zurück
    // IPC_PRIVATE: Erzeugt einen neuen, einzigartigen Semaphor
    // IPC_CREAT: Erzeugt einen neuen Semaphor, wenn noch kein Semaphor mit der gegebenen ID existiert
    // 2: Anzahl der Semaphoren im Semaphor-Set
    // 0666: Zugriffsrechte für das Semaphor-Set (Lesen und Schreiben für alle Benutzer)
    int sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    // Prüfen, ob semget erfolgreich war
    // Kann z.B. fehlschlagen, wenn der Speicher nicht ausreicht oder die Zugriffsrechte nicht gesetzt werden können
    if (sem_id == -1) {
        // Ausgabe einer Fehlermeldung
        perror("semget");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    }

    // Initialisieren einer Semaphor-Union 
    union semaphor sem_union;
    // Setzen des Wertes des ersten Semaphors auf 0
    sem_union.value = 0;
    // Prüfen, ob semctl erfolgreich war
    // Kann z.B. fehlschlagen, wenn der Semaphor nicht existiert oder die Operation nicht erlaubt ist
    // semid: ID des Semaphors
    // 0: Wert des Semaphors
    // SETVAL: Setzen des Wertes des Semaphors
    // sem_union: Union mit dem Wert des Semaphors
    // semctl gibt -1 zurück, wenn ein Fehler aufgetreten ist
    // Kann z.B. fehlschlagen, wenn der Semaphor nicht existiert oder die Operation nicht erlaubt ist
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        // Ausgabe einer Fehlermeldung, wenn Semaphor 0 nicht erfolgreich initialisiert wurde
        perror("Fehler beim Initialisieren von S0");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    }

    // Setzen des Wertes des zweiten Semaphors auf 1
    sem_union.value = 1;
    // Prüfen, ob semctl erfolgreich war
    // Kann z.B. fehlschlagen, wenn der Semaphor nicht existiert oder die Operation nicht erlaubt ist
    // semid: ID des Semaphors
    // 1: Wert des Semaphors
    // SETVAL: Setzen des Wertes des Semaphors
    // sem_union: Union mit dem Wert des Semaphors  
    // semctl gibt -1 zurück, wenn ein Fehler aufgetreten ist
    // Kann z.B. fehlschlagen, wenn der Semaphor nicht existiert oder die Operation nicht erlaubt ist   
    if (semctl(sem_id, 1, SETVAL, sem_union) == -1) {
        // Ausgabe einer Fehlermeldung, wenn Semaphor 1 nicht erfolgreich initialisiert wurde
        perror("Fehler beim Initialisieren von S1");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    }

    // Erzeugen eines Kindprozesses durch fork, pid enthält die Prozess-ID des Kindprozesses    
    pid_t pid = fork();

    // Prüfen, ob fork erfolgreich war
    // Falls pid < 0, ist ein Fehler aufgetreten
    if (pid < 0) {
        // Ausgabe einer Fehlermeldung
        perror("fork");
        // Programm wird mit Fehlercode EXIT_FAILURE beendet
        exit(EXIT_FAILURE);
    // Falls pid == 0, handelt es sich um den Kindprozess
    } else if (pid == 0) {
        // Anhängen des Shared Memory Segments an den Kindprozess durch shmat
        // shared_memory: Pointer auf den Speicherbereich des Shared Memory Segments
        // shm_id: ID des Shared Memory Segments
        // NULL: Adresse des Speicherbereichs, an den das Shared Memory Segment angehängt werden soll
        // 0: Flags für das Anhängen des Shared Memory Segments
        int* shared_memory = shmat(shm_id, NULL, 0);
        // Prüfen, ob shmat erfolgreich war
        if (shared_memory == (int*)(-1)) {
            // Ausgabe einer Fehlermeldung
            perror("shmat");
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }


        printf("Array des Kindprozesses nach fork():\n");

        // Ausgabe des Arrays des Kindprozesses nach fork
        for (int i = 0; i < N_DATA; i++) {
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        // Durchlaufen des Arrays und Schreiben der Zufallszahlen in den Shared Memory
        // N_DATA / N_SHARED: Anzahl der Schritte, die benötigt werden, um alle Zufallszahlen in den Shared Memory zu schreiben
        for (int i = 0; i < (N_DATA / N_SHARED); i++) {
            // Ausführen der Operationen für den ersten Semaphor  
            ex_semop(sem_id, 0, -1);

            // Schreiben der Zufallszahlen in den Shared Memory
            for (int j = 0; j < N_SHARED; j++) {
                numbers[i * N_SHARED + j] = shared_memory[j];
            }

            // Ausführen der Operationen für den zweiten Semaphor
            ex_semop(sem_id, 1, 1);
        }

        printf("Array des Kindprozesses nach Schreiben in Shared Memory:\n");

        // Ausgabe des Arrays des Kindprozesses nach Schreiben in den Shared Memory
        for (int i = 0; i < N_DATA; i++) {
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        // Abhängen des Shared Memory Segments vom Kindprozess
        if (shmdt(shared_memory) == -1) {
            // Ausgabe einer Fehlermeldung falls shmdt fehlschlägt
            perror("shmdt");
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }
    // Falls pid > 0, handelt es sich um den Elternprozess
    } else {
        // Anhängen des Shared Memory Segments an den Elternprozess durch shmat
        int* shared_memory = shmat(shm_id, NULL, 0);
        // Prüfen, ob shmat erfolgreich war
        if (shared_memory == (int*)(-1)) {
            // Ausgabe einer Fehlermeldung
            perror("shmat");
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }

        printf("Array des Elternprozesses nach fork():\n");

        // Ausgabe des Arrays des Elternprozesses nach fork
        for (int i = 0; i < N_DATA; i++) {
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        // Generieren von Zufallszahlen für den Elternprozess
        // time(NULL): Gibt die aktuelle Zeit zurück
        srand48(time(NULL));
        for (int i = 0; i < N_DATA; i++) {
            // Generieren einer Zufallszahl und Speichern in numbers
            numbers[i] = (int)lrand48();
        }

        printf("Vom Elternprozess generierte Zufallszahlen:\n");

        // Ausgabe der vom Elternprozess generierten Zufallszahlen
        for (int i = 0; i < N_DATA; i++) {
            printf("%d, ", numbers[i]);
        }
        printf("\n\n");

        // Durchlaufen des Arrays und ausführen der Operationen für die Semaphoren
        for (int i = 0; i < (N_DATA / N_SHARED); i++) {
            // Ausführen der Operationen für den zweiten Semaphor
            ex_semop(sem_id, 1, -1);
            // Schreiben der Zufallszahlen in den Shared Memory
            for (int j = 0; j < N_SHARED; j++) {
                shared_memory[j] = numbers[i * N_SHARED + j];
            }
            // Ausführen der Operationen für den ersten Semaphor
            ex_semop(sem_id, 0, 1);
        }
        // Warten auf den Kindprozess
        wait(NULL);

        // Abhängen des Shared Memory Segments vom Elternprozess
        if (shmdt(shared_memory) == -1) {
            // Ausgabe einer Fehlermeldung falls shmdt fehlschlägt
            perror("shmdt-Fehler im Elternprozess"); 
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }
        
        // Löschen des Shared Memory Segments
        // shm_id: ID des Shared Memory Segments
        // IPC_RMID: Flag zum Löschen des Shared Memory Segments
        // NULL: Pointer auf die Struktur, die Informationen über das Shared Memory Segment enthält
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            // Ausgabe einer Fehlermeldung falls shmctl fehlschlägt
            perror("Fehler beim Löschen des Shared Memory Segments");
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }

        // Löschen des Semaphor-Sets
        // sem_id: ID des Semaphor-Sets
        // IPC_RMID: Flag zum Löschen des Semaphor-Sets
        // 0: Pointer auf die Struktur, die Informationen über das Semaphor-Set enthält
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            // Ausgabe einer Fehlermeldung falls semctl fehlschlägt
            perror("Fehler beim Löschen des Semaphor-Sets");
            // Programm wird mit Fehlercode EXIT_FAILURE beendet
            exit(EXIT_FAILURE);
        }
    }
    // Programm wird mit Erfolg beendet
    exit(EXIT_SUCCESS);
}