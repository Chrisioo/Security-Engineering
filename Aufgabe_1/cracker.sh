#!/bin/bash

# Pfad zur Wörterbuch-Datei
WORDS_FILE="words.txt"

# Benutzer und deren Hashes
users=(
    "Steffi.Jones $1$O7v0C21Z$2FH7ib2Dxtoq6B83qTgON1"
    "Marco.Reus $1$Jebn3vQ5$2k..iqxtXNwfsCFAamWCS0"
    "Almuth.Schult $1$0ngrMRa1$uXLzWhnrYzmiRM3fi8Nde1"
    "Manuel.Neuer $1$1aaPttrp$VoF2rkOyC/tE.DxzQuuIY1"
    "Birgit.Prinz $1$7ieEwjFr$T/jwatbzqhLZNVDEfymB41"
)

# Funktion zum Extrahieren des Salts aus einem Hash
extract_salt() {
    echo "$1" | cut -d'$' -f3
}

# Durchlaufe alle Benutzer
for entry in "${users[@]}"; do
    user=$(echo "$entry" | cut -d' ' -f1)
    target_hash=$(echo "$entry" | cut -d' ' -f2)
    salt=$(extract_salt "$target_hash")
    echo "Überprüfe Passwort für $user mit Salt $salt..."

    # Durchlaufe alle Wörter im Wörterbuch
    while IFS= read -r word; do
        # Erzeuge den Hash des aktuellen Worts
        generated_hash=$(openssl passwd -1 -salt "$salt" "$word")

        # Vergleiche den generierten Hash mit dem Ziel-Hash
        if [ "$generated_hash" == "$target_hash" ]; then
            echo "Passwort für $user gefunden: $word"
            break
        fi
    done < "$WORDS_FILE"
done
