#include<stdio.h>
#include<time.h>

int main () {
    time_t t;     // Zeit-Struktur
    struct tm *localTime;   // Lokale Zeit-Struktur
    char timeString[100];   // Zeit-String in Form eines Char-Arrays

    // Systemzeit abrufen
    t = time(NULL);   // Zeit in Sekunden seit 01.01.1970

    // Systemzeit in lokale Zeit umwandeln
    localTime = localtime(&t);    // Zeit in lokale Zeit umwandeln

    // Zeit in String umwandeln
    strftime(timeString, sizeof(timeString), "%a %b %d %H:%M:%S %Y", localTime);    // Zeit im Format "Wochentag Monat Tag Stunde:Minute:Sekunde Jahr"

    printf("Aktuelle Zeit mit strftime():\n");
    // Ausgabe des Zeit-Strings
    printf("%s\n", timeString); 

    return 0;   // Programmende
}