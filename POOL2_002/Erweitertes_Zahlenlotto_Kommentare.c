#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Diese Konstante legt fest, wie viele Tippzahlen maximal verwaltet werden können.
#define MAX_TIPPS 100

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
    // Falls k größer als n ist, ist das mathematisch unmöglich -> direkt 0 zurückgeben
    if (k > n) 
        return 0;

    // Wenn k == 0 oder k == n, ist das Ergebnis 1 (null Elemente auswählen oder alle)
    if (k == 0 || k == n) 
        return 1;

    // Lokale Variablen für Zähler (num) und Nenner (denom) der Berechnung
    unsigned long num = 1;
    unsigned long denom = 1;

    // In dieser Schleife wird das Produkt (n * (n-1) * ...) geteilt durch (k * (k-1) * ... ) berechnet
    for (int i = 1; i <= k; i++)
    {
        // Zähler wird mit dem Faktor (n - (k - i)) multipliziert
        num *= (n - (k - i));

        // Nenner wird mit i multipliziert
        denom *= i;
    }

    // Das Endergebnis ist num / denom
    return num / denom;
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
    // i dient als Index zum Füllen des Arrays "numbers"
    int i = 0;

    // Schleife, bis k Zahlen im Array stehen
    while (i < k)
    {
        // Erzeugt eine Zufallszahl zwischen 1 und n (mittels % n + 1)
        int num = rand() % n + 1;

        // Flag-Variable, um festzustellen, ob wir ein Duplikat gefunden haben
        int duplicate = 0;

        // Schleife überprüft bereits eingetragene Zahlen (Index j < i)
        for (int j = 0; j < i; j++)
        {
            // Falls die neue Zahl bereits existiert:
            if (numbers[j] == num)
            {
                // Duplikat markieren
                duplicate = 1;

                // Diese Funktion gibt 1 zurück, wenn ein Duplikat entdeckt wurde
                //return 1;
            }
        }

        // Wenn kein Duplikat gefunden wurde, speichert man num ins Array
        if (!duplicate)
        {
            numbers[i] = num;
            i++;
        }
    }

    // 0 bedeutet: keine Duplikate gefunden, alles lief erfolgreich
    return 0;
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
    // Variable zum Zählen der Treffer
    int count = 0;

    // Äußere Schleife über alle Elemente im Array 'draw'
    for (int i = 0; i < k; i++)
    {
        // Innere Schleife über alle Elemente im Array 'tip'
        for (int j = 0; j < k; j++)
        {
            // Wenn die aktuelle Zahl in 'draw' in 'tip' auftaucht,
            // wird der Trefferzähler erhöht
            if (draw[i] == tip[j])
            {
                count++;
            }
        }
    }

    // Rückgabe der Gesamtzahl an Treffern
    return count;
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
    // Äußere Schleife: läuft bis k-1
    for (int i = 0; i < k - 1; i++)
    {
        // Innere Schleife: vergleicht das aktuelle Element mit den folgenden
        for (int j = i + 1; j < k; j++)
        {
            // Bei Bedarf tauschen wir die beiden Werte, 
            // sodass das kleinere Element nach vorne rutscht
            if (numbers[i] > numbers[j])
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
    // Variable n: maximale Zahl (z.B. wenn n=49, sind mögliche Zahlen 1..49)
    int n = 0;

    // Variable k: Anzahl der zu ziehenden Zahlen (z.B. 6 bei Lotto)
    int k = 0;

    // Array für die Benutzertipps (z.B. 6 Zahlen)
    int tip[MAX_TIPPS];

    // Array für die gezogenen Zufallszahlen
    int draw[MAX_TIPPS];

    // Zähler für die Anzahl korrekter Tipps
    int correct;

    // Binomialkoeffizient "n über k", mögliche unterschiedliche Ziehungsergebnisse
    unsigned long possibleResults;

    // Zählt, wie viele Ziehungen durchgeführt wurden, bis mindestens 3 Treffer vorliegen
    int totalDraws;

    // Initialisierung des Zufallsgenerators mit der aktuellen Uhrzeit
    srand(time(NULL));

    // Endlosschleife, damit das Programm nach einer Beendigung der Ziehung 
    // erneut starten kann
    while (1)
    {
        // Abfragen von n (maximale Zahl); 
        // Eingabe wird so oft wiederholt, bis etwas Gültiges eingegeben wurde
        printf("Bitte geben Sie die maximale Zahl (n) ein (groesser als 0): ");
        while (scanf("%d", &n) != 1 || n <= 0)
        {
            printf("Ungueltige Eingabe. Bitte eine positive ganze Zahl fuer n eingeben: ");
            while (getchar() != '\n'); // Leert den Eingabepuffer
        }

        // Abfragen von k (Anzahl zu ziehender Zahlen);
        // k muss im Bereich 1..n liegen
        printf("Bitte geben Sie die Anzahl der zu ziehenden Zahlen (k) ein (groesser als 0): ");
        while (scanf("%d", &k) != 1 || k <= 0 || k > n)
        {
            printf("Ungueltige Eingabe. Bitte eine Zahl fuer k im Bereich 1 bis %d eingeben: ", n);
            while (getchar() != '\n'); // Leert den Eingabepuffer
        }

        // Abfragen der einzelnen Tippzahlen vom Benutzer
        // Dabei dürfen keine Duplikate eingegeben werden, 
        // und jede Zahl muss zwischen 1 und n liegen.
        printf("Bitte geben Sie %d verschiedene Zahlen zwischen 1 und %d ein:\n", k, n);
        for (int i = 0; i < k; i++)
        {
            // So lange einlesen, bis eine gültige Zahl gefunden wird
            // (1 <= tip[i] <= n)
            while (scanf("%d", &tip[i]) != 1 || tip[i] <= 0 || tip[i] > n)
            {
                printf("Ungueltige Zahl. Bitte eine Zahl zwischen 1 und %d eingeben: ", n);
                while (getchar() != '\n');
            }

            // In einer Schleife prüfen, ob diese Zahl bereits zuvor eingegeben wurde
            for (int j = 0; j < i; j++)
            {
                if (tip[i] == tip[j])
                {
                    // Falls Duplikat vorhanden, wird i um 1 reduziert, 
                    // damit dieselbe Position erneut befüllt werden kann
                    printf("Die Zahl %d ist bereits im Tipp vorhanden. Bitte eine andere Zahl eingeben: ", tip[i]);
                    i--;
                    break;
                }
            }
        }

        // Generiert eine erste Zufallsziehung:
        // 1) ruft generateRandomNumbers auf, um k Zufallszahlen (1..n) zu erhalten
        // 2) sortiert das Ergebnis
        generateRandomNumbers(draw, n, k);
        sortNumbers(draw, k);

        // Sortiert auch die Tippzahlen des Benutzers zur übersichtlichen Ausgabe
        sortNumbers(tip, k);

        // Gibt das erste Ziehungsergebnis aus
        printf("\nZiehungsergebnis: ");
        for (int i = 0; i < k; i++)
        {
            printf("%d ", draw[i]);
        }
        printf("\n");

        // Gibt den eingegebenen Tipp aus
        printf("Ihr Tipp: ");
        for (int i = 0; i < k; i++)
        {
            printf("%d ", tip[i]);
        }
        printf("\n");

        // Zählt die Übereinstimmungen zwischen Tipp und erstem Ziehungsergebnis
        correct = countMatches(draw, tip, k);
        printf("Anzahl der richtig getippten Zahlen: %d\n", correct);

        // Berechnet, wie viele mögliche verschiedene Kombinationen (Ergebnisse) es 
        // für die Ziehung gibt (das ist der Binomialkoeffizient n über k)
        possibleResults = binomialCoefficient(n, k);
        printf("Anzahl der moeglichen Ziehungsergebnisse: %lu\n", possibleResults);

        // Falls beim ersten Versuch weniger als 3 Treffer vorliegen, 
        // werden weitere Ziehungen durchgeführt, bis mindestens 3 Richtige vorliegen.
        totalDraws = 1; 
        while (correct < 3)
        {
            // Erhöht den Zähler für die Ziehungen
            totalDraws++;

            // Generiert eine neue Ziehung und sortiert sie
            generateRandomNumbers(draw, n, k);
            sortNumbers(draw, k);

            // Zählt die Treffer mit dem Benutzer-Tipp
            correct = countMatches(draw, tip, k);

            // Gibt das Ergebnis der aktuellen Ziehung aus
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

        // Sobald mindestens 3 Richtige erreicht wurden, wird dies angezeigt
        printf("\nHerzlichen Glueckwunsch! Sie haben nach %d Ziehungen mindestens 3 Richtige getippt.\n", totalDraws);

        // Ein getchar(), damit die Benutzer*in Zeit hat, das Ergebnis zu sehen.
        // Anschließend geht die Schleife wieder nach oben, um eine neue Runde zu starten
        getchar();
    }

    // Rückgabe 0 bedeutet: Programm erfolgreich beendet
    return 0;
}
