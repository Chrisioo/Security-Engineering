#include <stdio.h>           // Einbinden der Standard Input/Output Bibliothek
#include <sys/stat.h>        // Einbinden von Funktionen und Strukturen für Dateistatistiken, wie z.B. stat, lstat, fstat
#include <pwd.h>             // Einbinden von Funktionen und Strukturen zur Benutzerverwaltung
#include <time.h>            // Einbinden von Funktionen und Strukturen zur Zeitverwaltung

// Ausgabe der Informationen zu einer vom Benutzer angegebenen Datei
void print_file_info(const char *filename) {                                                                                // Definition der Funktion zur Ausgabe von Dateiinformationen
    struct stat file_stat;                                                                                                  // Deklaration einer Struktur zum Speichern der Dateiinformationen
    if (stat(filename, &file_stat) == -1) {                                                                                 // Abrufen der Dateiinformationen und Check auf Fehler
        perror("");                                                                                                         // Ausgabe einer Fehlermeldung, falls stat fehlschlägt
        return;                                                                                                             // Beenden der Funktion im Fehlerfall
    }

    // Bestimmen und Ausgeben des Dateityps
    printf("Dateityp: ");                                   
    if (S_ISREG(file_stat.st_mode))                                                                                         // Check, ob es sich um eine reguläre Datei handelt
        printf("Reguläre Datei\n");                         
    else if (S_ISDIR(file_stat.st_mode))                                                                                    // Check, ob es sich um ein Verzeichnis handelt
        printf("Verzeichnis\n");                            
    else if (S_ISLNK(file_stat.st_mode))                                                                                    // Check, ob es sich um einen Link handelt
        printf("Link\n");                                   
    else if (S_ISFIFO(file_stat.st_mode))                                                                                   // Check, ob es sich um eine FIFO/pipe handelt
        printf("FIFO/pipe\n");                              
    else if (S_ISCHR(file_stat.st_mode))                                                                                    // Check, ob es sich um eine Char-Datei handelt
        printf("Char-Datei\n");                                   
    else if (S_ISSOCK(file_stat.st_mode))                                                                                   // Check, ob es sich um einen Socket handelt
        printf("Socket\n");                                                                                         
    else                                                                                                                    // Wenn keiner der obigen Fälle zutrifft, Ausgabe "Unbekannter Dateityp"
        printf("Unbekannter Dateityp\n");                                                                          

    // Ausgabe der Benutzer- und Gruppen-ID des Dateieigentümers
    printf("User ID: (%d)\n", file_stat.st_uid);                                                                    
    printf("Group ID: (%d)\n", file_stat.st_gid);                                                                   

    // Bestimmen des Namens des Benutzers, der die Datei besitzt
    struct passwd* user_name = getpwuid(file_stat.st_uid);                                                                  // Abrufen der Benutzerinformationen basierend auf der Benutzer-ID
    printf("Benutzername: %s\n", user_name->pw_name);                                                                       // Ausgabe des Benutzernamens
                                                                    
    // Ausgabe der Zugangsberechtigungen im oktalen Format
    printf("Zugangsberechtigungen (oktal): %o\n", file_stat.st_mode & 0777);    

    // Ausgabe Zeitpunkt letzter Zugriff auf die Datei
    struct tm* access_time = localtime(&file_stat.st_atime);                                                                // Umwandeln des Zeitpunkts des letzten Zugriffs in eine lokale Zeitstruktur
    char access_time_string[40];                                                                                            // Deklaration eines Char-Arrays zur Speicherung der formatierten Zeit
    strftime(access_time_string, sizeof(access_time_string), "%a %b %d %H:%M:%S %Z %Y", access_time);                       // Formatieren des Zeitpunkts des letzten Zugriffs
    printf("Letzter Zugriff auf Datei: %s\n", access_time_string);                                                          // Ausgabe des Zeitpunkts des letzten Zugriffs

    // Ausgabe Zeitpunkt letzte Änderung der Inode-Informationen
    struct tm* change_time = localtime(&file_stat.st_ctime);                                                                // Umwandeln des Zeitpunkts der letzten Änderung der Inode-Informationen in eine lokale Zeitstruktur
    char change_time_string[40];                                                                                            // Deklaration eines Char-Arrays zur Speicherung der formatierten Zeit
    strftime(change_time_string, sizeof(change_time_string), "%a %b %d %H:%M:%S %Z %Y", change_time);                       // Formatieren des Zeitpunkts der letzten Änderung der Inode-Informationen
    printf("Letzte Änderung an Inode-Informationen: %s\n", change_time_string);                                             // Ausgabe des Zeitpunkts der letzten Änderung der Inode-Informationen

    // Ausgabe Zeitpunkt letzte Änderung der Datei
    struct tm* modification_time = localtime(&file_stat.st_mtime);                                                          // Umwandeln des Zeitpunkts der letzten Änderung der Datei in eine lokale Zeitstruktur
    char modification_time_string[40];                                                                                      // Deklaration eines Char-Arrays zur Speicherung der formatierten Zeit
    strftime(modification_time_string, sizeof(modification_time_string), "%a %b %d %H:%M:%S %Z %Y", modification_time);     // Formatieren des Zeitpunkts der letzten Änderung der Datei
    printf("Letzte Veränderung der Datei: %s\n", modification_time_string);                                                 // Ausgabe des Zeitpunkts der letzten Änderung der Datei


    #ifdef st_birthtime
        struct tm* creation_time = localtime(&file_stat.st_birthtime);                                                      // Umwandeln des Zeitpunkts der Erstellung der Datei in eine lokale Zeitstruktur
        char creation_time_string[40];                                                                                      // Deklaration eines Char-Arrays zur Speicherung der formatierten Zeit
        strftime(creation_time_string, sizeof(creation_time_string), "%a %b %d %H:%M:%S %Z %Y", creation_time);             // Formatieren des Zeitpunkts der Erstellung der Datei
        printf("Erstellungszeitpunkt der Datei: %s\n", creation_time_string);                                               // Ausgabe des Zeitpunkts der Erstellung der Datei
    #else 
        printf("Erstellungszeitpunkt der Datei: Nicht verfügbar\n");                                                        // Ausgabe, dass der Erstellungszeitpunkt nicht verfügbar ist
    #endif
}

int main(int argc, char *argv[]) {                                                                                          // Definition der Hauptfunktion
    if (argc < 2) {                                                                                                         // Check, ob der Benutzer mindestens einen Dateinamen angegeben hat
        printf("Zur Nutzung bitte mindestens einen Dateinamen angeben.\n");                                                 // Ausgabe einer Fehlermeldung                                        
        return -1;                                                                                                          // Rückgabe eines Fehlercodes
    }
    for (int i = 1; i < argc; i++) {                                                                                        // Schleife über alle angegebenen Dateinamen
        printf("Dateiname: %s\n", argv[i]);                                                                                 // Ausgabe des aktuellen Dateinamens
        print_file_info(argv[i]);                                                                                           // Aufruf der Funktion zur Ausgabe der Dateiinformationen
        printf("\n");                                                                                                       // Ausgabe einer Leerzeile zur Trennung der Informationen
    }
    return 0;                                                                                                               // Erfolgreiches Beenden des Programms
}