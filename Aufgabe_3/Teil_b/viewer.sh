#!/bin/sh

if [ -z "$1" ]; then                                            # Check, ob eine Datei angegeben wurde
    echo "Usage: $0 <file>"                                     # Ausgabe, wenn keine Datei angegeben wurde
    exit 1                                                      # Exit mit Fehlercode 1
fi

file_type=$(file "$1")                                          # Dateiendung auslesen mit Kommando "file"

if echo "$file_type" | grep -q "PDF document"; then             # Check, ob die Datei eine PDF-Datei ist
    /usr/bin/evince "$1"                                        # Öffnen mit evince
elif echo "$file_type" | grep -q "image"; then                  # Check, ob die Datei ein Bild ist
    /usr/bin/eog "$1"                                           # Öffnen mit eog / Eye of Gnome
elif echo "$file_type" | grep -q "text"; then                   # Check, ob die Datei eine Textdatei ist
    /usr/bin/less "$1"                                          # Öffnen mit less                                      
elif echo "$file_type" | grep -q "OpenDocument"; then           # Check, ob die Datei eine OpenOffice-Datei ist
    /usr/bin/libreoffice "$1"                                   # Öffnen mit libreoffice                           
else
    echo "Unbekanntr Dateityp: $file_type"                      # Ausgabe, wenn die Dateiendung unbekannt ist
    exit 1                                                      # Exit mit Fehlercode 1
fi
exit 0                                                          # Bei Erfolg Exit mit 0