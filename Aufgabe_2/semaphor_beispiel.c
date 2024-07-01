#include <stdio.h> 																	// Einbinden der Standard-Ein-/Ausgabebibliothek
#include <stdlib.h> 																// Einbinden der Standard-Bibliothek für allgemeine Funktionen
#include <time.h> 																	// Einbinden der Zeit-Bibliothek für Zeitfunktionen
#include <sys/shm.h> 																// Einbinden der Bibliothek für Shared Memory Funktionen
#include <sys/sem.h> 																// Einbinden der Bibliothek für Semaphore Funktionen
#include <unistd.h> 																// Einbinden der Bibliothek für POSIX Betriebssystem-API
#include <sys/wait.h> 																// Einbinden der Bibliothek für Prozess-Synchronisation

#define N_DATA 2000000 																// Definieren der Anzahl der Daten
#define N_SHARED 20000 																// Definieren der Anzahl der gemeinsamen Daten

/**
 * Definition einer Union für Semaphore-Werte
 * Eine Union ermöglicht es, dass mehrere Variablen von unterschiedlichen Typen an der selben Adresse liegen können
 */
union semaphor {
    int value; 																		// Integer-Wert für den Semaphore
    struct semid_ds *buffer; 														// Puffer für semctl
    unsigned short *array; 															// Array für Semaphore-Werte
};


/**
 * Ausführung einer Semaphore-Operation
 * @param sem_id ID der Semaphore-Gruppe.
 * @param sem_num Anzahl der Semaphore in der Semaphore-Gruppe.
 * @param sem_op Operation, die auf dem Semaphor ausgeführt wird.
 */
void ex_semop(int sem_id, int sem_num, int sem_op) {
    struct sembuf sop; 																// Struktur für Semaphore-Operationen
    sop.sem_num = sem_num; 															// Nummer des Semaphors in der Semaphore-Gruppe
    sop.sem_op = sem_op;								 							// Operation, die auf dem Semaphor ausgeführt wird
    sop.sem_flg = 0; 																// Keine speziellen Flags gesetzt
    if (semop(sem_id, &sop, 1) == -1) { 											// Ausführen der Semaphore-Operation
        perror("semop error"); 														// Fehlermeldung ausgeben, falls semop fehlschlägt
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    }
}

