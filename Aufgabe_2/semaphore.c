#include <stdio.h>                      
#include <stdlib.h>                     
#include <unistd.h>                     
#include <sys/types.h>                  
#include <sys/ipc.h>                    // Einbindung der Bibliothek für Interprozesskommunikation (IPC)
#include <sys/shm.h>                    // Einbindung der Bibliothek für Shared Memory
#include <sys/sem.h>                    // Einbindung der Bibliothek für Semaphoren
#include <sys/wait.h>                   // Einbindung der Bibliothek für Prozesssteuerung und Warten auf Prozesse
#include <time.h>                       

#define N_DATA 2000000                  // Anzahl der Daten, die erzeugt und konsumiert werden
#define N_SHARED 2000                   // Größe des Shared Memory Puffers

/**
 * Semaphore sind globale Signalflags im System, mit denen sich gegenseitig ausschließende Ressourcen festgelegt werden können.
 * Sie können dazu genutzt werden, bestimmte Prozesse warten zu lassen, bis andere Prozesse abgeschlossen sind.
 */

// Definition einer Union für die Semaphore-Operationen
// Bei Unions wird der selbe Speicherplatz für alle Elemente genutzt
union semunion {
    int val;                            // Wert, der über SETVAL gesetzt wird
    struct semid_ds *buf;               // Buffer für IPC_STAT und IPC_SET
    unsigned short *array;              // Array für GETALL und SETALL
};

// Funktion für die P-Operation (Warten) auf einer Semaphore
// Signal -1 auf einer Semaphore bedeutet, dass der Wert der Semaphore um 1 verringert wird und der Prozess auf die Ressource wartet
void semaphore_wait(int sem_id, int sem_num) {
    struct sembuf sembuf;                   // Struktur für Semaphore-Operationen
    sembuf.sem_num = sem_num;               // Nummer der Semaphore im Set
    sembuf.sem_op = -1;                     // P-Operation (decrement)
    semop(sem_id, &sembuf, 1);              // Ausführen der Semaphore-Operation
}

// Funktion für die V-Operation (Signalisieren) auf einer Semaphore
// Signal 1 auf einer Semaphore bedeutet, dass der Wert der Semaphore um 1 erhöht wird und der Prozess die Ressource freigibt
void semaphore_signal(int sem_id, int sem_num) {
    struct sembuf sembuf;                   // Struktur für Semaphore-Operationen
    sembuf.sem_num = sem_num;               // Nummer der Semaphore im Set
    sembuf.sem_op = 1;                      // V-Operation (increment)
    semop(sem_id, &sembuf, 1);              // Ausführen der Semaphore-Operation
}

// Funktion zum Bereinigen von Shared Memory und Semaphoren
// smh_id: ID des Shared Memory Segments
// sem_id: ID des Semaphore Sets    
void cleanup(int shm_id, int sem_id) {
    // Shared Memory Segment löschen
    // Prüfen, ob das Löschen des Shared Memory erfolgreich war
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) { 
        perror("shmctl");               // Fehlermeldung, falls das Löschen fehlschlägt
        exit(EXIT_FAILURE);             // Programmabbruch bei Fehler
    }

    // Semaphoren löschen
    // Prüfen, ob das Löschen der Semaphoren erfolgreich war
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl");               // Fehlermeldung, falls das Löschen fehlschlägt
        exit(EXIT_FAILURE);             // Programmabbruch bei Fehler
    }
}

