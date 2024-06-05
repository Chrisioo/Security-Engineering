#include <stdio.h>                                                                                              // Standard Ein-/Ausgabe Bibliothek
#include <stdlib.h>                                                                                             // Standard Bibliothek, genutzt für EXIT_SUCCESS und EXIT_FAILURE
#include <sys/stat.h>                                                                                           // Bibliothek für Dateiinformationen, genutzt für struct stat und lstat()               
#include <time.h>                                                                                               // Bibliothek für Zeitfunktionen     
#include <pwd.h>                                                                                                // Bibliothek für Benutzerinformationen 

void print_file_info (const char *filename) {
       struct stat file_info;                                                                                   // Struktur für Dateiinformationen
       if (lstat(filename, &file_info) == -1) {                                                                 // lstat() statt stat(), um Symbolic Links zu folgen                                  
              perror("lstat");                                                                                  // Fehlermeldung, falls lstat() fehlschlägt          
              exit(EXIT_FAILURE);                                                                               // Programm beenden
       }

       printf("Dateityp: ");       
       switch (file_info.st_mode & __S_IFMT) {                                                                  // Dateityp bestimmen
              case __S_IFREG: printf("Reguläre Datei\n");      break;                                           // Ausgabe des Dateityps wenn reguläre Datei
              case __S_IFDIR: printf("Verzeichnis\n");         break;                                           // Ausgabe wenn Verzeichnis
              case __S_IFLNK: printf("Symbolischer Link\n");   break;                                           // Ausgabe wenn Symbolischer Link
              case __S_IFIFO: printf("FIFO / Pipe\n");         break;                                           // Ausgabe wenn FIFO / Pipe
              case __S_IFCHR: printf("Char-Datei\n");          break;                                           // Ausgabe wenn Char-Datei
              case __S_IFSOCK: printf("Socket\n");             break;                                           // Ausgabe wenn Socket
              default: printf("Unbekannter Dateityp\n");       break;                                           // Ausgabe wenn unbekannter Dateityp
       }

       printf("Benutzer-ID: %d\n", file_info.st_uid);                                                           // Benutzer-ID ausgeben
       printf("Gruppen-ID: %d\n", file_info.st_gid);                                                            // Gruppen-ID ausgeben
       printf("Benutzername: %s\n", getpwuid(file_info.st_uid)->pw_name);                                       // Benutzername ausgeben
       printf("Zugangsberechtigungen (oktal): %o\n", file_info.st_mode & 0777);                                 // Zugangsberechtigungen in oktal ausgeben

       // Zugriffszeit, Änderungszeit und Inode-Änderungszeit, mussten neu formatiert werden
       struct tm *atime_date = localtime(&file_info.st_atime);                                                  // Anlegen von struct tm für Zugriffszeit
       char atime_str[80];                                                                                      // Anlegen von char Array für formatierte Zugriffszeit                                
       strftime(atime_str, sizeof(atime_str), "%a %b %d %H:%M:%S %Z %Y", atime_date);                           // Formatierung der Zugriffszeit        
       printf("Zeitpunkt letzter Zugriff: %s\n", atime_str);                                                    // Ausgabe der formatierten Zugriffszeit

       struct tm *ctime_date = localtime(&file_info.st_ctime);                                                  // Anlegen von struct tm für Inode-Änderungszeit
       char ctime_str[80];                                                                                      // Anlegen von char Array für formatierte Inode-Änderungszeit                                            
       strftime(ctime_str, sizeof(ctime_str), "%a %b %d %H:%M:%S %Z %Y", ctime_date);                           // Formatierung der Inode-Änderungszeit
       printf("Zeitpunkt letzte Inode-Änderung: %s\n", ctime_str);                                              // Ausgabe der formatierten Inode-Änderungszeit

       struct tm *mtime_date = localtime(&file_info.st_mtime);                                                  // Anlegen von struct tm für Änderungszeit
       char mtime_str[80];                                                                                      // Anlegen von char Array für formatierte Änderungszeit
       strftime(mtime_str, sizeof(mtime_str), "%a %b %d %H:%M:%S %Z %Y", mtime_date);                           // Formatierung der Änderungszeit
       printf("Zeitpunkt letzte Dateiänderung: %s\n", mtime_str);                                               // Ausgabe der formatierten Änderungszeit

       #ifdef st_birthtime                                                                                      // Prüfen ob st_birthtime verfügbar ist
              struct tm *birthtime_date = localtime(&file_info.st_birthtime);                                   // Anlegen von struct tm für Erstellungszeit
              char birthtime_str[80];                                                                           // Anlegen von char Array für formatierte Erstellungszeit
              strftime(birthtime_str, sizeof(birthtime_str), "%a %b %d %H %M %S %Z %Y", birthtime_date);        // Formatierung der Erstellungszeit      
              printf("Zeitpunkt der Erstellung: %s\n", birthtime_str);                                          // Ausgabe der formatierten Erstellungszeit
       #else                                                                                                    // Wenn st_birthtime nicht verfügbar ist
              printf("Zeitpunkt der Erstellung: Nicht verfügbar\n");                                            // Ausgabe, dass Erstellungszeit nicht verfügbar ist
       #endif                                                                                                   // Ende der Prüfung auf st_birthtime
}

int main (int argc, char *argv[]) {
       if (argc < 2) {                                                                                          // Prüfen ob mindestens ein Dateiname angegeben wurde
              fprintf(stderr, "Bitte geben Sie mindestens einen Dateinamen an, z.B ./fileinfo datei.txt\n");    // Fehlermeldung, falls kein Dateiname angegeben wurde
              return EXIT_FAILURE;                                                                              // Programm beenden
       }
       for (int i = 1; i < argc; i++) {                                                                         // Schleife über alle Dateinamen
              printf("Dateiname: %s\n", argv[i]);                                                               // Ausgabe Dateiname
              print_file_info(argv[i]);                                                                         // Dateiinformationen ausgeben
              printf("\n");                                                                                     // Leerzeile
       } 
       return EXIT_SUCCESS;                                                                                     // Programm beenden, wenn alle Dateinamen abgearbeitet wurden
}