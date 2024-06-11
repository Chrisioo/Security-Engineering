#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define CAMERA_MODEL_OFFSET 0xb6
#define DATA_TIME_OFFSET 0xe4
#define BUFFER_SIZE 1024

void print_image_info (const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    char camera_model[BUFFER_SIZE];
    char date_time[BUFFER_SIZE];
    read_metdata(fd, CAMERA_MODEL_OFFSET, camera_model, BUFFER_SIZE);
    read_metdata(fd, DATA_TIME_OFFSET, date_time, BUFFER_SIZE);

    printf("Camera model: %s\n", camera_model);
    printf("Date and time: %s\n", date_time);
    close(fd);
}

void read_metdata (int fd, off_t offset, char *buffer, size_t size) {
    lseek(fd, offset, SEEK_SET);
    read(fd, buffer, size);
    buffer[size - 1] = '\0';
}


int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    print_image_info(argv[1]);
    exit(EXIT_SUCCESS);
}