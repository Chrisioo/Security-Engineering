#!/bin/bash

#################################################
###Demoversion mit 3 Nutzern und 3 Passwörtern###
#################################################

# User mit gehashten Passwörtern
# Assoziatives Array, das die Nutzernamen als Schlüssel und die gehashten Passwörter als Werte speichert -> Key-Value-Paar
declare -A USERS
USERS["Steffi.Jones"]='$1$O7v0C21Z$2FH7ib2Dxtoq6B83qTgON1'                      # Ist in words_demo.txt vorhanden
USERS["Marco.Reus"]='$1$Jebn3vQ5$2k..iqxtXNwfsCFAamWCS0'                        # Ist in words_demo.txt vorhanden
USERS["Birgit.Prinz"]='$1$7ieEwjFr$T/jwatbzqhLZNVDEfymB41'                      # Ist nicht in words_demo.txt vorhanden (flyer)

# Array zum Speichern der gefundenen Passwörter
declare -A GEFUNDENE_PASSWOERTER

while read word; do                                                             # Schleife über alle Wörter in words_demo.txt                                                                       
    for username in "${!USERS[@]}"; do                                          # Schleife über alle Nutzer im Array USERS
        hash="${USERS[$username]}"                                              # Extrahiert Value (gehashtes Passwort) aus dem Array USERS
        salt=$(echo $hash | cut -d'$' -f3)                                      # Extrahiert Salt aus dem gehashten Passwort -> 3. Feld getrennt durch '$'

        generated_hash=$(openssl passwd -1 -salt "$salt" "$word")               # Generiert gehashtes Passwort mit dem Salt und dem aktuellen Wort
        echo "Hash: $generated_hash, Wort: $word, salt: $salt ($username)"      # Ausgabe des generierten Hashes, des aktuellen Wortes und des Salts
        
        if [[ "$generated_hash" == *"$hash"* ]]; then                           # Vergleich des generierten Hashes mit dem gehashten Passwort
            echo "Passwort für Nutzer $username: $word"                         # Wenn Passwort gefunden, Ausgabe des Nutzernamens und gefundenen Passworts
            GEFUNDENE_PASSWOERTER["$username"]="$word"                          # Speichern des gefundenen Passworts im Array GEFUNDENE_PASSWOERTER
            unset USERS["$username"]                                            # Entfernen des Nutzers aus dem Array USERS
        fi

        if [ ${#USERS[@]} -eq 0 ]; then                                         # Prüfen, ob alle Passwörter gefunden wurden
            break 2                                                             # Wenn ja, innere for-Schleife beenden
        fi
    done                                                                        # Ende der inneren for-Schleife
done < "words_demo.txt"                                                         # Ende der äußeren while-Schleife, Wörter werden aus words_demo.txt gelesen

# Ausgabe aller Nutzer mit ihren gefundenen Passwörtern
for username in "${!GEFUNDENE_PASSWOERTER[@]}"; do                              # Schleife über alle Nutzer im Array GEFUNDENE_PASSWOERTER 
    echo "Password für Nutzer $username: ${GEFUNDENE_PASSWOERTER[$username]}"   # Ausgabe des Nutzernamens und des gefundenen Passworts
done                                                                    

# Ausgabe der Nutzer, für die keine Passwörter gefunden wurden
for username in "${!USERS[@]}"; do                                              # Schleife über alle Nutzer im Array USERS
    echo "Passwort für Nutzer $username wurde nicht gefunden."                  # Ausgabe der Nutzer, für die keine Passwörter gefunden wurden
done

exit 0                                                                          # Beenden des Skripts mit Exit-Code 0     