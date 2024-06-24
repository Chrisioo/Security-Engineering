#include <stdio.h>                                                                  // Standard I/O library
#include <stdlib.h>                                                                 // Standard library 
#include <fcntl.h>                                                                  // File control library
#include <unistd.h>                                                                 

#define CAMERA_COMPANY_OFFSET 0x9e                                                  // Offset of the camera company in flash.JPG
#define CAMERA_MODEL_OFFSET 0xb6                                                    // Offset of the camera model in flash.JPG
#define DATA_TIME_OFFSET 0xe4                                                       // Offset of the date and time in flash.JPG    
#define STRING_SIZE 1024                                                            // Size of the string used for reading metadata

void print_image_info (const char *filename);                                       // Prints the camera model and date and time information of an image file
void read_metadata (int filedata, off_t offset, char *string, size_t size);         // Reads metadata from a file at a given offset and stores it in a string

/**
 * Prints the camera model and date and time information of an image file.
 * @param filename The path of the image file.
 * @return void
 */
void print_image_info (const char *filename) {                                      // Prints the camera model and date and time information of an image file
    int filedata = open(filename, O_RDONLY);                                        // Open the image file in read-only mode
    if (filedata == -1) {                                                           // Check if the file was opened successfully
        perror(filename);                                                           // Print an error message if the file was not opened successfully
        exit(EXIT_FAILURE);                                                         // Exit the program with an error code
    }
    char camera_model[STRING_SIZE];                                                 // String to save the camera model, initialized with size of STRING_SIZE
    char date_time[STRING_SIZE];                                                    // String to save the date and time, initialized with size of STRING_SIZE    
    char camera_company[STRING_SIZE];                                               // String to save the camera company, initialized with size of STRING_SIZE
    read_metadata(filedata, CAMERA_COMPANY_OFFSET, camera_company, STRING_SIZE);    // Read the camera model and date and time from the image file    
    read_metadata(filedata, CAMERA_MODEL_OFFSET, camera_model, STRING_SIZE);        // Read the camera model and date and time from the image file
    read_metadata(filedata, DATA_TIME_OFFSET, date_time, STRING_SIZE);              // Read the camera model and date and time from the image file


    printf("Camera model: %s %s\n", camera_company, camera_model);                  // Print the camera company and model 
    printf("Date and time: %s\n", date_time);                                       // Print the date and time
    close(filedata);                                                                // Close the image file                     
}

/**
 * Reads metadata from a file at a given offset and stores it in a string.
 * @param filedata the file descriptor of the file to read from
 * @param offset the offset in the file to start reading from
 * @param string the string to store the metadata in
 * @param size the size of the string
 * @return void
 */
void read_metadata (int filedata, off_t offset, char *string, size_t size) {        // Reads metadata from a file at a given offset and stores it in a string
    lseek(filedata, offset, SEEK_SET);                                              // Move the file pointer to the specified offset
    read(filedata, string, size);                                                   // Read the metadata from the file
    string[size - 1] = '\0';                                                        // Add a null terminator to the end of the string
}


/**
 * Main function that takes an image file as input and prints the camera model and date and time information of the image.
 * Only takes one image as input at a time.
 * @param argc the number of command line arguments
 * @param argv an array of strings containing the command line arguments
 * @return EXIT_SUCCESS if the program executed successfully, EXIT_FAILURE otherwise
 */
int main (int argc, char *argv[]) {                                                 // Main function, takes an image file as input
    if (argc != 2) {                                                                // Check if the number of command line arguments is correct
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);                       // Print an error message if the number of command line arguments is not correct
        exit(EXIT_FAILURE);                                                         // Exit the program with an error code
    }
    print_image_info(argv[1]);                                                      // Call the print_image_info function
    exit(EXIT_SUCCESS);                                                             // Exit the program with a success code after the function call
}