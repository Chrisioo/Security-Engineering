#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>

int main(int argc, char *argv[]) {
struct stat sb;
       if (argc != 2) {
              fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
              exit(EXIT_FAILURE);
       }
       if (lstat(argv[1], &sb) == -1) {
              perror("lstat");
              exit(EXIT_FAILURE);
       }

       printf("Dateityp: ");

       switch (sb.st_mode & S_IFMT) {
              case S_IFREG:  printf("Reguläre Datei\n");              break;
              case S_IFDIR:  printf("Verzeichnis\n");                 break;
              case S_IFLNK:  printf("Symbolischer Link\n");           break;
              case S_IFIFO:  printf("FIFO/pipe\n");                   break;
              case S_IFCHR:  printf("Char-Datei\n");                  break;
              case S_IFSOCK: printf("socket\n");                      break;
              default:       printf("Unbekannter Dateityp?\n");       break;
       }

       printf("Benutzer-ID: %ju\n", (uintmax_t) sb.st_uid);
       printf("Gruppen-ID: %ju\n", (uintmax_t) sb.st_gid);
       printf("Benutzername: %s\n", getpwuid(sb.st_uid)->pw_name);
       printf("Zugangsberechtigungen (oktal): %o\n", sb.st_mode & 0777);
       printf("Letzter Zugriff: %s", ctime(&sb.st_atime));
       printf("Letzte Änderung an Inode-Informationen: %s", ctime(&sb.st_ctime));
       printf("Letzte Änderung an Datei: %s", ctime(&sb.st_mtime));

       #ifdef st_birthtime
       printf("Erstellungszeitpunkt der Datei: %s", ctime(&sb.st_birthtime));
       #else
       printf("Erstellungszeitpunkt der Datei: Nicht verfügbar\n");
       #endif

       exit(EXIT_SUCCESS);
}