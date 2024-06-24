#!/bin/sh

if [ -z "$1" ]; then                                            # Check if a file was given
    echo "Usage: $0 <file>"                                     # error message if no file was given
    exit 1                                                      # exit with error code 1
fi

file_type=$(file "$1")                                          # Determine the type of the file using the 'file' command

if echo "$file_type" | grep -q "PDF document"; then             # Check if the file is a PDF
    /usr/bin/evince "$1"                                        # Open PDF file with evince
elif echo "$file_type" | grep -q "image"; then                  # Check if the file is an image
    /usr/bin/eog "$1"                                           # Open image with eog
elif echo "$file_type" | grep -q "text"; then                   # Check if the file is a text file
    /usr/bin/less "$1"                                          # Open text file with less                                 
elif echo "$file_type" | grep -q "OpenDocument"; then           # Check if the file is an OpenDocument
    /usr/bin/libreoffice "$1"                                   # Open OpenDocument with libreoffice                       
else
    echo "Unknown file type: $file_type"                        # Print an error message if the file type is not supported
    exit 1                                                      # Exit with error code 1
fi
exit 0                                                          # Exit with success code 0