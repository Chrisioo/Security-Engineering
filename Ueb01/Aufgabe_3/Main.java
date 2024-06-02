package Aufgabe_3;

import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
/**
 * Klasse Main, die die Methoden generateGivenNumber und simulateCollision enthält.
 * Die Methode generateGivenNumber generiert Zufallszahlen bis zu einer vorgegebenen Obergrenze n.
 * Die Methode simulateCollision simuliert eine Kollision bei der Generierung von Zufallszahlen.
 * Die Main-Methode nimmt die Eingabe des Benutzers entgegen und ruft die entsprechende Methode auf.
 * @author Christian Petry, Xudong Zhang
 * @version 1.0
 */
public class Main {
    private static int n = 10000;

    /**
     * Main-Methode, die die Eingabe des Benutzers entgegennimmt und die entsprechende Methode aufruft.
     * @param args Kommandozeilenargumente
     */
    public static void main (String[]args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Aktuelle Obergrenze: " + n);                                        
        System.out.print("Soll die Obergrenze geändert werden? (j/n): ");
        String auswahlObergrenze = scanner.nextLine();                                              // User-Input, ob die Obergrenze n geändert werden soll
        if (auswahlObergrenze.equalsIgnoreCase("j")) {
            System.out.println("Neue Obergrenze eingeben: ");
            int neueObergrenze = scanner.nextInt();                                                 // User-Input für die neue Obergrenze n auf Variable neueObergrenze 
            n = neueObergrenze;                                                                     // n wird auf den Wert von neueObergrenze gesetzt
        }
        System.out.println("Dateiname: ");
        String filename = scanner.next();                                                           // User-Input für den Dateinamen auf Variable filename                     
        System.out.print("1. Generiere Zahlen bis y\n2. Simuliere Kollision\nAuswahl: ");
        int auswahlMenue = scanner.nextInt();                                                       // User-Input für Menüauswahl auf Variable auswahlMenue
        if (auswahlMenue == 1) {                                                                    // Wenn die Menüauswahl 1 ist, wird die Methode generateGivenNumber aufgerufen
            System.out.print("Vorgegebener Wert für y: ");
            int y = scanner.nextInt();                                                              // User-Input für den vorgegebenen Wert y                             
            if (y > n) {
                System.out.println("y muss kleiner als n sein.");                                 // Fehlermeldung, wenn y größer n ist 
                System.exit(0);                                                              // Programm wird beendet
            }
            System.out.println(generateGivenNumber(n, y, filename));                                // Aufruf der Methode generateGivenNumber mit den Parametern n, y und filename
        } else if (auswahlMenue == 2) {                                                             // Wenn die Menüauswahl 2 ist, wird die Methode simulateCollision aufgerufen
            System.out.println(simulateCollision(n, filename));                                     // Aufruf der Methode simulateCollision mit den Parametern n und filename
        } else {
            System.out.println("Ungültige Option.");                                              // Wenn die Menüauswahl weder 1 noch 2 ist, wird eine Fehlermeldung ausgegeben                      
            System.exit(0);                                                                       // Programm wird beendet                     
        }
        scanner.close();
    }

    /**
     * Methode zum Generieren von Zufallszahlen bis zu einer vorgegebenen Obergrenze n.
     * Die Methode generiert Zufallszahlen, bis der vorgegebene Wert y generiert wird.
     * @param n Obergrenze für die Zufallszahlen
     * @param y Vorgegebener Wert für y, der generiert werden soll
     * @param filename Dateiname, in den die generierten Zahlen geschrieben werden
     * @return Anzahl der generierten Zahlen, bis y generiert wurde
     */
    public static int generateGivenNumber(int n, int y, String filename) {
        Random random = new Random();                                                               // Zufallszahlengenerator                  
        int zaehler = 0;                                                                            // Zähler für die Anzahl der generierten Zahlen
        int randomNumber;                                                                           // Variable für die generierte Zufallszahl             
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename + ".txt"))) {       // BufferedWriter zum Schreiben in Datei, Dateiname wird übergeben
            do {
                randomNumber = random.nextInt(n);                                                   // Zufällige Zahl zwischen 0 und n-1 generieren
                System.out.println("Generierte Zahl: " + randomNumber);                             // Generierte Zahl ausgeben
                writer.write(String.valueOf(randomNumber));                                         // Zahl in Datei schreiben
                writer.newLine();                                                                   // Neue Zeile für die nächste Zahl
                zaehler++;                                                                          // Zähler inkrementieren
            } while (randomNumber != y);                                                            // Schleife wiederholen, bis y generiert wird
        } catch (IOException e) {                                                                     
            e.printStackTrace();
        }
        return zaehler;                                                                             // Rückgabe der Anzahl generierter Zahlen
    }

    /**
     * Methode zum Simulieren einer Kollision bei der Generierung von Zufallszahlen.
     * @param n Obergrenze für die Zufallszahlen
     * @param filename Dateiname, in den die generierten Zahlen geschrieben werden
     * @return Anzahl der generierten Zahlen, bis eine Kollision auftritt
     */
    public static int simulateCollision (int n, String filename) {
        Random random = new Random();                                                               // Zufallszahlengenerator                  
        Set<Integer> generatedNumbers = new HashSet<>();                                            // Set zum Speichern der generierten Zahlen
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename + ".txt"))) {       // BufferedWriter zum Schreiben in Datei, Dateiname wird übergeben
            do {
                int randomNumber = random.nextInt(n);                                               // Zufällige Zahl zwischen 0 und n-1 generieren
                if (generatedNumbers.contains(randomNumber)) {                                      // Check, ob generierte Zahl bereits im Set enthalten ist
                    System.out.println("Kollision bei generierter Zahl: " + randomNumber);          // Ausgabe der generierten Zahl, bei der eine Kollision auftritt
                    writer.write(String.valueOf(randomNumber));                                     // Zahl in Datei schreiben
                    writer.write("\nKollision bei: " + String.valueOf(randomNumber));               // Zahl in Datei schreiben
                    break;                                                                          // Schleife beenden, wenn die generierte Zahl bereits im Set enthalten ist
                }
                System.out.println("Generierte Zahl: " + randomNumber);                             // Generierte Zahl ausgeben
                writer.write(String.valueOf(randomNumber));                                         // Zahl in Datei schreiben
                writer.newLine();                                                                   // Neue Zeile für die nächste Zahl
                generatedNumbers.add(randomNumber);                                                 // Generierte Zahl zur Menge hinzufügen
            } while (true);                                                                         // Endlosschleife, bis eine Kollision auftritt
        } catch (IOException e) {
            e.printStackTrace();
        }
        return generatedNumbers.size();                                                             // Rückgabe der Anzahl der eindeutigen generierten Zahlen
    }
}