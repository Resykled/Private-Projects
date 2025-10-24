/*
 Dateiname : Arbeitszeit_tracking.c
 Programm  : Eine software um arbeitszeit zu erfassen mit oder ohne soll zeit , funktioniert über mitternacht hinaus und track die zeiten in einer datei
 Eingaben  : Sollwert Ja / nein, startwert, endwert
 Ausgaben  : Rückgabewerte des Programms:
             Gibt die gearbeiteten stunden wieder aus , sagt ob es zu wenig waren bei soll zeit 

 Autor     : Linke Dominik 11371232
 Version   : 02   vom   2025.02.19
 */


#include <stdio.h>  // standart bib
#include <stdlib.h> // für speicherverwaltung etc
#include <string.h> // für string verarbeitung bzw manipulation
#include <time.h>  // für zeit bzw datum tracking 

/*
 * Funktion:  appendLine
 * Zweck:     Öffnet eine Datei im Anfüge-Modus (append) und schreibt eine Textzeile ans Ende.
 * Parameter:
 *   - filename: Dateiname oder Pfad, in den geschrieben werden soll
 *   - line:     Zeile, die ans Ende der Datei angehängt wird
 * Rückgabewert:
 *   - keiner (void), aber bei Fehlern wird eine Fehlermeldung auf dem Bildschirm ausgegeben
 */
void appendLine(const char *filename, const char *line)   
// die funtkion öffent eine datei ( in diesem fall für die arbeitszeiten) und fügt eine neue line hinzu
{
    FILE *fp = fopen(filename, "a");  
    // datei wird geöffnet mit modus a ( anfügen)
    if (fp) 
    {
        fprintf(fp, "%s\n", line); // zeile in datei schreiben
        fclose(fp); // datei wieder schließen
    }
    else 
    {
        printf("Fehler: Kann '%s' nicht oeffnen!\n", filename); // wenn es keine datei gibt bzw der name falsch ist kommt hier eine fehler meldung 
    }
}
/*
 * Funktion:  parseSollTime
 * Zweck:     Parst (liest) aus einem gegebenen String eine Sollzeit im Format "HH:MM" 
 *            und schreibt die Werte in zwei int-Variablen (Stunden, Minuten).
 * Parameter:
 *   - input:  Der String, z. B. "08:30"
 *   - hour:   Pointer auf eine int-Variable, in die die Stunden geschrieben werden
 *   - minute: Pointer auf eine int-Variable, in die die Minuten geschrieben werden
 * Rückgabewert:
 *   - 1, wenn erfolgreich im Format "HH:MM" gelesen werden konnte
 *   - 0, wenn das Format nicht passt
 */
int parseSollTime(const char *input, int *hour, int *minute)  
// die funktion ließt die soll zeit ein in HH:MM
{
    // Erwartetes Format: HH:MM
    return (sscanf(input, "%d:%d", hour, minute) == 2);
}

/*
 * Funktion:  parseDateTime
 * Zweck:     Liest einen String im Format "YYYY-MM-DD HH:MM:SS" in eine struct tm ein.
 * Parameter:
 *   - input:   Der String mit Datum und Uhrzeit, z. B. "2025-02-19 08:15:00"
 *   - outTm:   Pointer auf eine struct tm, in die die gelesenen Werte übertragen werden
 * Rückgabewert:
 *   - 1, wenn erfolgreich alle Daten ausgelesen wurden
 *   - 0, wenn das Format nicht passt
 */
int parseDateTime(const char *input, struct tm *outTm)  
// die funktion ließt die zeit und das datum ein im format YYYY-MM-DD HH:MM:SS und speichert die struktur die später verwendet wird für die zeitberechnung 
{
    int year, month, day, hour, min, sec;
    int n = sscanf(input, "%d-%d-%d %d:%d:%d", 
                   &year, &month, &day,
                   &hour, &min, &sec); 
    if (n != 6)
    {
        return 0; // wenn das format falsch ist kommt hier ein fehler 
    }

    memset(outTm, 0, sizeof(struct tm)); // struktur der speicherung 
    outTm->tm_year = year - 1900;
    outTm->tm_mon  = month - 1;
    outTm->tm_mday = day;
    outTm->tm_hour = hour;
    outTm->tm_min  = min;
    outTm->tm_sec  = sec;
    return 1;
}

/*
 * Funktion:  formatHMS
 * Zweck:     Zerlegt eine Gesamtanzahl an Sekunden (totalSec) in Stunden, Minuten und Sekunden.
 * Parameter:
 *   - totalSec: Anzahl der Gesamt-Sekunden, die umgewandelt werden
 *   - hh, mm, ss: Zeiger auf int-Variablen, um das Ergebnis zu speichern
 */
