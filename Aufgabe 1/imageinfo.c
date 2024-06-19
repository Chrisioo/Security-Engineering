#include <stdio.h>                                                                  // Standard I/O library
#include <stdlib.h>                                                                 // Standard library 
#include <fcntl.h>                                                                  // File control library
#include <unistd.h>                                                                 

#define CAMERA_MODEL_OFFSET 0xb6                                                    // Offset of the camera model in flash.JPG
#define DATA_TIME_OFFSET 0xe4                                                       // Offset of the date and time in flash.JPG    
#define BUFFER_SIZE 1024                                                            // Size of the buffer used for reading metadata

/**
 * Prints the camera model and date and time information of an image file.
 *
 * @param filename The path of the image file.
 *
 * @return void
 *
 * @throws None
 */
void print_image_info (const char *filename) {                                      // Prints the camera model and date and time information of an image file
    int filedata = open(filename, O_RDONLY);                                        // Open the image file in read-only mode
    if (filedata == -1) {                                                           // Check if the file was opened successfully
        perror(filename);                                                           // Print an error message if the file was not opened successfully
        exit(EXIT_FAILURE);                                                         // Exit the program with an error code
    }
    char camera_model[BUFFER_SIZE];                                                 // String to save the camera model, created with size of BUFFER_SIZE
    char date_time[BUFFER_SIZE];                                                    // String to save the date and time, created with size of BUFFER_SIZE        
    read_metdata(filedata, CAMERA_MODEL_OFFSET, camera_model, BUFFER_SIZE);         // Read the camera model and date and time from the image file
    read_metdata(filedata, DATA_TIME_OFFSET, date_time, BUFFER_SIZE);               // Read the camera model and date and time from the image file

    printf("Camera model: %s\n", camera_model);                                     // Print the camera model 
    printf("Date and time: %s\n", date_time);                                       // Print the date and time
    close(filedata);                                                                // Close the image file                     
}

/**
 * Reads metadata from a file at a given offset and stores it in a buffer.
 *
 * @param filedata the file descriptor of the file to read from
 * @param offset the offset in the file to start reading from
 * @param buffer the buffer to store the metadata in
 * @param size the size of the buffer
 *
 * @return void
 *
 * @throws None
 */
void read_metdata (int filedata, off_t offset, char *buffer, size_t size) {         // Reads metadata from a file at a given offset and stores it in a buffer
    lseek(filedata, offset, SEEK_SET);                                              // Move the file pointer to the specified offset
    read(filedata, buffer, size);                                                   // Read the metadata from the file
    buffer[size - 1] = '\0';                                                        // Add a null terminator to the end of the buffer
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
int main (int argc, char *argv[]) {                                                 // Main function, takes an image file as input
    if (argc != 2) {                                                                // Check if the number of command line arguments is correct
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);                       // Print an error message if the number of command line arguments is not correct
        exit(EXIT_FAILURE);                                                         // Exit the program with an error code
    }
    print_image_info(argv[1]);                                                      // Call the print_image_info function
    exit(EXIT_SUCCESS);                                                             // Exit the program with a success code after the function call
}