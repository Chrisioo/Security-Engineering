#!/bin/sh

if ["$#" -lt 1]; then                               # Check if a file was given
    echo "Usage: $0 <file>"                         # error message if no file was given
    exit 1                                          # exit with error code 1
fi                                                  # End of if statement

for wavfile in "$@"; do                             # Loop through all arguments
    if [ ! -f "$wavfile" ]; then                    # Check if the file is a wav file
        echo "Error: $wavfile is not a file"        # error message if the file is not a file
        continue                                    # skip the rest of the loop
    fi                                              # End of if statement

    base=$(basename "$wavfile" .wav)                # Get the base name of the file
    ffmpeg -i "$wavfile" "$base.mp3"                # Convert the wav file to mp3 using ffmpeg

    if [ $? -ne 0 ]; then                           # Check if the conversion was successful
        echo "Error converting $wavfile"            # error message if the conversion was not successful
    else 
        echo "Converted $wavfile to $base.mp3"      # success message if the conversion was successful
    fi                                              # End of if statement
done                                                # End of for loop
exit 0                                              # exit with success code 0