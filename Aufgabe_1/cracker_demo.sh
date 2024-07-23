#!/bin/bash

#################################################
###Demoversion mit 3 Nutzern und 3 Passwörtern###
#################################################

# User mit gehashten Passwörtern
declare -A USERS
USERS["Steffi.Jones"]='$1$O7v0C21Z$2FH7ib2Dxtoq6B83qTgON1'
USERS["Marco.Reus"]='$1$Jebn3vQ5$2k..iqxtXNwfsCFAamWCS0'
USERS["Birgit.Prinz"]='$1$7ieEwjFr$T/jwatbzqhLZNVDEfymB41'

# Array zum Speichern der gefundenen Passwörter
declare -A GEFUNDENE_PASSWOERTER

while read -r word; do                                                      # Lese jedes Wort aus der Datei words.txt                       
    for username in "${!USERS[@]}"; do                                      # Iteriere über alle User              
        hash="${USERS[$username]}"                                          # Zuweisen des Hashes des aktuellen Users
        salt=$(echo $hash | cut -d'$' -f3)                                  # Extrahieren des Salts aus dem Hash                  
        target_hash=$(echo $hash | cut -d'$' -f4)                           # Extrahieren des Zielhashes aus dem Hash      

        generated_hash=$(openssl passwd -1 -salt "$salt" "$word")           # Generiere Hash für das aktuelle Wort
        echo "Hash: $generated_hash, Wort: $word, salt: $salt ($username)"  # Ausgabe des generierten Hashes, des Wortes und des Salts
        
        if [[ "$generated_hash" == *"$target_hash"* ]]; then                # Vergleiche den generierten Hash mit dem Zielhash
            echo "Passwort für Nutzer $username: $word"                     # Falls Passwort gefunden, Ausgabe des Nutzernamens und Passworts
            GEFUNDENE_PASSWOERTER["$username"]="$word"                      # Speichern des gefundenen Passworts
            unset USERS["$username"]                                        # Löschen des Nutzers aus dem Array, da Passwort gefunden
        fi

        if [ ${#USERS[@]} -eq 0 ]; then                                     # Prüfe, ob alle Passwörter gefunden wurden
            break 2                                                         # Beende die Schleife, falls alle Passwörter gefunden wurden
        fi
    done
done < "words_demo.txt"

# Ausgabe aller Nutzer mit ihren gefundenen Passwörtern
for username in "${!GEFUNDENE_PASSWOERTER[@]}"; do
    echo "Password für Nutzer $username: ${GEFUNDENE_PASSWOERTER[$username]}"
done

# Ausgabe der Nutzer, für die keine Passwörter gefunden wurden
for username in "${!USERS[@]}"; do
    echo "Passwort für Nutzer $username wurde nicht gefunden."
done

exit 0