package Aufgabe_3;

import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class Main {
    private static int n = 10000;

    public static void main (String[]args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Aktuelle Obergrenze: " + n);
        System.out.print("Soll die Obergrenze geändert werden? (j/n): ");
        String auswahlObergrenze = scanner.nextLine();
        if (auswahlObergrenze.equalsIgnoreCase("j")) {
            System.out.println("Neue Obergrenze eingeben: ");
            int neueObergrenze = scanner.nextInt();
            n = neueObergrenze;
        }
        System.out.print("1. Generiere Zahlen bis y\n2. Simuliere Kollision\nAuswahl: ");
        int auswahlMenue = scanner.nextInt();
        if (auswahlMenue == 1) {
            System.out.print("Vorgegebener Wert für y: ");
            int y = scanner.nextInt();
            if (y >= n) {
                System.out.println("y muss kleiner als n sein.");
                System.exit(0);
            }
            System.out.println(generateGivenNumber(n, y));
        } else if (auswahlMenue == 2) {
            System.out.println(simulateCollision(n));
        } else {
            System.out.println("Ungültige Option.");
            System.exit(0);
        }
        scanner.close();
    }

    public static int generateGivenNumber(int n, int y) {
        Random random = new Random();
        int zaehler = 0; // Zähler für die Anzahl der generierten Zahlen
        int randomNumber;
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("zahlen.txt"))) {
            do {
                randomNumber = random.nextInt(n); // Zufällige Zahl zwischen 0 und n-1 generieren
                System.out.println("Generierte Zahl: " + randomNumber);
                writer.write(String.valueOf(randomNumber)); // Zahl in Datei schreiben
                writer.newLine(); // Neue Zeile für die nächste Zahl
                zaehler++; // Zähler inkrementieren
            } while (randomNumber != y); // Schleife wiederholen, bis y generiert wird
        } catch (IOException e) {
            e.printStackTrace();
        }
        return zaehler; // Rückgabe der Anzahl generierter Zahlen
    }

    public static int simulateCollision (int n) {
        Random random = new Random();
        Set<Integer> generatedNumbers = new HashSet<>();
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("kollision.txt"))) {
            do {
                int randomNumber = random.nextInt(n); // Zufällige Zahl zwischen 0 und n-1 generieren
                if (generatedNumbers.contains(randomNumber)) {
                    System.out.println("Kollision bei generierter Zahl: " + randomNumber);
                    writer.write("Kollision bei: " + String.valueOf(randomNumber)); // Zahl in Datei schreiben
                    break; // Schleife beenden, wenn die generierte Zahl bereits in der Menge enthalten ist
                }
                System.out.println("Generierte Zahl: " + randomNumber);
                writer.write(String.valueOf(randomNumber)); // Zahl in Datei schreiben
                writer.newLine(); // Neue Zeile für die nächste Zahl
                generatedNumbers.add(randomNumber); // Generierte Zahl zur Menge hinzufügen
            } while (true); // generated number isnt part of the set
        } catch (IOException e) {
            e.printStackTrace();
        }
        return generatedNumbers.size(); // Rückgabe der Anzahl der eindeutigen generierten Zahlen
    }
}