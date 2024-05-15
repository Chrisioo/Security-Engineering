#include<stdio.h>                                                                   // Standardbibliothek fuer Ein- und Ausgabe
#include<time.h>                                                                    // Bibliothek fuer Zeitfunktionen

int main () {                                                                       // Beginn der Funktion main
    time_t t;                                                                       // Datentyp fuer Zeit, dargestellt mit Variable t
    struct tm *localTime;                                                           // Struktur fuer lokale Zeit, Pointer localTime zeigt auf Struktur
    char timeString[100];                                                           // String fuer Zeit, 100 Zeichen langes Char-Array

    // Systemzeit abrufen
    t = time(NULL);                                                                 // Aktuelle Systemzeit, t wird mit aktueller Zeit initialisiert

    // Systemzeit in lokale Zeit umwandeln
    localTime = localtime(&t);                                                      // In Variable t gespeicherte Zeit wird in lokale Zeit umgewandelt und in localTime gespeichert

    // Zeit in String umwandeln
    strftime(timeString, sizeof(timeString), "%a %b %d %H:%M:%S %Y", localTime);    // strftime() wandelt lokale Zeit in String mit bestimmtem Format um
                                                                                    // %a - Abkuerzung des Wochentags
                                                                                    // %b - Abkuerzung des Monats
                                                                                    // %d - Tag des Monats
                                                                                    // %H - Stunde (24-Stunden-Format)
                                                                                    // %M - Minute
                                                                                    // %S - Sekunde
                                                                                    // %Y - Vierstellige Jahreszahl

    printf("Aktuelle Zeit mit strftime():\n");
    // Ausgabe des Zeit-Strings
    printf("%s\n", timeString);                                                     // Ausgabe des formatierten Strings            

    return 0;                                                                       // Programmende
}