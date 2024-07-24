#!/bin/bash

echo "Nutzername: "
read USERNAME
echo "Anzahl TANs: "
read NUM_TANS

TAN_DIR="TAN"           # Verzeichnis für TAN-Dateien

# Erstellen des Verzeichnisses für die TAN-Dateien, falls es nicht existiert
# -p: Erstellt auch übergeordnete Verzeichnisse, falls notwendig
mkdir -p "$TAN_DIR"

# Generieren eines Zufallsseeds
# od -vAn -N4 -tu4 < /dev/urandom: Liest 4 Bytes aus /dev/urandom und gibt sie als vorzeichenlose Ganzzahl aus
# tr -d ' ': Entfernt Leerzeichen
RANDOM_SEED=$(od -vAn -N4 -tu4 < /dev/urandom | tr -d ' ')

# Erzeugen des ersten Hashwerts
# sha256sum: Berechnet den SHA-256-Hashwert
# awk '{print $1}': Gibt nur den Hashwert aus, da sonst noch Leerzeichen und der Dateiname ausgegeben werden
CURRENT_HASH=$(echo "$RANDOM_SEED" | sha256sum | awk '{print $1}')

# Anlegen einer Liste für die TANs
TAN_LIST=()
for (( i=0; i<$NUM_TANS; i++ )); do
    NEXT_HASH=$(echo -n "$CURRENT_HASH" | sha256sum | awk '{print $1}')     # Berechnen des nächsten Hashwerts indem der aktuelle Hashwert gehasht wird
    TAN_LIST+=("$CURRENT_HASH")                                             # Hinzufügen des aktuellen Hashwerts zur TAN-Liste
    CURRENT_HASH=$NEXT_HASH                                                 # Aktualisieren des aktuellen Hashwerts mit dem gerade berechneten Hashwert
done

# Umkehren der TAN-Liste
# tac -s ' ': Gibt die Eingabezeilen in umgekehrter Reihenfolge aus
REVERSED_TAN_LIST=($(echo "${TAN_LIST[@]}" | tac -s ' '))

# Speichern der TAN-Liste in der Datei
TAN_FILE="$TAN_DIR/$USERNAME.tan"
for TAN in "${REVERSED_TAN_LIST[@]}"; do
    echo "$TAN" >> "$TAN_FILE"
done

# Speichern des letzten Hashwerts (wird als erster aktueller Hashwert verwendet)
CURRENT_HASH_FILE="$TAN_DIR/$USERNAME.hash"
echo "${TAN_LIST[$NUM_TANS-1]}" > "$CURRENT_HASH_FILE"

# Ausgabe einer Bestätigungsmeldung
echo "TAN-Liste für Benutzer '$USERNAME' mit $NUM_TANS TANs generiert."