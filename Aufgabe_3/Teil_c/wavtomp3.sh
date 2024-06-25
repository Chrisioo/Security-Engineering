#!/bin/sh

if ["$#" -lt 1]; then                               # Check if a file was given
    echo "Usage: $0 <file>"                         # error message if no file was given
    exit 1                                          # exit with error code 1
fi                                                  # End of if statement

for wavfile in "$@"; do                             # Loop through all arguments
    mp3file=$(basename "$wavfile" .wav)             # Get the base name of the file
    ffmpeg -i "$wavfile" "$mp3file.mp3"             # Convert the wav file to mp3 using ffmpeg

    if [ $? -ne 0 ]; then                           # Check if the conversion was successful by checking if the exit code of the previous command is not 0
        echo "Error converting $wavfile"            # error message if the conversion was not successful
    else 
        echo "Converted $wavfile to $mp3file.mp3"   # success message if the conversion was successful
    fi                                              # End of if statement
done                                                # End of for loop
exit 0                                              # exit with success code 0