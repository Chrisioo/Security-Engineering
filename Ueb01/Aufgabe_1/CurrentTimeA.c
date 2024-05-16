#include<stdio.h>                                                   // Standardbibliothek fuer Ein- und Ausgabe
#include<time.h>                                                    // Bibliothek fuer Zeitfunktionen

int main() {                                                        // Beginn der Funktion main
    time_t t;                                                       // Datentyp fuer Zeit, dargestellt mit Variable t
    time(&t);                                                       // Ruft aktuelle Zeit auf und speichert sie in Variable t
    printf("Aktuelle Zeit mit time(), gefolgt von ctime():\n");     // Ausgabe eines Strings
    printf("%s", ctime(&t));                                        // Konvertiert Zeit in String und gibt sie aus, ctime(&t) gibt Pointer auf String zurueck
    return 0;                                                       // Programmende
}
