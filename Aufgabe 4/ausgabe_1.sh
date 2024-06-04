#!/bin/bash

echo "Ausgabe des Kommandos 'od -t x4 /dev/urandom | head -1 | cut -c 17- | sed -e "s/ //g"'"
echo "Hierbei wird der Inhalt von /dev/urandom in hexadezimaler Darstellung ausgegeben."
echo "Durch die Option -t x4 wird die Ausgabe in 4 Byte große Blöcke unterteilt."
echo "Mit head -1 wird nur die erste Zeile ausgegeben."
echo "Mit cut -c 17- wird der Inhalt ab dem 17. Zeichen ausgegeben."
key=$(od -t x4 /dev/urandom | head -1 | cut -c 17- | sed -e "s/ //g")
echo $key

echo -e "\nAusgabe des Kommandos 'od -t x4 /dev/urandom | head -1 | cut -c 17-48 | sed -e "s/ //g"'"
echo "Mit cut -c 17-48 wird der Inhalt von Zeichen 17 bis 48 ausgegeben."
od -t x4 /dev/urandom | head -1 | cut -c 17-48 | sed -e "s/ //g"

echo "Ausgabe des Kommandos 'openssl dgst -sha256 -mac HMAC -macopt hexkey:$key /etc/services'"
echo "Hierbei wird der SHA256-Hashwert des Inhalts von /etc/services berechnet."
echo "Der Hashwert wird mit dem HMAC-Verfahren und dem Schlüssel $key berechnet."
openssl dgst -sha256 -mac HMAC -macopt hexkey:$key /etc/services
