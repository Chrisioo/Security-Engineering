#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// Ausgabe der Informationen zu einer vom Benutzer angegebenen Datei
void print_file_info(const char *filename) {                
    struct stat file_stat;                                 
    if (stat(filename, &file_stat) == -1) {                 
        perror("stat");                                                            
        return;                                                     
    }

    // Bestimmen des Dateityps
    printf("Dateityp: ");
    if (S_ISREG(file_stat.st_mode))
        printf("Reguläre Datei\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Verzeichnis\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Link\n");
    else if (S_ISFIFO(file_stat.st_mode))
        printf("FIFO/pipe\n");
    else if (S_ISCHR(file_stat.st_mode))
        printf("Char\n");
    else if (S_ISSOCK(file_stat.st_mode))
        printf("Socket\n");
    else
        printf("Unbekannter Dateityp\n");

    // Bestimmen der User- und Group-ID
    printf("User ID: (%d)\n", file_stat.st_uid);
    printf("Group ID: (%d)\n", file_stat.st_gid);

    // Bestimmen des Namens des Benutzers, der die Datei besitzt
    struct passwd* pw = getpwuid(file_stat.st_uid);         
    if (pw != NULL)                                         
        printf("Benutzername: %s\n", pw->pw_name);             

    // Bestimmen des Namens der Gruppe, die die Datei besitzt
    struct group* gr = getgrgid(file_stat.st_gid);          
    if (gr != NULL)                                         
        printf("Gruppenname: %s\n", gr->gr_name);          

    // Ausgabe der Zugangsberechtigungen
    printf("Zugangsberechtigungen (oktal): %o\n", file_stat.st_mode & 0777);

    // Ausgabe Zeitpunkt letzter Zugriff auf die Datei
    struct tm* atime = localtime(&file_stat.st_atime);
    char atime_str[20];
    strftime(atime_str, sizeof(atime_str), "%d.%m.%Y %H:%M:%S", atime);
    printf("Letzter Zugriff auf Datei: %s\n", atime_str);

    // Ausgabe Zeitpunkt letzte Änderung der Inode-Informationen
    struct tm* ctime = localtime(&file_stat.st_ctime);
    char ctime_str[20];
    strftime(ctime_str, sizeof(ctime_str), "%d.%m.%Y %H:%M:%S", ctime);
    printf("Letzte Änderung an Inode-Informationen: %s\n", ctime_str);

    // Last modification time
    struct tm* mtime = localtime(&file_stat.st_mtime);
    char mtime_str[20];
    strftime(mtime_str, sizeof(mtime_str), "%d.%m.%Y %H:%M:%S", mtime);
    printf("Letzte Veränderung der Datei: %s\n", mtime_str);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename1> [<filename2> ...]\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        printf("Dateiname: %s\n", argv[i]);
        print_file_info(argv[i]);
        printf("\n");
    }
    return 0;
}