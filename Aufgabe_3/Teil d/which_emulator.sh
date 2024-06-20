#!/bin/sh

# Überprüfen, ob ein Argument übergeben wurde
if [ "$#" -ne 1 ]; then
    echo "Nutzung: $0 programname"
    exit 1
fi

# Gesuchtes Programm
program=$1

# Durchlaufen der in PATH definierten Verzeichnisse
OLDIFS=$IFS
IFS=':'
for dir in $PATH; do
    if [ -x "$dir/$program" ]; then
        echo "$dir/$program"
        IFS=$OLDIFS
        exit 0
    fi
done
IFS=$OLDIFS

# Wenn das Programm nicht gefunden wurde
echo "$program: Kommando nicht gefunden"
exit 1