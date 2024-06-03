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
    struct passwd* user_name = getpwuid(file_stat.st_uid);         
    if (user_name != NULL)                                         
        printf("Benutzername: %s\n", user_name->pw_name);           

    // Bestimmen des Namens der Gruppe, die die Datei besitzt
    struct group* group_name = getgrgid(file_stat.st_gid);          
    if (group_name != NULL)                                         
        printf("Gruppenname: %s\n", group_name->gr_name);          

    // Ausgabe der Zugangsberechtigungen
    printf("Zugangsberechtigungen (oktal): %o\n", file_stat.st_mode & 0777);

    // Ausgabe Zeitpunkt letzter Zugriff auf die Datei
    struct tm* access_time = localtime(&file_stat.st_atime);
    char access_time_string[20];
    strftime(access_time_string, sizeof(access_time_string), "%d.%m.%Y %H:%M:%S", access_time);
    printf("Letzter Zugriff auf Datei: %s\n", access_time_string);

    // Ausgabe Zeitpunkt letzte Änderung der Inode-Informationen
    struct tm* change_time = localtime(&file_stat.st_ctime);
    char change_time_string[20];
    strftime(change_time_string, sizeof(change_time_string), "%d.%m.%Y %H:%M:%S", ctime);
    printf("Letzte Änderung an Inode-Informationen: %s\n", change_time_string);

    // Ausgabe Zeitpunkt letzte Änderung der Datei
    struct tm* modification_time = localtime(&file_stat.st_mtime);
    char modification_time_string[20];
    strftime(modification_time_string, sizeof(modification_time_string), "%d.%m.%Y %H:%M:%S", modification_time);
    printf("Letzte Veränderung der Datei: %s\n", modification_time_string);
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