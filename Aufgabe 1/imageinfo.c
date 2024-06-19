#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define CAMERA_MODEL_OFFSET 0xb6
#define DATA_TIME_OFFSET 0xe4
#define BUFFER_SIZE 1024

/**
 * Prints the camera model and date and time information of an image file.
 *
 * @param filename The path of the image file.
 *
 * @return void
 *
 * @throws None
 */
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

/**
 * Reads metadata from a file at a given offset and stores it in a buffer.
 *
 * @param fd the file descriptor of the file to read from
 * @param offset the offset in the file to start reading from
 * @param buffer the buffer to store the metadata in
 * @param size the size of the buffer
 *
 * @return void
 *
 * @throws None
 */
void read_metdata (int fd, off_t offset, char *buffer, size_t size) {
    lseek(fd, offset, SEEK_SET);
    read(fd, buffer, size);
    buffer[size - 1] = '\0';
}


/**
 * Main function that takes an image file as input and prints the camera model and date and time information of the image.
 * Only takes one image as input at a time.
 * @param argc the number of command line arguments
 * @param argv an array of strings containing the command line arguments
 *
 * @return EXIT_SUCCESS if the program executed successfully, EXIT_FAILURE otherwise
 *
 * @throws None
 */
int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    print_image_info(argv[1]);
    exit(EXIT_SUCCESS);
}