#!/bin/bash

# Überprüfen, ob alle benötigten Parameter angegeben wurden
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <username> <number_of_tans>"
    exit 1
fi

USERNAME=$1
NUM_TANS=$2
TAN_DIR="TAN"

# Erstellen des Verzeichnisses für die TAN-Dateien, falls es nicht existiert
mkdir -p "$TAN_DIR"

# Erzeugen einer zufälligen Startzahl
RANDOM_SEED=$(od -vAn -N4 -tu4 < /dev/urandom | tr -d ' ')

# Initialisieren des ersten Hashwerts
CURRENT_HASH=$(echo -n "$RANDOM_SEED" | sha256sum | awk '{print $1}')

# Erzeugen der TAN-Liste
TAN_LIST=()
for (( i=0; i<$NUM_TANS; i++ )); do
    NEXT_HASH=$(echo -n "$CURRENT_HASH" | sha256sum | awk '{print $1}')
    TAN_LIST+=("$CURRENT_HASH")
    CURRENT_HASH=$NEXT_HASH
done

# Umkehren der TAN-Liste
REVERSED_TAN_LIST=($(echo "${TAN_LIST[@]}" | tac -s ' '))

# Speichern der TAN-Liste in der Datei
TAN_FILE="$TAN_DIR/$USERNAME.tan"
echo "${REVERSED_TAN_LIST[@]}" > "$TAN_FILE"

# Speichern des letzten Hashwerts (wird als erster aktueller Hashwert verwendet)
CURRENT_HASH_FILE="$TAN_DIR/$USERNAME.hash"
echo "${TAN_LIST[$NUM_TANS-1]}" > "$CURRENT_HASH_FILE"

echo "TAN-Liste für Benutzer '$USERNAME' mit $NUM_TANS TANs generiert."