int main() {
    int shm_id, sem_id;                 // IDs für Shared Memory und Semaphoren
    int *shared_memory;                 // Pointer auf Shared Memory
    pid_t pid;                          // Prozess-ID, Datentyp pid_t / Integer
    key_t key = IPC_PRIVATE;            // Generieren eines eindeutigen Schlüssels mit IPC_PRIVATE, Datentyp key_t / Integer

    // Shared Memory anlegen
    // key: Schlüssel für das Shared Memory Segment
    // N_SHARED * sizeof(int): Größe des Shared Memory Segments, sizeof(int) = 4 Bytes
    // 0666 | IPC_CREAT: Zugriffsrechte und Flags für das Anlegen des Shared Memory Segments
    shm_id = shmget(key, N_SHARED * sizeof(int), 0666 | IPC_CREAT);
    // Prüfen, ob das Anlegen des Shared Memory erfolgreich war
    if (shm_id < 0) {
        perror("shmget");               // Fehlermeldung, falls das Anlegen fehlschlägt, in diesem Fall wird ein Wert < 0 zurückgegeben
        exit(EXIT_FAILURE);             // Programmabbruch bei Fehler
    }

    // Shared Memory an den Adressraum anhängen
    // (int *): Typ des Shared Memory Segments (int-Array)
    // shmat: Funktion zum Anhängen des Shared Memory Segments
    // shm_id: ID des Shared Memory Segments
    // NULL: Adresse, an die das Shared Memory Segment angehängt werden soll
    // 0: Flags für das Anhängen des Shared Memory Segments
    shared_memory = (int *)shmat(shm_id, NULL, 0);
    // Prüfen, ob das Anhängen des Shared Memory erfolgreich war
    if (shared_memory == (int *) -1) {
        perror("shmat");                // Fehlermeldung, falls das Anhängen fehlschlägt, in diesem Fall wird ein Wert -1 zurückgegeben
        exit(EXIT_FAILURE);             // Programmabbruch bei Fehler
    }

    // Semaphoren anlegen
    // key: Schlüssel für das Semaphore Set
    // 2: Anzahl der Semaphoren im Set
    // 0666 | IPC_CREAT: Zugriffsrechte und Flags für das Anlegen des Semaphore Sets
    sem_id = semget(key, 2, 0666 | IPC_CREAT);
    // Prüfen, ob das Anlegen des Semaphore Sets erfolgreich war
    if (sem_id < 0) {
        perror("semget");               // Fehlermeldung, falls das Anlegen fehlschlägt, in diesem Fall wird ein Wert < 0 zurückgegeben
        shmdt(shared_memory);           // Shared Memory ablösen
        exit(EXIT_FAILURE);             // Programmabbruch bei Fehler
    }

    // Semaphoren initialisieren
    // sem_union: Union für die Semaphore-Operationen
    union semunion sem_union;
    sem_union.val = 0;                  // Initial S1: Lesen verboten
    // semctl: Funktion für Semaphore-Operationen, wird zum setzen eines Wertes verwendet
    // sem_id: ID des Semaphore Sets
    // 0: Nummer der Semaphore im Set
    // SETVAL: Operation zum Setzen des Werts
    // sem_union: Union für die Semaphore-Operationen
    // Prüfen, ob das Setzen des Werts erfolgreich war
    if (semctl(sem_id, 0, SETVAL, sem_union) < 0) {
        perror("Fehler beim Initialisieren von Semaphore 0");   // Fehlermeldung, falls das Initialisieren fehlschlägt, in diesem Fall wird ein Wert < 0 zurückgegeben
        exit(EXIT_FAILURE);                                     // Programmabbruch bei Fehler
    }

    // semctl: Funktion für Semaphore-Operationen, wird zum setzen eines Wertes verwendet
    // sem_id: ID des Semaphore Sets
    // 1: Nummer der Semaphore im Set
    // SETVAL: Operation zum Setzen des Werts
    // sem_union: Union für die Semaphore-Operationen
    // Prüfen, ob das Setzen des Werts erfolgreich war
    sem_union.val = 1;                  // Initial S2: Schreiben erlaubt
    if (semctl(sem_id, 1, SETVAL, sem_union) < 0) {
        perror("Fehler beim Initialisieren von Semaphore 1");   // Fehlermeldung, falls das Initialisieren fehlschlägt, in diesem Fall wird ein Wert < 0 zurückgegeben
        exit(EXIT_FAILURE);                                     // Programmabbruch bei Fehler
    }

    // Prozess P1 erzeugt den Kindprozess P2
    // pid ist 0 im Kindprozess und die Prozess-ID des Kindprozesses im Elternprozess   
    pid = fork();
    // Prüfen, ob das Erzeugen des Prozesses erfolgreich war
    if (pid < 0) {                                              // Wenn pid < 0, dann ist ein Fehler aufgetreten    
        perror("fork");                                         // Fehlermeldung, falls das Erzeugen des Prozesses fehlschlägt
        exit(EXIT_FAILURE);                                     // Programmabbruch bei Fehler
    } else if (pid == 0) {                                      // Wenn pid = 0, dann handelt es sich um den Kindprozess    
        // Kindprozess P2 (Verbraucher)
        // Schleife über alle Daten in Schritten der Größe N_SHARED
        for (int i = 0; i < N_DATA; i += N_SHARED) {
            semaphore_wait(sem_id, 0);  // Warten auf Freigabe durch P1 (S1)
            // Schleife über alle Daten im Shared Memory Puffer
            for (int j = 0; j < N_SHARED; ++j) {
                // Prüfen, ob nur so viele Daten gelesen werden, wie vorhanden sind
                if (i + j < N_DATA) {
                    // Ausgabe der gelesenen Daten
                    printf("P2 liest auf Position [%d]: %d\n", i + j + 1, shared_memory[j]);
                }
            }

            semaphore_signal(sem_id, 1); // Signalisieren, dass P2 fertig ist (S2)
        }

        // Shared Memory ablösen
        shmdt(shared_memory);
        exit(EXIT_SUCCESS);             // Erfolgreiches Programmende
    } else {
        // Vaterprozess P1 (Erzeuger)
        // Initialisieren des Zufallszahlengenerators
        // Wird mit der aktuellen Zeit initialisiert, um bei jedem Programmstart unterschiedliche Zufallszahlen zu erhalten
        srand48(time(NULL));            
        // Schleife über alle Daten in Schritten der Größe N_SHARED
        for (int i = 0; i < N_DATA; i += N_SHARED) {
            semaphore_wait(sem_id, 1);  // Warten auf Freigabe durch P2 (S2)
            // Schleife über alle Daten im Shared Memory Puffer
            for (int j = 0; j < N_SHARED; ++j) {
                if (i + j < N_DATA) {
                    // Prüfen, ob nur so viele Daten geschrieben werden, wie Platz im Shared Memory Puffer ist
                    shared_memory[j] = lrand48();   // Zufällige Zahl schreiben
                    // Ausgabe der geschriebenen Daten
                    printf("P1 schreibt auf Position [%d]: %d\n", i + j + 1, shared_memory[j]);
                }
            }

            semaphore_signal(sem_id, 0); // Signalisieren, dass P1 fertig ist (S1)
        }

        // Auf den Kindprozess warten
        wait(NULL);

        // Shared Memory ablösen
        shmdt(shared_memory);

        // Cleanup
        cleanup(shm_id, sem_id);
    }
    exit(EXIT_SUCCESS);                // Erfolgreiches Programmende
}