int main() {
   
    int numbers[N_DATA]; 															// Array zur Speicherung der Daten erstellen, Grosse N_DATA
    for (int i = 0; i < N_DATA; i++) { 												// Initialisieren des Arrays mit Nullen
        numbers[i] = 0; 															// Setzen jedes Elements im Array auf 0
    }

    // Shared Memory und Semaphore-Set erstellen
    int shm_id = shmget(IPC_PRIVATE, (sizeof(int) * N_SHARED), IPC_CREAT | 0666); 	// Erstellen eines Shared Memory Segments
    if (shm_id == -1) { 															// Überprüfen auf Fehler beim Erstellen des Shared Memory
        perror("shmget error"); 													// Fehlermeldung ausgeben
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    }
    int sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666); 							// Erstellen eines Semaphore-Sets mit zwei Semaphoren
    if (sem_id == -1) { 															// Überprüfen auf Fehler beim Erstellen des Semaphore-Sets
        perror("semget error"); 													// Fehlermeldung ausgeben
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    }

    union semaphor sem_union; 															// Initialisieren einer Union für Semaphore
    sem_union.value = 0; 															// Setzen des Wertes auf 0
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) { 								// Initialisieren des ersten Semaphors mit 0
        perror("error initializing S0"); 											// Fehlermeldung ausgeben
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    }

    sem_union.value = 1; 															// Setzen des Wertes auf 1
    if (semctl(sem_id, 1, SETVAL, sem_union) == -1) { 								// Initialisieren des zweiten Semaphors mit 1
        perror("error initializing S1"); 											// Fehlermeldung ausgeben
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    }

    pid_t pid = fork(); 															// Erstellen eines Kindprozesses durch fork()

    if (pid < 0) { 																	// Überprüfen auf Fehler beim Fork
        perror("Fork fail\n"); 														// Fehlermeldung ausgeben
        exit(EXIT_FAILURE); 														// Programm beenden bei Fehler
    } else if (pid == 0) { 															// Kindprozess
        int* shared_memory = shmat(shm_id, NULL, 0);	 							// Anhängen des Shared Memory an den Kindprozess
        if (shared_memory == (int*)(-1)) { 											// Überprüfen auf Fehler beim Anhängen des Shared Memory
            perror("shmat error in child process"); 								// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }

        printf("Child process array after fork:\n");

        for (int i = 0; i < N_DATA; i++) { 											// Ausgeben des Arrays nach dem Fork
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        for (int i = 0; i < (N_DATA / N_SHARED); i++) { 							// Lesen von Daten aus dem Shared Memory
            ex_semop(sem_id, 0, -1); 												// Warten auf den Semaphore S0

            for (int j = 0; j < N_SHARED; j++) { 									// Kopieren der Daten vom Shared Memory ins lokale Array
                numbers[i * N_SHARED + j] = shared_memory[j];
            }

            ex_semop(sem_id, 1, 1); 												// Freigeben des Semaphore S1
        }

        printf("Child process array after read:\n");

        for (int i = 0; i < N_DATA; i++) { 											// Ausgeben des Arrays nach dem Lesen
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        if (shmdt(shared_memory) == -1) { 											// Trennen des Shared Memory vom Kindprozess
            perror("shmdt error in child process"); 								// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }

    } else { // Elternprozess
        int* shared_memory = shmat(shm_id, NULL, 0); 								// Anhängen des Shared Memory an den Elternprozess
        if (shared_memory == (int*)(-1)) { 											// Überprüfen auf Fehler beim Anhängen des Shared Memory
            perror("shmat error in parent process"); 								// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }

        printf("Parent process array after fork:\n");

        for (int i = 0; i < N_DATA; i++) { 											// Ausgeben des Arrays nach dem Fork
            printf("%d, ", numbers[i]);
        }
        printf("\n");

        srand48(time(NULL)); 														// Initialisieren des Zufallszahlengenerators
        for (int i = 0; i < N_DATA; i++) { 											// Generieren von Zufallszahlen
            numbers[i] = (int)lrand48();	
        }

        printf("Parent process generated numbers:\n");

        for (int i = 0; i < N_DATA; i++) { 											// Ausgeben der generierten Zufallszahlen
            printf("%d, ", numbers[i]);
        }
        printf("\n\n");

        for (int i = 0; i < (N_DATA / N_SHARED); i++) { 							// Schreiben von Daten ins Shared Memory
            ex_semop(sem_id, 1, -1); 												// Warten auf den Semaphore S1

            for (int j = 0; j < N_SHARED; j++) { 									// Kopieren der Daten vom lokalen Array ins Shared Memory
                shared_memory[j] = numbers[i * N_SHARED + j];
            }

            ex_semop(sem_id, 0, 1); 												// Freigeben des Semaphore S0
        }

        wait(NULL); 																// Warten auf das Ende des Kindprozesses

        if (shmdt(shared_memory) == -1) { 											// Trennen des Shared Memory vom Elternprozess
            perror("shmdt error in parent process"); 								// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }

        if (shmctl(shm_id, IPC_RMID, NULL) == -1) { 								// Löschen des Shared Memory Segments
            perror("Error deleting shm segment"); 									// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }

        if (semctl(sem_id, 0, IPC_RMID) == -1) { 									// Löschen des Semaphore-Sets
            perror("Error deleting sem set"); 										// Fehlermeldung ausgeben
            exit(EXIT_FAILURE); 													// Programm beenden bei Fehler
        }
    }

    exit(EXIT_SUCCESS); 															// Erfolgreiches Beenden des Programms
}
