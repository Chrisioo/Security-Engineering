#!/bin/bash

declare -a password_array

# Durchlaufe alle Wörter im Wörterbuch
while IFS= read -r word; do
    # Erzeuge den Hash des aktuellen Worts
    generated_hash=$(openssl passwd -1 -salt "$salt" "$word")

    echo "Generierter Hash: $generated_hash für Wort: $word"
    # Vergleiche den generierten Hash mit dem Ziel-Hash
    if [ "$generated_hash" == "$1$O7v0C21Z$2FH7ib2Dxtoq6B83qTgON1" ]; then
        password_array+=("Steffi.Jones: $word")
        continue
    elif [ "$generated_hash" == "$1$Jebn3vQ5$2k..iqxtXNwfsCFAamWCS0" ]; then
        password_array+=("Marco.Reus: $word")
        continue
    elif [ "$generated_hash" == "$1$0ngrMRa1$uXLzWhnrYzmiRM3fi8Nde1" ]; then
        password_array+=("Almuth.Schult: $word")
        continue
    elif [ "$generated_hash" == "$1$1aaPttrp$VoF2rkOyC/tE.DxzQuuIY1" ]; then
        password_array+=("Manuel.Neuer: $word")
        continue
    elif [ "$generated_hash" == "$1$7ieEwjFr$T/jwatbzqhLZNVDEfymB41" ]; then
        password_array+=("Birgitt.Prinz: $word")
        continue
    fi
    
done < "words.txt"
echo "Passwörter: ${password_array[@]}"