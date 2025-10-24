/*
 Dateiname : ERweitertes_Zahlenlotto.c
 Programm  : Ein zahlenlotto das in einem bereich von 1 bis N  verschiedene 
 ziehungen durchführt basirend auf der größe von k, bis 3 gefunden wurden
 Eingaben  : Eingabe über die exe 
 Ausgaben  : Rückgabewerte des Programms:
             Gibt alle ziehungen wieder bis 3 richtige gefunden wurden, gibt dann nochmall alle möglichen ziehungen der kombination aus k&n aus 

 Autor     : Linke Dominik 11371232
 Version   : 02   vom   2025.02.19
 */


#include <stdio.h> // Importieren von Funktionen zur Ein-/Ausgabe
#include <stdlib.h> // Importieren von Standardfunktionen, z.B. Speicherverwaltung
#include <time.h> // Importieren von Zeitfunktionen

#define MAX_TIPPS 100 // Maximale Anzahl der Tipps, die erlaubt sind

/*
 * Funktion:  binomialCoefficient
 * Zweck:     Berechnet den Binomialkoeffizienten "n über k" (nCk).
 *            Dies ist die Anzahl der Möglichkeiten, k Elemente aus einer Menge 
 *            von n Elementen auszuwählen, ohne Reihenfolge zu berücksichtigen.
 * Parameter:
 *   - n: Gesamte Elemente
 *   - k: Anzahl der auszuwählenden Elemente
 * Rückgabewert:
 *   - Der Binomialkoeffizient als unsigned long. 
 *     Ist k > n, wird 0 zurückgegeben (keine Möglichkeiten).
 */
unsigned long binomialCoefficient(int n, int k)
{
    if (k > n) return 0; // Wenn k größer  als n ist, ist das Ergebnis 0
    if (k == 0 || k == n) return 1; // Wenn k gleich 0 oder n ist, ist das Ergebnis 1
    
    unsigned long num = 1; // Initialisierung des Zählers
    unsigned long denom = 1; // Initialisierung des Nenners
    
    for (int i = 1; i <= k; i++) 
    {
        num *= (n - (k - i)); // Berechnung des zählers
        denom *= i; // Berechnung des Nenners
    }
    
    return num / denom; // Zurückgeben des Ergebnisses
}

/*
 * Funktion:  generateRandomNumbers
 * Zweck:     Generiert k zufällige, eindeutige Zahlen (zwischen 1 und n) 
 *            und speichert sie im übergebenen Array 'numbers'.
 * Parameter:
 *   - numbers: Pointer auf ein int-Array, in dem die Zufallszahlen abgelegt werden
 *   - n:       Die maximale Zahl, aus der Zufallszahlen generiert werden (1..n)
 *   - k:       Wie viele Zufallszahlen benötigt werden
 * Rückgabewert:
 *   - 0, wenn alles ohne Duplikate erfolgreich war
 *   - 1, falls ein Duplikat entdeckt wurde (sofortiger Abbruch)
 */
int generateRandomNumbers(int *numbers, int n, int k)
{
    int i = 0;
    while (i < k)
    {
        int num = rand() % n + 1; // Generieren einer Zufallszahl zwischen 1 und n
        int duplicate = 0;
        for (int j = 0; j < i; j++) // Prüfen auf Duplikate
        {
            if (numbers[j] == num)
            {
                duplicate = 1; // Markieren als Duplikat
                //return 1; // Abbrechen bei Duplikaten
            }
        }
        if (!duplicate)
        {
            numbers[i] = num; // Hinzufügen der Zahl zum Array
            i++;
        }
    }
    return 0; // Erfolgreiches Generieren
}

/*
 * Funktion:  countMatches
 * Zweck:     Vergleicht zwei Arrays (draw und tip) miteinander und 
 *            ermittelt, wie viele gemeinsame Elemente sie besitzen.
 * Parameter:
 *   - draw: Array der gezogenen (Zufalls-)Zahlen
 *   - tip:  Array der vom Benutzer eingegebenen Tippzahlen
 *   - k:    Größe beider Arrays (beide haben k Elemente)
 * Rückgabewert:
 *   - Anzahl der Übereinstimmungen (Treffer)
 */

int countMatches(int *draw, int *tip, int k)
{
    int count = 0; // Initialisierung des Zälers
    for (int i = 0; i < k; i++) // schleife zum überprüfen der Ziehungszahlen
    {
        for (int j = 0; j < k; j++) // schleife zum überprüfen der Tipps
        {
            if (draw[i] == tip[j]) // Prüfen auf Übereinstimmungen
            {
                count++; // Erhöhen des Zählers bei Treffern
            }
        }
    }
    return count; // Zurückgeben der Trefferanzahl
}

/*
 * Funktion:  sortNumbers
 * Zweck:     Sortiert ein Array (numbers) aufsteigend nach dem Bubble-Sort-Prinzip.
 * Parameter:
 *   - numbers: Array mit k Elementen, das sortiert werden soll
 *   - k:       Anzahl der Elemente im Array
 */
