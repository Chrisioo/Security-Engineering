#!/bin/bash

TAN_DIR="TAN"

trap "" SIGINT SIGTERM  # Ignoriere STRG+C und SIGTERM

while true; do
    echo "Bitte geben Sie Ihren Benutzernamen ein:"
    read USERNAME

    TAN_FILE="$TAN_DIR/$USERNAME.tan"
    CURRENT_HASH_FILE="$TAN_DIR/$USERNAME.hash"

    if [ ! -f "$TAN_FILE" ] || [ ! -f "$CURRENT_HASH_FILE" ]; then
        echo "Zugriff verweigert: Benutzer nicht gefunden."
        continue
    fi

    echo "Bitte geben Sie Ihre TAN ein:"
    read USER_TAN

    if [ ! -s "$TAN_FILE" ]; then
        echo "Zugriff verweigert: TAN-Liste aufgebraucht."
        rm "$TAN_FILE"
        rm "$CURRENT_HASH_FILE"
    fi

    if grep -q "$USER_TAN" "$TAN_FILE"; then
        # TAN gefunden und korrekt
        echo "Zugriff erlaubt."

        # Entfernen der verbrauchten TAN aus der TAN-Datei
        sed -i "/$USER_TAN/d" "$TAN_FILE"

        # Speichern des neuen aktuellen Hashwerts
        echo "$USER_TAN" > "$CURRENT_HASH_FILE"
    else
        echo "Zugriff verweigert: Falsche TAN oder TAN-Liste aufgebraucht."
    fi

    # Überprüfen, ob die TAN-Liste leer ist
    
done