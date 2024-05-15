#include<stdio.h>
#include<time.h>

int main() {
    time_t t;                   // Zeit-Struktur
    time(&t);                   // Systemzeit abrufen
    printf("Aktuelle Zeit mit time() und ctime():\n");  
    printf("%s", ctime(&t));    // Zeit in String umwandeln und ausgeben
    return 0;                   // Programmende
}
