/*
 * Dateiname : 2D-Barcodes.c
 * Programm  : Dieses Programm liest wiederholt eine Zeichenkette ein, wandelt sie in einen Barcode um,
 *             speichert diesen Barcode in einer Datei und liest ihn anschließend wieder ein, um ihn zu dekodieren.
 * Eingaben  : Zeichenketten (über die Standardeingabe, also Tastatur)
 * Ausgaben  : - Eine Textdatei ("barcode_output.txt") mit dem Barcode (als Bitmuster, also aus 0 und 1)
 *             - Eine Konsolenausgabe mit der dekodierten Zeichenkette und eventuellen Warnmeldungen
 *
 * Autor     : Linke Dominik 11371232
 * Version   : 02 vom 2025.02.24
 */

 #include <stdio.h>      // Bibliothek für Ein- und Ausgabe (z. B. printf, fopen)
 #include <stdlib.h>     // Bibliothek für Speicherverwaltung (z. B. malloc, calloc, free)
 #include <string.h>     // Bibliothek für Zeichenkettenfunktionen (z. B. strlen, memcpy, strcpy)
 #include <math.h>       // Bibliothek für mathematische Funktionen (z. B. sqrt, ceil)
 #include <stdint.h>     // Bibliothek für Datentypen mit fester Größe (z. B. uint16_t, uint32_t)
 #include <ctype.h>      // Bibliothek für Zeichenüberprüfungen (z. B. isdigit, isalpha)
 
 
 /*
  * Funktion:  berechneQuersumme
  * Zweck:     Berechnet die Summe aller Bytes in einem Datenarray und gibt nur
  *            die unteren 16 Bit dieser Summe zurück (als Prüfsumme).
  * Parameter:
  *   - data:   Zeiger auf das Array, das die zu verarbeitenden Bytes enthält
  *   - laenge: Anzahl der Bytes im Array, die verarbeitet werden sollen
  * Rückgabewert:
  *   - Eine 16-Bit-Zahl (unsigned short), die die Prüfsumme darstellt
  */
 static uint16_t berechneQuersumme(const unsigned char *data, size_t laenge) {
     uint32_t sum = 0;              // Erstelle eine 32-Bit-Zahl, um die Summe der Bytes zu speichern
     for (size_t i = 0; i < laenge; i++) {  // Schleife von 0 bis zur Länge des Arrays (jedes Byte durchgehen)
         sum += data[i];            // Füge den Wert des aktuellen Bytes zur Summe hinzu
     }
     // Wir nehmen nur die unteren 16 Bit der Summe (das "& 0xFFFF" entfernt höhere Bits)
     return (uint16_t)(sum & 0xFFFF);
 }
 
 
 /*
  * Funktion:  MinSeitenLaenge
  * Zweck:     Bestimmt die kleinste Seitenlänge N eines quadratischen Barcodes,
  *            sodass N*N (also die Gesamtzahl der Bits) mindestens so groß ist wie die benötigte Bitanzahl.
  * Parameter:
  *   - bitCount: Die Anzahl der Bits, die gespeichert werden müssen
  * Rückgabewert:
  *   - Die kleinste Seitenlänge N, sodass N*N >= bitCount
  */
 static int MinSeitenLaenge(int bitCount) {
     // Berechne die Quadratwurzel von bitCount, um zu wissen, wie groß eine Seite sein müsste,
     // und runde mit ceil() auf die nächste ganze Zahl auf, falls die Wurzel nicht exakt passt.
     int N = (int)ceil(sqrt((double)bitCount));
     return N;  // Gib die berechnete Seitenlänge zurück
 }
 
 
 /*
  * Funktion:  errorMessage
  * Zweck:     Gibt eine Fehlermeldung auf dem Standardfehler (stderr) aus und versieht sie mit dem Präfix "[Fehler]".
  * Parameter:
  *   - msg: Der Text der Fehlermeldung, der ausgegeben werden soll
  * Rückgabewert:
  *   - Keiner (void)
  */
 static void errorMessage(const char *msg) {
     // Drucke die Fehlermeldung auf stderr (wird oft für Fehler genutzt, um sie von normalen Ausgaben zu trennen)
     fprintf(stderr, "[Fehler] %s\n", msg);
 }
 
 
 /*
  * Funktion:  encode
  * Zweck:     Wandelt einen Eingabestring in einen binären Barcode (als Bitfeld) um.
  *            Der Barcode wird als quadratisches Bitfeld (N x N) gespeichert.
  * Parameter:
  *   - input:       Der Eingabestring, der kodiert werden soll (maximal 255 Zeichen)
  *   - barcodeBits: Zeiger auf einen Zeiger, in den der Speicher für das Barcode-Bitfeld eingetragen wird
  *   - size:        Zeiger auf einen int, in dem die Seitenlänge (N) des Barcodes zurückgegeben wird
  * Rückgabewert:
  *   - Keiner (void). Die Ergebnisse werden über die Parameter barcodeBits und size zurückgegeben.
  */
 void encode(const char *input, unsigned char **barcodeBits, int *size)
 {
     // Ermittele die Länge des Eingabestrings
     size_t inputLen = strlen(input);
     // Falls der String länger als 255 Zeichen ist, wird er abgeschnitten
     if (inputLen > 255) {
         fprintf(stderr, "[Warnung] Eingabe laenger als 255 Zeichen. Abschneiden.\n");
         inputLen = 255;  // Begrenze die Länge auf 255
     }
     // Falls keine Zeichen eingegeben wurden, wird eine Fehlermeldung ausgegeben und die Funktion beendet
     if (inputLen < 1) {
         errorMessage("Eingabe darf nicht leer sein.");
         return;
     }
 
     // Erstelle einen temporären Puffer, der alle Daten aufnehmen kann:
     // 1 Byte für die Länge, max. 255 Zeichen, 2 Byte für die Endekennung, 2 Byte für die Prüfsumme
     unsigned char datenPuffer[260];
     size_t pos = 0;  // Variable, um die aktuelle Schreibposition im Puffer zu verfolgen
 
     // Speichere als erstes Byte die Länge des Eingabestrings (als Zahl)
     datenPuffer[pos++] = (unsigned char)(inputLen & 0xFF);
 
     // Kopiere den Eingabestring in den Puffer (Zeichen für Zeichen)
     memcpy(&datenPuffer[pos], input, inputLen);
     pos += inputLen;  // Aktualisiere die Position um die Anzahl der kopierten Zeichen
 
     // Füge eine Endekennung hinzu, bestehend aus zwei aufeinanderfolgenden 0x00 Bytes
     datenPuffer[pos++] = 0x00;
     datenPuffer[pos++] = 0x00;
 
     // Berechne die Prüfsumme (Quersumme) über alle bisher eingefügten Bytes im Puffer
     uint16_t checkSumme = berechneQuersumme(datenPuffer, pos);
 
     // Hänge die berechnete Prüfsumme (als 2 Bytes) an das Ende des Puffers:
     // Zuerst das höhere Byte (High-Byte)
     datenPuffer[pos++] = (unsigned char)((checkSumme >> 8) & 0xFF);
     // Dann das niedrigere Byte (Low-Byte)
     datenPuffer[pos++] = (unsigned char)(checkSumme & 0xFF);
 
     // dataLen speichert die gesamte Anzahl der benutzten Bytes im Puffer
     size_t dataLen = pos;
 
     // Berechne die gesamte Bitanzahl (1 Byte = 8 Bits)
     int totalBits = (int)(dataLen * 8);
 
     // Bestimme die minimale Seitenlänge N des quadratischen Barcodes,
     // sodass N * N mindestens so viele Bits aufnehmen kann wie totalBits
     int N = MinSeitenLaenge(totalBits);
 
     // Berechne, wie viele Bits in einem N x N-Feld insgesamt Platz haben
     int KapazitaetBits = N * N;
 
     // Berechne, wie viele Bits noch fehlen, um das Feld komplett zu füllen
     int NibbleRestlicheBits = KapazitaetBits - totalBits;
 
     int fillPos = 0;  // Variable, um abwechselnd zwei Füllwerte zu nutzen (0xEC und 0x11)
     // Solange die aktuelle Bitanzahl im Puffer kleiner ist als die Kapazität des Barcode-Felds:
     while ((int)(dataLen * 8) < KapazitaetBits) {
         // Füge abwechselnd 0xEC und 0x11 als Füllbytes ein, um den Puffer aufzufüllen
         if (fillPos == 0) {
             datenPuffer[dataLen++] = 0xEC;  // Füge 0xEC hinzu
             fillPos = 1;                   // Wechsel den Füllwert
         } else {
             datenPuffer[dataLen++] = 0x11;  // Füge 0x11 hinzu
             fillPos = 0;                   // Wechsel zurück zu 0xEC
         }
     }
 
     // finalBits speichert die finale Anzahl der Bits im Puffer
     int finalBits = (int)(dataLen * 8);
     // Falls wir mehr Bits haben als in ein N x N-Feld passen:
     if (finalBits > KapazitaetBits) {
         int excess = finalBits - KapazitaetBits;  // Berechne, wie viele Bits zu viel sind
         // Da wir nur in 4-Bit-Schritten (Nibble) kürzen, berechne den Rest:
         int NibbleRest = excess % 4;
         // Bestimme, wie viele Nibbles (4-Bit-Blöcke) gelöscht werden müssen
         int NibblesZuLoeschen = (excess / 4) + (NibbleRest > 0 ? 1 : 0);
         // Bestimme die tatsächliche Anzahl der zu löschenden Bits
         int bitsZuLoeschen = NibblesZuLoeschen * 4;
         // Berechne die neue Ziel-Bitanzahl
         int FinaleBits = finalBits - bitsZuLoeschen;
 
         // Bestimme, wie viele ganze Bytes (8-Bit-Blöcke) in der neuen Bitanzahl enthalten sind
         int newDataLen = FinaleBits / 8;
         // Bestimme, ob es noch verbleibende Bits gibt (weniger als 8)
         int RestBits = FinaleBits % 8;
 
         // Passe die Länge des Puffers auf die neue Anzahl ganzer Bytes an
         dataLen = newDataLen;
 
         // Falls noch Restbits übrig sind, muss das nächste Byte teilweise übernommen werden:
         if (RestBits > 0) {
             // Hole das Byte, in dem sich noch ein paar Bits befinden
             unsigned char lastByte = datenPuffer[newDataLen];
             // Erstelle eine Maske, um nur die relevanten Bits zu behalten (z. B. 4 Bits: 0xF0)
             unsigned char mask = (unsigned char)(0xFF << (8 - RestBits));
             // Wende die Maske an, um die nicht benötigten Bits zu löschen
             lastByte &= mask;
             // Schreibe das bearbeitete Byte zurück in den Puffer
             datenPuffer[newDataLen] = lastByte;
             // Erhöhe dataLen um 1, da dieses teilweise belegte Byte jetzt auch gezählt wird
             dataLen = newDataLen + 1;
         }
     }
 
     // Allokiere Speicher für das 2D-Barcode-Bitfeld (Quadrat mit N x N Feldern)
     // Alle Felder werden dabei initial auf 0 gesetzt
     *barcodeBits = (unsigned char*)calloc(N * N, sizeof(unsigned char));
     // Überprüfe, ob die Speicherallokierung erfolgreich war
     if (!(*barcodeBits)) {
         errorMessage("Speicherallokierung fehlgeschlagen");
         return;
     }
 
     int bitIndex = 0;  // Variable, die angibt, an welcher Stelle im Bitfeld wir gerade schreiben
     // Gehe durch jedes Byte im datenPuffer
     for (int byteIdx = 0; byteIdx < (int)dataLen; byteIdx++) {
         // Speichere das aktuelle Byte in einer Variablen
         unsigned char aktuelleBytes = datenPuffer[byteIdx];
         // Gehe jedes Bit des aktuellen Bytes durch (von Bit 0 bis Bit 7)
         for (int b = 0; b < 8; b++) {
             // Falls bereits alle Felder im 2D-Bitfeld belegt sind, verlasse die Schleife
             if (bitIndex >= N * N)
                 break;
             // Extrahiere das Bit an der Position b aus dem aktuellen Byte
             // (>> b verschiebt das Byte um b Positionen nach rechts, & 0x01 liest das niedrigste Bit)
             unsigned char bit = (aktuelleBytes >> b) & 0x01;
             // Berechne die Zeilennummer im 2D-Feld anhand von bitIndex
             int row = bitIndex / N;
             // Berechne die Spaltennummer im 2D-Feld anhand von bitIndex
             int col = bitIndex % N;
             // Da die Bits von rechts nach links abgelegt werden sollen, wird die Spalte "umgedreht"
             int realCol = N - 1 - col;
 
             // Speichere das extrahierte Bit im 2D-Barcode-Feld an der berechneten Position
             (*barcodeBits)[row * N + realCol] = bit;
             // Erhöhe den Bitindex, damit das nächste Bit in das nächste Feld geschrieben wird
             bitIndex++;
         }
     }
 
     // Übergib die Seitenlänge des Barcodes (N) zurück
     *size = N;
 }
 
 
 /*
  * Funktion:  schreibeBarcode
  * Zweck:     Schreibt das N x N-Bitfeld als Barcode in eine Textdatei.
  *            Dabei wird jeder Eintrag (0 oder 1) als Zeichen geschrieben,
  *            und am Ende jeder Zeile wird ein Zeilenumbruch eingefügt.
  * Parameter:
  *   - barcodeBits: Zeiger auf das 2D-Bitfeld, das den Barcode darstellt
  *   - N:           Die Seitenlänge des quadratischen Barcode-Feldes
  *   - filename:    Der Name der Datei, in die der Barcode geschrieben wird
  * Rückgabewert:
  *   - Keiner (void)
  */
 void schreibeBarcode(const unsigned char *barcodeBits, int N, const char *filename)
 {
     // Öffne die Datei im Schreibmodus ("w")
     FILE *fp = fopen(filename, "w");
     // Überprüfe, ob die Datei erfolgreich geöffnet werden konnte
     if (!fp) {
         errorMessage("Konnte Ausgabedatei nicht oeffnen");
         return;
     }
 
     // Gehe jede Zeile des Barcode-Feldes durch
     for (int row = 0; row < N; row++) {
         // Gehe jede Spalte der aktuellen Zeile durch
         for (int col = 0; col < N; col++) {
             // Schreibe das Bit (als Zahl, 0 oder 1) in die Datei
             // Dabei wird das Feld als eindimensionales Array betrachtet: (row * N + col)
             fprintf(fp, "%d", barcodeBits[row * N + col]);
         }
         // Nach jeder Zeile wird ein Zeilenumbruch in die Datei geschrieben
         fprintf(fp, "\n");
     }
 
     // Schließe die Datei, damit alle Daten gespeichert werden
     fclose(fp);
 }
 
 
 /*
  * Funktion:  readBarcode
  * Zweck:     Liest einen Barcode aus einer Textdatei ein, in der jede Zeile
  *            eine Folge von '0' und '1' enthält. Die eingelesenen Zeilen werden
  *            in ein quadratisches Bitfeld umgewandelt.
  * Parameter:
  *   - filename: Der Name der Datei, aus der der Barcode gelesen wird
  *   - size:     Zeiger auf einen int, in dem die ermittelte Seitenlänge (N) zurückgegeben wird
  * Rückgabewert:
  *   - Ein Zeiger auf das dynamisch allokierte 2D-Bitfeld, das den Barcode darstellt
  *     (bei Fehlern wird NULL zurückgegeben; der Aufrufer muss den Speicher später mit free() freigeben)
  */
 unsigned char* readBarcode(const char *filename, int *size)
 {
     // Öffne die Datei im Lesemodus ("r")
     FILE *fp = fopen(filename, "r");
     if (!fp) {
         errorMessage("Konnte Eingabedatei nicht oeffnen");
         return NULL;
     }
 
     char **lines = NULL;   // Dynamisches Array, um Zeilen aus der Datei zu speichern
     int maxLineLen = 0;    // Variable zur Speicherung der Länge der längsten Zeile
     int lineCount = 0;     // Zähler, wie viele Zeilen insgesamt gelesen werden
 
     {
         char buffer[1024];  // Temporärer Puffer, um jeweils eine Zeile einzulesen
         // Solange es eine Zeile in der Datei gibt, wird diese eingelesen
         while (fgets(buffer, sizeof(buffer), fp)) {
             // Bestimme die Länge der eingelesenen Zeile
             size_t len = strlen(buffer);
             // Entferne das Zeilenumbruchzeichen am Ende der Zeile, falls vorhanden
             if (len > 0 && buffer[len - 1] == '\n') {
                 buffer[len - 1] = '\0';
                 len--;
             }
             // Aktualisiere die maximale Zeilenlänge, falls diese Zeile länger ist
             if ((int)len > maxLineLen) {
                 maxLineLen = (int)len;
             }
 
             // Erhöhe das Array, um Platz für die neue Zeile zu schaffen
             lines = (char**)realloc(lines, sizeof(char*) * (lineCount + 1));
             // Allokiere Speicher für diese Zeile (einschließlich Nullterminator)
             lines[lineCount] = (char*)malloc(len + 1);
             // Kopiere den Inhalt des Puffers in das neu allokierte Speicherfeld
             strcpy(lines[lineCount], buffer);
             // Erhöhe den Zeilenzähler
             lineCount++;
         }
     }
     // Schließe die Eingabedatei
     fclose(fp);
 
     // Prüfe, ob der Barcode quadratisch ist (d.h. gleiche Anzahl an Zeilen wie Spalten)
     if (lineCount != maxLineLen) {
         // Wenn nicht, wird eine Warnung ausgegeben
         fprintf(stderr, "[Warnung] Eingelesener Barcode ist nicht quadratisch (Zeilen=%d, Spalten=%d)\n",
                 lineCount, maxLineLen);
     }
     
     // Wähle die kleinere Zahl zwischen Zeilenanzahl und maximaler Zeilenlänge als Seitenlänge N
     int N = (lineCount < maxLineLen) ? lineCount : maxLineLen;
     // Allokiere Speicher für das 2D-Bitfeld des Barcodes (N x N) und initialisiere alle Felder mit 0
     unsigned char *barcodeBits = (unsigned char*)calloc(N * N, sizeof(unsigned char));
     if (!barcodeBits) {
         errorMessage("Speicherallokierung fehlgeschlagen (readBarcode)");
         // Gebe den für die Zeilen allokierten Speicher frei
         for (int i = 0; i < lineCount; i++) {
             free(lines[i]);
         }
         free(lines);
         return NULL;
     }
 
     // Kopiere die Zeichen aus jeder Zeile in das 2D-Bitfeld
     for (int row = 0; row < N; row++) {
         // Bestimme die tatsächliche Länge der aktuellen Zeile
         int len = (int)strlen(lines[row]);
         for (int col = 0; col < N; col++) {
             if (col < len) {
                 // Falls das Zeichen '1' ist, speichere im Bitfeld den Wert 1,
                 // andernfalls (auch wenn es '0' ist) den Wert 0
                 barcodeBits[row * N + col] = (lines[row][col] == '1') ? 1 : 0;
             } else {
                 // Falls in der Zeile nicht genügend Zeichen vorhanden sind, fülle mit 0 auf
                 barcodeBits[row * N + col] = 0;
             }
         }
     }
 
     // Gebe den allokierten Speicher für die Zeilen wieder frei
     for (int i = 0; i < lineCount; i++) {
         free(lines[i]);
     }
     free(lines);
 
     // Übergib die ermittelte Seitenlänge N an den Aufrufer
     *size = N;
 
     // Gib den Zeiger auf das Barcode-Bitfeld zurück
     return barcodeBits;
 }
 
 
 /*
  * Funktion:  decode
  * Zweck:     Dekodiert das N x N-Bitfeld des Barcodes zurück in die ursprüngliche Zeichenkette.
  *            Dabei werden die Daten, die im Barcode gespeichert sind, wieder zu Bytes zusammengesetzt,
  *            die Länge, Endekennung und Prüfsumme überprüft und das Ergebnis ausgegeben.
  * Parameter:
  *   - barcodeBits: Zeiger auf das 2D-Bitfeld (Barcode)
  *   - N:           Die Seitenlänge des Barcode-Feldes
  * Rückgabewert:
  *   - Keiner (void). Die dekodierte Zeichenkette und eventuelle Warnungen werden auf der Konsole ausgegeben.
  */
 void decode(const unsigned char *barcodeBits, int N)
 {
     // Erstelle einen Puffer, in dem die extrahierten Bytes gespeichert werden sollen
     unsigned char datenPuffer[1024];
     int datenZaehler = 0;  // Zählt, wie viele Bytes bereits extrahiert wurden
 
     // Berechne die Gesamtzahl der Bits im Barcode (N x N)
     int totalBits = N * N;
     int bitIndex = 0;  // Index, der angibt, welches Bit aktuell verarbeitet wird
 
     unsigned char aktuelleByt = 0;  // Variable, um 8 Bits (ein Byte) zwischenzuspeichern
     int bitsCollected = 0;          // Zählt, wie viele Bits bereits im aktuellen Byte gesammelt wurden
 
     // Solange es noch Bits im Barcode gibt, werden diese ausgelesen
     while (bitIndex < totalBits) {
         // Berechne die aktuelle Zeile im Barcode
         int row = bitIndex / N;
         // Berechne die aktuelle Spalte im Barcode
         int col = bitIndex % N;
         // Da die Bits von rechts nach links gelesen werden sollen, wird die Spalte "umgedreht"
         int realCol = N - 1 - col;
 
         // Hole das Bit aus dem Barcode-Feld an der Position (row, realCol) und stelle sicher,
         // dass nur das niederwertigste Bit betrachtet wird (daher & 0x01)
         unsigned char bit = barcodeBits[row * N + realCol] & 0x01;
         // Schiebe das gefundene Bit an die richtige Position in das aktuelle Byte
         // (bitsCollected gibt an, wie viele Bits bereits gesammelt wurden)
         aktuelleByt |= (bit << bitsCollected);
 
         bitsCollected++;  // Erhöhe den Zähler für die gesammelten Bits
         if (bitsCollected == 8) { // Wenn 8 Bits gesammelt wurden, ist ein komplettes Byte entstanden
             datenPuffer[datenZaehler++] = aktuelleByt;  // Speichere das Byte im Puffer
             aktuelleByt = 0;       // Setze die Variable für das aktuelle Byte zurück
             bitsCollected = 0;     // Setze den Bit-Zähler zurück
         }
 
         bitIndex++;  // Gehe zum nächsten Bit im Barcode
         // Falls der Puffer voll ist, verlasse die Schleife, um einen Pufferüberlauf zu verhindern
         if (datenZaehler >= (int)sizeof(datenPuffer)) {
             break;
         }
     }
    
     // Falls weniger als 4 Bytes extrahiert wurden, können die Daten nicht dekodiert werden
     if (datenZaehler < 4) {
         printf("Zu wenige Daten, um Dekodierung durchzufuehren.\n");
         return;
     }
 
     // Das erste Byte im Puffer enthält die Länge der ursprünglichen Zeichenkette
     unsigned char laenge = datenPuffer[0];
     // Überprüfe, ob die Gesamtzahl der Daten (Länge + 5 Bytes) erreicht wurde
     if (datenZaehler < (int)(laenge + 5)) {
         printf("Warnung: Eingelesener Barcode hat weniger Daten als erwartet.\n");
     }
 
     // Berechne, wie viele Bytes theoretisch benötigt werden (Länge des Strings + 5 Zusatzbytes)
     size_t benoetigt = laenge + 5;
     if (benoetigt > (size_t)datenZaehler) {
         benoetigt = datenZaehler;
     }
     
     uint16_t LeseCheckSumme = 0;
     // Falls mindestens 2 Bytes vorhanden sind, bilden die letzten beiden Bytes die gespeicherte Prüfsumme
     if (benoetigt >= 2) {
         LeseCheckSumme = (datenPuffer[benoetigt - 2] << 8) | (datenPuffer[benoetigt - 1]);
     }
 
     // Berechne die Prüfsumme über die Daten (ausgenommen die letzten 2 Bytes, die die Prüfsumme enthalten)
     uint16_t calc = 0;
     if ((benoetigt - 2) > 0) {
         calc = berechneQuersumme(datenPuffer, benoetigt - 2);
     }
 
     // Erstelle einen Puffer für die dekodierte Zeichenkette (maximal 255 Zeichen + Nullterminator)
     unsigned char decodedString[256];
     // Setze den gesamten Speicher im Puffer auf 0 (damit ein leerer String entsteht)
     memset(decodedString, 0, sizeof(decodedString));
 
     // Kopiere die ursprünglichen Zeichen (beginnend bei Byte 1) in den decodedString
     // Achte darauf, dass maximal 255 Zeichen übernommen werden
     for (int i = 0; i < laenge && i < 255; i++) {
         decodedString[i] = datenPuffer[1 + i];
     }
     // Füge am Ende ein Nullzeichen ein, um den String zu terminieren
     decodedString[laenge] = '\0';
 
     // Überprüfe, ob die Endekennung (zwei aufeinanderfolgende 0x00 Bytes) vorhanden ist
     if (laenge + 1 < (int)benoetigt && laenge + 2 < (int)benoetigt) {
         if (datenPuffer[laenge + 1] != 0x00 || datenPuffer[laenge + 2] != 0x00) {
             printf("[Warnung] Das ende (0x00 0x00) fehlt oder ist fehlerhaft.\n");
         }
     }
 
     // Vergleiche die berechnete Prüfsumme mit der im Barcode gespeicherten Prüfsumme
     int CheckSummeOK = (calc == LeseCheckSumme);
 
     // Gebe die Länge und den dekodierten String auf der Konsole aus
     printf("Dekodierte Laenge: %d\n", laenge);
     printf("Dekodierte Zeichenkette: %s\n", decodedString);
 
     // Falls die Prüfsummen nicht übereinstimmen, gebe eine Warnung aus
     if (!CheckSummeOK) {
         printf("[Warnung] Quersumme stimmt nicht ueberein! Die Zeichenkette kann fehlerhaft sein.\n");
         printf("  Erwartet (berechnet): 0x%04X, Gelesen: 0x%04X\n", calc, LeseCheckSumme);
     }
 }
 
 
 /*
  * Funktion:  main
  * Zweck:     Hauptfunktion des Programms.
  *            Sie liest wiederholt eine Zeichenkette ein, kodiert sie in einen Barcode,
  *            speichert den Barcode in einer Datei, liest ihn wieder ein und dekodiert ihn.
  * Parameter:
  *   - Keine (Standard-Eintrittspunkt ohne Parameter)
  * Rückgabewert:
  *   - int: 0 bei erfolgreichem Ablauf, 1 bei Fehlern
  */
 int main(void)
 {
     int leseGroesse = 0;  // Variable, in der später die gelesene Seitenlänge des Barcodes gespeichert wird
     int size = 0;         // Variable, in der die Seitenlänge (N) des erzeugten Barcodes gespeichert wird
     
     // Endlosschleife: Das Programm läuft so lange, bis es manuell beendet wird
     while(1) {
         char eingabe[1024];  // Puffer, um die Benutzereingabe (Zeichenkette) zu speichern
         // Fordere den Benutzer zur Eingabe einer Zeichenkette auf (zwischen 1 und 255 Zeichen)
         printf("Bitte eine Zeichenkette (1..255 Zeichen) eingeben: ");
         // Lies eine Zeile von der Standardeingabe (Tastatur)
         if (!fgets(eingabe, sizeof(eingabe), stdin)) {
             errorMessage("Fehler beim Lesen der Eingabe");
             return 1;  // Beende das Programm, falls beim Einlesen ein Fehler auftritt
         }
         // Ermittle die Länge der eingegebenen Zeichenkette
         size_t len = strlen(eingabe);
         // Entferne den Zeilenumbruch (falls vorhanden), der am Ende der Eingabe steht
         if (len > 0 && eingabe[len - 1] == '\n') {
             eingabe[len - 1] = '\0';
         }
 
         unsigned char *barcode = NULL;  // Zeiger, der später auf das Barcode-Bitfeld zeigt (wird in encode() per calloc erstellt)
         
         // Rufe die Funktion encode auf, um die Eingabe in einen Barcode umzuwandeln
         encode(eingabe, &barcode, &size);
         // Überprüfe, ob der Barcode erfolgreich erstellt wurde
         if (!barcode || size == 0) {
             errorMessage("Barcode konnte nicht erstellt werden.");
             return 1;  // Beende das Programm, falls ein Fehler aufgetreten ist
         }
 
         // Definiere den Namen der Datei, in die der Barcode geschrieben werden soll
         const char *filename = "barcode_output.txt";
         // Schreibe den Barcode in die Datei
         schreibeBarcode(barcode, size, filename);
         // Gebe den für den Barcode allokierten Speicher wieder frei, da er nicht mehr benötigt wird
         free(barcode);
         barcode = NULL;
 
         // Informiere den Benutzer, dass der Barcode in die Datei geschrieben wurde
         printf("\nBarcode wurde in '%s' geschrieben.\n", filename);
 
         // Lese den Barcode aus der Datei wieder ein
         unsigned char *leseBits = readBarcode(filename, &leseGroesse);
         // Überprüfe, ob das Einlesen erfolgreich war
         if (!leseBits || leseGroesse == 0) {
             errorMessage("Barcode konnte nicht gelesen werden.");
             return 1;
         }
 
         // Gebe einen Trenner in der Ausgabe aus, um die Dekodierung zu kennzeichnen
         printf("\n--- Dekodierung ---\n");
         // Dekodiere den eingelesenen Barcode und gebe das Ergebnis auf der Konsole aus
         decode(leseBits, leseGroesse);
 
         // Gebe den Speicher, der beim Einlesen des Barcodes allokiert wurde, wieder frei
         free(leseBits);
         
         // Warte auf eine Eingabe, bevor die Schleife erneut beginnt
         getchar();
     }
     return 0;  // Rückgabe 0, falls das Programm normal beendet wird (dieser Punkt wird in dieser Endlosschleife theoretisch nie erreicht)
 }
 