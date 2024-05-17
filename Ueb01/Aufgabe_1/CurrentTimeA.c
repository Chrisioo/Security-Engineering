#include<stdio.h>                                                   // Standardbibliothek fuer Ein- und Ausgabe
#include<time.h>                                                    // Bibliothek fuer Zeitfunktionen

int main() {                                                        // Beginn der Funktion main
    time_t zeit;                                                    // Datentyp fuer Zeit, dargestellt mit Variable zeit
    time(&zeit);                                                    // Ruft aktuelle Zeit auf und speichert sie in Variable zeit
    printf("Aktuelle Zeit mit time(), gefolgt von ctime():\n");     // Ausgabe eines Strings
    printf("%s", ctime(&zeit));                                     // Konvertiert Zeit in String und gibt sie aus, ctime(&zeit) gibt Pointer auf String zurueck
    return 0;                                                       // Programmende
}
