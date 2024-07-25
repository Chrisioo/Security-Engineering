#!/bin/bash

# Definition des TAN-Verzeichnisses
TAN_DIR="TAN"

# Trap zum Ignorieren von STRG+C
trap "" SIGINT

while true; do                                                          # Endlosschleife               
    echo "Bitte geben Sie Ihren Benutzernamen ein:"
    read USERNAME                                                       # Einlesen des Benutzernamens             

    TAN_FILE="$TAN_DIR/$USERNAME.tan"                                   # Pfad zur TAN-Datei des Benutzers
    CURRENT_HASH_FILE="$TAN_DIR/$USERNAME.hash"                         # Pfad zur Datei, die den aktuellen Hashwert speichert

    if [ ! -f "$TAN_FILE" ] || [ ! -f "$CURRENT_HASH_FILE" ]; then      # Prüfen, ob die TAN-Datei oder die Datei mit dem aktuellen Hashwert existieren
        echo "Zugriff verweigert: Benutzer nicht gefunden."             # Ausgabe, wenn Benutzer nicht gefunden wurde
        continue                                                        # Schleife von vorne starten            
    fi

    if [ ! -s "$TAN_FILE" ]; then                                       # Prüfen, ob die TAN-Datei leer ist
        echo "Zugriff verweigert: TAN-Liste aufgebraucht."              # Ausgabe, wenn die TAN-Liste aufgebraucht ist
        rm "$TAN_FILE"                                                  # Löschen der TAN-Datei   
        rm "$CURRENT_HASH_FILE"                                         # Löschen der Datei mit dem aktuellen Hashwert
        continue                                                        # Schleife von vorne starten
    fi

    echo "Bitte geben Sie Ihre TAN ein:"
    read USER_TAN                                                       # Einlesen der Benutzer-TAN

    HASHED_TAN=$(echo -n "$USER_TAN" | sha256sum | awk '{print $1}')  # Berechnen des Hashwerts der eingegebenen TAN

    if grep -q "$HASHED_TAN" "$CURRENT_HASH_FILE"; then                   # Mit grep prüfen, ob die TAN in der Hash-Datei vorhanden ist
                                                                        # -q: Quiet-Modus, gibt keinen Output aus
        echo "Zugriff erlaubt."                                         # Ausgabe, wenn die TAN in der Hash-Datei vorhanden ist
        sed -i "/$USER_TAN/d" "$TAN_FILE"                               # Löschen der verwendeten TAN aus der TAN-Datei

        echo "$USER_TAN" > "$CURRENT_HASH_FILE"                         # Schreiben der verwendeten TAN in die Datei mit dem aktuellen Hashwert
        #echo "$(head -n 1 "$TAN_FILE")" > "$CURRENT_HASH_FILE"          # Schreiben der nächsten TAN in die Datei mit dem aktuellen Hashwert
    else
        echo "Zugriff verweigert: Ungültige TAN."                       # Ausgabe, wenn die TAN ungültig ist
    fi
done