void formatHMS(long totalSec, int *hh, int *mm, int *ss) // funktion zum umwanden von sekunden in stunden und minuten in sekunden
{
    *hh = (int)(totalSec / 3600);// stunden berechnen
    totalSec %= 3600;            // übrige sekunden
    *mm = (int)(totalSec / 60);  // minuten berechnen
    *ss = (int)(totalSec % 60); // übrige sekunden
}

/*
 * Hauptprogramm: main
 * Zweck:         Steuert den kompletten Ablauf des Arbeitszeit-Trackings.
 *                Fragt Eingaben ab (Sollzeit ja/nein, Startzeit, Endzeit),
 *                berechnet Arbeitszeit, Differenz zur Sollzeit und speichert 
 *                Daten in einer Textdatei (arbeitszeit.txt).
 */
int main(void)
{
    

    // Name der Arbeitszeitdatei
    const char *filename = "arbeitszeit.txt";
    
    // Eingabepuffer und Hilfsvariablen
    char weitereEingabe =  '\0';    // Für die Frage "Neue Arbeitszeit erfassen? (j/n)"
    char sollBuffer[32] = {0};  // Für Sollzeit-Eingabe (HH:MM)
    char startBuffer[64] = {0};    // Für Startzeit (YYYY-MM-DD HH:MM:SS)
    char endBuffer[64] = {0};    // Für Endzeit   (YYYY-MM-DD HH:MM:SS)
    char sollzeitAntwort = '\0';    // Für die Frage "Sollzeit eingeben? (j/n)"
    char line[128] = {0};          // Für das Zusammenbauen von Dateizeilen
    
    // Sollzeit
    int sollH = 0, sollM = 0;        
    int sollzeitVorhanden = 0;       // Flag, ob eine Sollzeit eingegeben wurde
    
    // Strukturvariablen für Start- und Endzeit
    struct tm tmStart = {0};
    struct tm tmEnd = {0};
    
    // Zeitberechnungsvariablen
    long sollSec = 0;        
    long workSec = 0;        
    long diffSec = 0;        
    long diffAbs = 0;        
    int sign = 0;            
    // Für die Ausgabe in Stunden/Minuten/Sekunden
    int wH = 0, wM = 0, wS = 0; // Tatsächliche Arbeitszeit
    int sH = 0, sM = 0, sS = 0; // Sollzeit (Sekunden = 0)
    int dH = 0, dM = 0, dS = 0; // Differenz (Arbeitszeit - Sollzeit)

    // Konstante maximale Arbeitszeit
    long maxArbeitszeitSekunden = 12 * 60 * 60;  // 12 Stunden in Sekunden

    // ----------------------------
    // Start der Programmschleife
    // ----------------------------
    while (1)
    {
        // Abfrage, ob wir überhaupt eine weitere Zeiterfassung machen wollen
        printf("Moechtest du eine neue Arbeitszeit erfassen? (j/n): ");
        scanf(" %c", &weitereEingabe);
        getchar(); // Restlichen Puffer leeren (Entfernt das '\n')

        // Wenn 'n' oder 'N', Schleife (und damit Programm) beenden
        if (weitereEingabe == 'n' || weitereEingabe == 'N') 
        {
            break;
        }

        // ----------------------------
        // Sollarbeitszeit abfragen
        // ----------------------------
        printf("Moechtest du eine Sollarbeitszeit eingeben? (j/n): ");
        scanf(" %c", &sollzeitAntwort);
        getchar(); // Puffer leeren

        if (sollzeitAntwort == 'j' || sollzeitAntwort == 'J')
        {
            // Sollzeit eingeben
            printf("Bitte Sollzeit eingeben (Format HH:MM): ");
            if (!fgets(sollBuffer, sizeof(sollBuffer), stdin)) 
            {
                printf("Fehler beim Lesen der Eingabe.\n");
                return 1;
            }
            // Zeilenumbruch entfernen
            sollBuffer[strcspn(sollBuffer, "\n")] = '\0';

            // Format prüfen
            if (!parseSollTime(sollBuffer, &sollH, &sollM))
            {
                printf("Fehlerhaftes Format. Erwartet: HH:MM\n");
                return 1;
            }
            sollzeitVorhanden = 1; // Flag setzen

            // In Datei schreiben: "soll HH:MM"
            snprintf(line, sizeof(line), "soll %s", sollBuffer);
            appendLine(filename, line);
        }
        else
        {
            // Keine Sollzeit
            sollzeitVorhanden = 0;
            sollH = 0;
            sollM = 0;
        }

        // ----------------------------
        // Startzeit abfragen
        // ----------------------------
        printf("Bitte Startzeit eingeben (Format YYYY-MM-DD HH:MM:SS): ");
        if (!fgets(startBuffer, sizeof(startBuffer), stdin)) 
        {
            printf("Fehler beim Lesen der Eingabe.\n");
            return 1;
        }
        // Zeilenumbruch entfernen
        startBuffer[strcspn(startBuffer, "\n")] = '\0';

        // Umwandeln in struct tm
        if (!parseDateTime(startBuffer, &tmStart))
        {
            printf("Fehlerhaftes Format. Erwartet: YYYY-MM-DD HH:MM:SS\n");
            return 1;
        }

        // In die Datei schreiben: "start YYYY-MM-DD HH:MM:SS"
        snprintf(line, sizeof(line), "start %s", startBuffer);
        appendLine(filename, line);

        // ----------------------------
        // Endzeit abfragen
        // ----------------------------
        printf("Bitte Endzeit eingeben (Format YYYY-MM-DD HH:MM:SS): ");
        if (!fgets(endBuffer, sizeof(endBuffer), stdin))
        {
            printf("Fehler beim Lesen der Eingabe.\n");
            return 1;
        }
        // Zeilenumbruch entfernen
        endBuffer[strcspn(endBuffer, "\n")] = '\0';

        // Umwandeln in struct tm
        if (!parseDateTime(endBuffer, &tmEnd))
        {
            printf("Fehlerhaftes Format. Erwartet: YYYY-MM-DD HH:MM:SS\n");
            return 1;
        }

        // In die Datei schreiben: "end YYYY-MM-DD HH:MM:SS"
        snprintf(line, sizeof(line), "end %s", endBuffer);
        appendLine(filename, line);

        // ----------------------------
        // Berechnungen
        // ----------------------------
        // Sollzeit in Sekunden
        if (sollzeitVorhanden)
        {
            sollSec = (long)sollH * 3600L + (long)sollM * 60L;
        }
        else
        {
            sollSec = 0;
        }

        // Zeitpunkte in time_t umwandeln
        time_t startTime = mktime(&tmStart);
        time_t endTime   = mktime(&tmEnd);

        // Tatsächliche Arbeitszeit in Sekunden
        workSec = (long)(endTime - startTime);

        // Prüfen, ob mehr als 12h gearbeitet wurde
        if (workSec > maxArbeitszeitSekunden)
        {
            printf("\nAchtung: Du hast mehr als 12 Stunden gearbeitet! (tatsaechlich: %.2f Stunden)\n",
                   workSec / 3600.0);
        }

        // Falls Endzeit vor Startzeit lag, auf 0 setzen
        if (workSec < 0)
        {
            printf("\nAchtung: Endzeit liegt vor Startzeit! Setze Arbeitszeit auf 0.\n");
            workSec = 0;
        }

        // Differenz zur Sollzeitü
        if (sollzeitVorhanden)
        {
            diffSec = workSec - sollSec; // berechent die differenz
        }
        else
        {
            diffSec = 0; // wenn keine sollzeit vorhanden ist wird 0 zurück gegeben
        }

        // Ist-Zeit aufsplitten
        formatHMS(workSec, &wH, &wM, &wS);

        // Kopieren der Sollzeit in h/m/s (Sekunden sind hier immer 0)
        sH = sollH;
        sM = sollM;
        sS = 0;

        // Vorzeichen und absoluter Wert der Differenz
        diffAbs = (diffSec >= 0) ? diffSec : -diffSec;
        sign = (diffSec >= 0) ? 1 : -1;

        // Differenz in h/m/s
        formatHMS(diffAbs, &dH, &dM, &dS);

      
        // Ausgabe 
        printf("\n========== AUSWERTUNG ==========\n");
        if (sollzeitVorhanden) {  // wenn eine sollzeit vorhanden ist wird das hier ausgegeben
            printf("Sollarbeitszeit:     %02d:%02d:%02d\n", sH, sM, sS); // ausgabe im format HH:MM:SS
        } else {
            printf("Keine Sollarbeitszeit angegeben.\n"); //alternative wenn keine sollzeit angegeben ist 
        }

        printf("Tatsaechliche Zeit:   %02d:%02d:%02d\n", wH, wM, wS); // gibt die tazächliche bzw gearbeitete zeit wieder im format HH:MM:SS

        if (sollzeitVorhanden) {
            printf("Arbeitszeitguthaben: %c%02d:%02d:%02d\n",  // wenn eine sollzeit vorhanden ist wird hier das arbeitszeit guhaben ausgegeben
                   (sign > 0 ? '+' : '-'), dH, dM, dS);
        }
        printf("================================\n\n");
    }

    // Am Ende: Warten, damit das Konsolenfenster sich nicht sofort schließt
    printf("Druecke ENTER zum Beenden...\n");
    getchar();
    
    return 0;
}