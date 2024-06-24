#!/bin/sh

if [ "$#" -ne 1 ]; then                             # Überprüfen, ob ein Argument übergeben wurde
    echo "Nutzung: $0 programname"                  # Fehlermeldung, wenn kein Argument übergeben wurde
    exit 1                                          # Beenden des Skripts mit Fehlercode 1
fi

program=$1                                          # Speichern des Programms in der Variablen "program"

OLDIFS=$IFS                                         # Speichern des alten IFS
IFS=':'                                             # Setzen des IFS auf ":"                 
for dir in $PATH; do                                # Schleife über alle Verzeichnisse im PATH
    if [ -x "$dir/$program" ]; then                 # Überprüfen, ob das Programm im Verzeichnis vorhanden ist
        echo "$dir/$program"                        # Ausgabe des Pfades zum Programm
        IFS=$OLDIFS                                 # Wiederherstellen des alten IFS
        exit 0                                      # Beenden des Skripts mit Erfolg
    fi                                              # Beenden der Schleife
done                                                # Ende der Schleife
IFS=$OLDIFS                                         # Wiederherstellen des alten IFS

echo "$program: Kommando nicht gefunden"            # Ausgabe einer Fehlermeldung, wenn das Programm nicht gefunden wurde
exit 1                                              # Beenden des Skripts mit Fehlercode 1