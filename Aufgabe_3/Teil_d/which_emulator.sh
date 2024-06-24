#!/bin/sh

if [ "$#" -ne 1 ]; then                             # Check if an argument was given
    echo "Usage: $0 <fie>"                          # Error message if no argument was given
    exit 1                                          # Exit with error code 1
fi                                                  # End of if statement

program=$1                                          # Program name is stored in the variable "program"

for dir in $PATH; do                                # Loop through all directories in the PATH
    if [ -x "$dir/$program" ]; then                 # Überprüfen, ob das Programm im Verzeichnis vorhanden ist
        echo "$dir/$program"                        # Ausgabe des Pfades zum Programm
        exit 0                                      # Beenden des Skripts mit Erfolg
    fi                                              # Beenden der Schleife
done                                                # Ende der Schleife

echo "$program: Kommando nicht gefunden"            # Ausgabe einer Fehlermeldung, wenn das Programm nicht gefunden wurde
exit 1                                              # Beenden des Skripts mit Fehlercode 1