void sortNumbers(int *numbers, int k)
{
    for (int i = 0; i < k - 1; i++)
    {
        for (int j = i + 1; j < k; j++) // schleife zur sortierung 
        {
            if (numbers[i] > numbers[j]) // fals ein element größer ist als des folgende werde die werte bzw die arrays getauscht 
            {
                int temp = numbers[i]; 
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }
}

/*
 * Hauptfunktion: main
 * Zweck:         Steuert den kompletten Programmablauf.
 *                Es wird mehrmals n (maximale Zahl), k (Anzahl zu ziehender Zahlen) 
 *                und die Benutzertipps eingelesen. 
 *                Danach erfolgen eine oder mehrere Zufallsziehungen, 
 *                bis mindestens 3 Übereinstimmungen auftreten.
 */
int main()
{
    // Alle Variablen gleich zu Beginn deklarieren:
    int n = 0;          // Maximale Zahl
    int k = 0;          // Anzahl zu ziehender Zahlen
    int tip[MAX_TIPPS] = {0}; // Feld für die Tipps
    int draw[MAX_TIPPS] = {0}; // Feld für die Zufallsziehung
    int correct = 0;   // Anzahl richtiger Tipps
    unsigned long possibleResults = 0;
    int totalDraws = 0;   // Zähler für die Anzahl an Ziehungen
    
    // Einmal zu Beginn den Zufallsgenerator initialisieren
    srand(time(NULL));

    // Endlosschleife, damit man immer wieder neue Tipps abgeben kann
    while (1)
    {
        //Eingabe von n
        printf("Bitte geben Sie die maximale Zahl (n) ein (groesser als 0): ");
    
        while (scanf("%d", &n) != 1 || n <= 1 || n > 100 || n <= 4 )  // überprüft ob n positiv ist und eine ganz zahl ist
        { 
            printf("Ungueltige Eingabe. Bitte eine positive ganze Zahl fuer n eingeben: ");
            while (getchar() != '\n'); // Eingabepuffer leeren
        }

        //Eingabe k
        printf("Bitte geben Sie die Anzahl der zu ziehenden Zahlen (k) ein (groesser als 0): ");
        while (scanf("%d", &k) != 1 || k <= 0 || k >= n || k <= 3) // überprüft ob k im bereich von 1 bis n liegt
        {
            printf("Ungueltige Eingabe. Bitte eine Zahl fuer k im Bereich 1 bis %d eingeben: ", n);
            while (getchar() != '\n');
        }

        //Benutzertipps einlesen
        printf("Bitte geben Sie %d verschiedene Zahlen zwischen 1 und %d ein:\n", k, n);
        for (int i = 0; i < k; i++)
        {
            while (scanf("%d", &tip[i]) != 1 || tip[i] <= 0 || tip[i] > n) // prüfung ob die eingabe numerisch ist und im bereich von k - n liegt 
            {
                printf("Ungueltige Zahl. Bitte eine Zahl zwischen 1 und %d eingeben: ", n);
                while (getchar() != '\n'); // filter für enter bzw \n
            }
            // Prüfung auf Duplikate
            for (int j = 0; j < i; j++)
            {
                if (tip[i] == tip[j])
                {
                    printf("Die Zahl %d ist bereits im Tipp vorhanden. Bitte eine andere Zahl eingeben: ", tip[i]);
                    i--; // i zurücksetzen, damit dieselbe Position noch einmal befüllt wird
                    break;
                }
            }
        }

        //Erste Zufallsziehung
        generateRandomNumbers(draw, n, k); // Generiert k Zufallszahlen [1..n], schreibt sie ins Array 'draw'
        sortNumbers(draw, k); // Sortiert 'draw' anschließend aufsteigend
        sortNumbers(tip, k); // Sortiert auch den Nutzer-Tipp 'tip' aufsteigend (für die Ausgabe)

        // Ausgabe
        printf("\nZiehungsergebnis: ");
        for (int i = 0; i < k; i++)
        {
            printf("%d ", draw[i]);
        }
        printf("\n");

        printf("Ihr Tipp: ");
        for (int i = 0; i < k; i++)
        {
            printf("%d ", tip[i]);
        }
        printf("\n");

        // anzahl der richtigen tipps zählen
        correct = countMatches(draw, tip, k);
        printf("Anzahl der richtig getippten Zahlen: %d\n", correct);

        // Mögliche Ergebnisse berechnen (Binomialkoeffizient)
        possibleResults = binomialCoefficient(n, k);
        printf("Anzahl der moeglichen Ziehungsergebnisse: %lu\n", possibleResults);

        // Weitere Ziehungen solange es < 3 Richtige sind
        totalDraws = 1;
        while (correct < 3)
        {
            totalDraws++; // zähler für ziehungen um 1 bei jedem durchlauf erhöhen
            generateRandomNumbers(draw, n, k); // neue zahlen generieren und sortieren
            sortNumbers(draw, k);

            correct = countMatches(draw, tip, k); // treffer erneut zählen

            // Zwischenstand ausgeben
            printf("\nZiehung %d:\n", totalDraws);
            printf("Ziehungsergebnis: ");
            for (int i = 0; i < k; i++)
            {
                printf("%d ", draw[i]);
            }
            printf("\n");

            printf("Ihr Tipp: ");
            for (int i = 0; i < k; i++)
            {
                printf("%d ", tip[i]);
            }
            printf("\n");

            printf("Anzahl der richtig getippten Zahlen: %d\n", correct);
        }

        printf("\nHerzlichen Glueckwunsch! Sie haben nach %d Ziehungen mindestens 3 Richtige getippt.\n", totalDraws); // sobalt es 3 richtige gezogen werden wird das hier ausgegeben
        possibleResults = binomialCoefficient(n, k);
        printf("Anzahl der moeglichen Ziehungsergebnisse: %lu\n", possibleResults);
        // Warten auf ENTER, damit man ggf. erneut starten kann
        getchar(); 
    }

    return 0;
}