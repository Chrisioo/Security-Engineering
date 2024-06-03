#!/bin/bash

# Check, ob zwei Dateinamen als Argumente übergeben wurden
if [ $# -ne 2 ]; then
    echo "Es müssen zwei Dateinamen als Argumente übergeben werden"
    echo "Beispiel: ./archivierung.sh file1 file2"
    exit 1
fi

# Check if the first file exists
if [ ! -f "$1" ]; then
    echo "Datei mit dem Namen $1 nicht gefunden"
    exit 1
fi

# Check if the second file exists
if [ ! -f "$2" ]; then
    echo "Datei mit dem Namen $2 nicht gefunden"
    exit 1
fi


# Dateien werden in verschiedene Formate archiviert
# Kommando muss jeweils kompletten Namen des Archivs und den Dateinamen selbst enthalten, also z.B. tar -cvf datei.tar datei
tar -cvf "archiv.tar" "$1" "$2"
tar -czvf "archiv.tar.gz" "$1" "$2"
zip "archiv.zip" "$1" "$2"
tar -cjvf "archiv.tar.bz2" "$1" "$2"

# Inhalt der Archive wird ausgegeben
# Ausgabe von tar 
echo "Inhalt von archiv.tar:"
tar -tvf "archiv.tar"

# Ausgabe von tar.gz
echo "Inhalt von archiv.tar.gz:"
tar -tzvf "archiv.tar.gz"

# Ausgabe von tar.bz2
echo "Inhalt von archiv.tar.bz2:"
tar -tjvf "archiv.tar.bz2"

# Ausgabe von zip
echo "Inhalt von archiv.zip:"
unzip -l "archiv.zip"