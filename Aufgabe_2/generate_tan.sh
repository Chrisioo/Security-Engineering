#!/bin/bash

# Überprüfen, ob alle benötigten Parameter angegeben wurden
if [ "$#" -ne 2 ]; then                                                     # Check, ob Anzahl Parameter ungleich 2 ist
    echo "Nutzung: $0 <Benutzername> <Anzahl TANs>"                         # Ausgabe der korrekten Nutzung des Skripts
    exit 1                                                                  # Beenden des Skripts mit einem Fehlercode
fi

USERNAME=$1             # Benutzername = 1. Parameter  
NUM_TANS=$2             # Anzahl TANs = 2. Parameter
TAN_DIR="TAN"           # Verzeichnis für TAN-Dateien

# Erstellen des Verzeichnisses für die TAN-Dateien, falls es nicht existiert
mkdir -p "$TAN_DIR"

# Generieren eines Zufallsseeds
RANDOM_SEED=$(od -vAn -N4 -tu4 < /dev/urandom | tr -d ' ')                  # Lesen von 4 zufälligen Bytes und Umwandlung in eine Ganzzahl
                                                                            # od -vAn -N4 -tu4 < /dev/urandom: Lesen von 4 zufälligen Bytes und Umwandlung in eine vorzeichenlose Ganzzahl 
                                                                            # tr -d ' ': Entfernen von Leerzeichen

# Erzeugen des ersten Hashwerts
CURRENT_HASH=$(echo -n "$RANDOM_SEED" | sha256sum | awk '{print $1}')       # Berechnen des SHA256-Hashwerts des Zufallsseeds

TAN_LIST=()                                                                 # Initialisieren einer leeren Liste für die TANs
for (( i=0; i<$NUM_TANS; i++ )); do                                         # Schleife für die Anzahl der TANs
    NEXT_HASH=$(echo -n "$CURRENT_HASH" | sha256sum | awk '{print $1}')     # Berechnen des nächsten SHA256-Hashwerts
    TAN_LIST+=("$CURRENT_HASH")                                             # Hinzufügen des aktuellen Hashwerts zur TAN-Liste
    CURRENT_HASH=$NEXT_HASH                                                 # Setzen des aktuellen Hashwerts auf den nächsten Hashwert
done

# Umkehren der TAN-Liste
REVERSED_TAN_LIST=($(echo "${TAN_LIST[@]}" | tac -s ' '))                   # Umkehren der Reihenfolge der TAN-Liste

# Speichern der TAN-Liste in der Datei
TAN_FILE="$TAN_DIR/$USERNAME.tan"                                           # Festlegen des Dateipfads für die TAN-Datei
for TAN in "${REVERSED_TAN_LIST[@]}"; do                                    # Schleife über jede TAN in der umgekehrten TAN-Liste
    echo "$TAN" >> "$TAN_FILE"                                              # Anhängen der TAN an die TAN-Datei
done

# Speichern des letzten Hashwerts (wird als erster aktueller Hashwert verwendet)
CURRENT_HASH_FILE="$TAN_DIR/$USERNAME.hash"                                 # Festlegen des Dateipfads für die Hash-Datei
echo "${TAN_LIST[$NUM_TANS-1]}" > "$CURRENT_HASH_FILE"                      # Speichern des letzten Hashwerts in die Hash-Datei

# Ausgabe einer Bestätigungsmeldung
echo "TAN-Liste für Benutzer '$USERNAME' mit $NUM_TANS TANs generiert."     # Ausgabe einer Erfolgsmeldung