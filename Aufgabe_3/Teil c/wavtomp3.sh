#!/bin/sh

if ["$#" -lt 1]; then
    echo "Usage: $0 <file>"
    exit 1
fi

for wavfile in "$@"; do
    if [ ! -f "$wavfile" ]; then
        echo "Error: $wavfile is not a file"
        continue
    fi
    
    base=$(basename "$wavfile" .wav)
    ffmpeg -i "$wavfile" "$base.mp3"

    if [ $? -ne 0 ]; then
        echo "Error converting $wavfile"
    else 
        echo "Converted $wavfile to $base.mp3"
    fi
done
exit 0