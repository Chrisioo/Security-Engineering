#!/bin/bash

# Check, ob ein Dateiname übergeben wurde
if [ -z "$1" ]; then                                # -z: Check, ob die Länge des Strings 0 ist
    echo "Es wird ein Dateiname benötigt"
    echo "Beispiel: ./archivierung.sh dateiname"
    exit 1
fi

# Check, ob die Datei existiert
if [ ! -f $1 ]; then                                # -f: Check, ob die Datei existiert
    echo "Datei $1 wurde nicht gefunden"
    exit 1
fi

# Datei wird in verschiedene Formate archiviert
tar -cvf "$1.tar" "$1"
tar -czvf "$1.tar.gz" "$1"
tar -cjvf "$1.tar.bz2" "$1"
zip "$1.zip" "$1"

# Inhalt der Archive wird ausgegeben
# Ausgabe von tar 
echo "Inhalt von $1.tar:"
tar -tvf "$1.tar"

# Ausgabe von tar.gz
echo "Inhalt von $1.tar.gz:"
tar -tzvf "$1.tar.gz"

# Ausgabe von tar.bz2
echo "Inhalt von $1.tar.bz2:"
tar -tjvf "$1.tar.bz2"

# Ausgabe von zip
echo "Inhalt von $1.zip:"
unzip -l "$1.zip"