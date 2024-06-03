#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void print_file_info(const char *filename) {
    struct stat file_stat;
    struct passwd *pw;
    struct group *gr;

    // Using lstat to get the status of the symbolic link itself
    if (lstat(filename, &file_stat) == -1) {
        perror("lstat");
        return;
    }

    // Determine file type
    printf("File: %s\n", filename);
    printf("Type: ");
    if (S_ISREG(file_stat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Symbolic link\n");
    } else if (S_ISFIFO(file_stat.st_mode)) {
        printf("FIFO (pipe)\n");
    } else if (S_ISSOCK(file_stat.st_mode)) {
        printf("Socket\n");
    } else if (S_ISCHR(file_stat.st_mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(file_stat.st_mode)) {
        printf("Block device\n");
    } else {
        printf("Unknown\n");
    }

    // User ID and Group ID
    pw = getpwuid(file_stat.st_uid);
    gr = getgrgid(file_stat.st_gid);
    printf("Owner: %s (UID: %d)\n", pw ? pw->pw_name : "unknown", file_stat.st_uid);
    printf("Group: %s (GID: %d)\n", gr ? gr->gr_name : "unknown", file_stat.st_gid);

    // Access rights in octal
    printf("Access: %o\n", file_stat.st_mode & 0777);

    // Time of last access
    printf("Last access: %s", ctime(&file_stat.st_atime));

    // Time of last inode change
    printf("Last inode change: %s", ctime(&file_stat.st_ctime));

    // Time of last modification
    printf("Last modification: %s", ctime(&file_stat.st_mtime));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [<filename> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
        printf("\n");
    }

    return 0;
}
