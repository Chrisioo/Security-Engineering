#include<stdio.h>                                                                   // Standardbibliothek fuer Ein- und Ausgabe
#include<time.h>                                                                    // Bibliothek fuer Zeitfunktionen

int main () {                                                                       // Beginn der Funktion main
    time_t zeit;                                                                    // Datentyp fuer Zeit, dargestellt mit Variable zeit
    struct tm *localTime;                                                           // Zeit-Struktur, Pointer localTime zeigt auf Struktur
    char timeString[100];                                                           // String fuer Zeit, 100 Zeichen langes Char-Array

    zeit = time(NULL);                                                              // Aktuelle Systemzeit wird abgerufen und in zeit gespeichert

    localTime = localtime(&zeit);                                                   // In Variable zeitt gespeicherte Zeit wird in lokale Zeit umgewandelt und in localTime gespeichert

    // Zeit in String umwandeln
    strftime(timeString, sizeof(timeString), "%a %b %d %H:%M:%S %Y", localTime);    // strftime() wandelt lokale Zeit in String mit bestimmtem Format um
                                                                                    // %a - Abkuerzung des Wochentags
                                                                                    // %b - Abkuerzung des Monats
                                                                                    // %d - Tag des Monats
                                                                                    // %H - Stunde (24-Stunden-Format)
                                                                                    // %M - Minute
                                                                                    // %S - Sekunde
                                                                                    // %Y - Vierstellige Jahreszahl

    printf("Aktuelle Zeit mit time(), gefolgt von localtime() und strftime():\n");
    // Ausgabe des Zeit-Strings
    printf("%s\n", timeString);                                                     // Ausgabe des formatierten Strings            

    return 0;                                                                       // Programmende
}