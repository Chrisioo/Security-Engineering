#!/bin/sh

for i in $*             # $* gibt alle Argumente aus
do                      # Schleife wird gestartet, durchläuft alle Argumente
    echo "Hallo $i"     # $i gibt das aktuelle Argument aus, z.B. "Hallo Hans"
done                    # Schleife wird beendet
exit 0                  # Programm wird mit 0 beendet, d.h. keine Fehler