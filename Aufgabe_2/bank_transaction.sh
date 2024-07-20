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

    # Lesen des aktuellen Hashwerts
    CURRENT_HASH=$(cat "$CURRENT_HASH_FILE")

    # Berechnen des Hashwerts der eingegebenen TAN
    USER_TAN_HASH=$(echo -n "$USER_TAN" | sha256sum | awk '{print $1}')

    if [ "$USER_TAN_HASH" == "$CURRENT_HASH" ]; then
        echo "Zugriff erlaubt."

        # Entfernen der verbrauchten TAN
        sed -i '1d' "$TAN_FILE"

        # Speichern des neuen aktuellen Hashwerts
        if [ -s "$TAN_FILE" ]; then
            NEW_HASH=$(head -n 1 "$TAN_FILE")
            echo "$NEW_HASH" > "$CURRENT_HASH_FILE"
        else
            echo "Zugriff verweigert: TAN-Liste aufgebraucht."
            rm "$TAN_FILE"
            rm "$CURRENT_HASH_FILE"
        fi
    else
        echo "Zugriff verweigert: Falsche TAN."
    fi
done
