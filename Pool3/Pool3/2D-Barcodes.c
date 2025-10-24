/*
 Dateiname : 2D-Barcodes.c
 Programm  : Ein Programm, das über ein Menü entweder eine Zeichenkette einliest und in 
             einen 2D-Barcode kodiert oder einen Barcode aus einer Datei dekodiert.
 Eingaben  : Zeichenketten (über stdin) oder Barcode-Dateien, je nach Modus
 Ausgaben  : - Eine Textdatei ("barcode_output.txt") mit dem Barcode (Bitmuster)
             - Konsolenausgabe des dekodierten Ergebnisses und ggf. Warnmeldungen

 Autor     : Linke Dominik 11371232
 Version   : 04   vom   2025.03.11
 */

 #include <stdio.h>      
 #include <stdlib.h>     
 #include <string.h>     
 #include <math.h>       
 #include <stdint.h>     
 #include <ctype.h>      
 
 /*
  * Funktion:  berechneQuersumme
  * Zweck:     Berechnet die Summe aller Bytes in einem Datenarray und gibt nur 
  *            die unteren 16 Bits dieser Summe als Quersumme zurück.
  * Parameter:
  *   - data:   Zeiger auf das Datenarray
  *   - laenge: Anzahl der zu verarbeitenden Bytes im Array
  * Rückgabewert:
  *   - 16-Bit-Quersumme (unsigned short) aus allen aufsummierten Werten
  */
 static uint16_t berechneQuersumme(const unsigned char *data, size_t laenge) {
     uint32_t sum = 0;              // Variable für die aufsummierten Werte (32-Bit zum Aufaddieren)
     for (size_t i = 0; i < laenge; i++) {
         sum += data[i];            // Jedes Byte zur Summe addieren
     }
     return (uint16_t)(sum & 0xFFFF); // Nur die unteren 16 Bit als Rückgabewert
 }
 
 /*
  * Funktion:  MinSeitenLaenge
  * Zweck:     Ermittelt die kleinste Seitenlänge N eines quadratischen Barcodes,
  *            sodass N*N mindestens so viele Bits aufnehmen kann wie benötigt.
  * Parameter:
  *   - bitCount: Anzahl der zu speichernden Bits
  * Rückgabewert:
  *   - Kleinste Seitenlänge N, damit N*N >= bitCount
  */
 static int MinSeitenLaenge(int bitCount) {
     int N = (int)ceil(sqrt((double)bitCount));
     return N;
 }
 
 /*
  * Funktion:  errorMessage
  * Zweck:     Gibt eine Fehlermeldung auf STDERR aus, versehen mit dem Präfix "[Fehler]".
  * Parameter:
  *   - msg: Textnachricht, die als Fehlermeldung ausgegeben wird
  * Rückgabewert:
  *   - Kein Rückgabewert (void)
  */
 static void errorMessage(const char *msg) {
     fprintf(stderr, "[Fehler] %s\n", msg);
 }
 
 /*
 * Funktion:  encode
 * Zweck:     Kodiert eine Zeichenkette in einen binären Barcode (Bitfeld) und
 *            gibt die Seitenlänge des resultierenden Quadrats zurück.
 *            Das Längenbyte wird dabei speziell so abgelegt, dass es in Zeile 1
 *            (den ersten 8 Spalten der ersten Zeile) steht.
 * Parameter:
 *   - input:       Zeiger auf den Eingabe-String (max. 255 Zeichen)
 *   - barcodeBits: Zeiger auf einen Zeiger, in den der dynamisch allokierte 
 *                  Speicher für den Barcode eingetragen wird
 *   - size:        Zeiger auf einen int, in den die berechnete Seitenlänge (N) 
 *                  zurückgeschrieben wird
 * Rückgabewert:
 *   - Kein Rückgabewert (void). Die Ausgabe erfolgt über barcodeBits und size.
 */
void encode(const char *input, unsigned char **barcodeBits, int *size)
{
    // Bestimme die Länge des Eingabestrings und speichere sie in inputLen
    size_t inputLen = strlen(input);
    if (inputLen > 255) {
        fprintf(stderr, "[Warnung] Eingabe laenger als 255 Zeichen. Abschneiden.\n");  // Falls die Eingabe länger als 255 Zeichen ist wird ne warnung ausgegeben
        inputLen = 255;
    }
    if (inputLen < 1) {  // Falls die Eingabe leer ist kommt auch ne warn meldung 
        errorMessage("Eingabe darf nicht leer sein.");
        return;
    }

    unsigned char datenPuffer[260];  // Puffer :  1 Byte Länge + 255 Zeichen + 2 Endbytes + 2 Checksumme-Bytes
    size_t pos = 0;
    datenPuffer[pos++] = (unsigned char)(inputLen & 0xFF);  // Schreibe das Längenbyte in den Puffer
    memcpy(&datenPuffer[pos], input, inputLen);  // Kopiere die Eingabezeichenkette in den Puffer
    pos += inputLen; // erhöht den zähler um die anzahl der zeichen
    datenPuffer[pos++] = 0x00;  // Endekennung: 0x00, 0x00
    datenPuffer[pos++] = 0x00;
    uint16_t checkSumme = berechneQuersumme(datenPuffer, pos); // Berechne die Checksumme der bisher gespeicherten Bytes (bis zu den Endekennungsbytes)
    datenPuffer[pos++] = (unsigned char)((checkSumme >> 8) & 0xFF); // fügt MSB an
    datenPuffer[pos++] = (unsigned char)(checkSumme & 0xFF); // fügt LSB an

    size_t dataLen = pos; // speichert die aktuelle länge 
    int totalBits = (int)(dataLen * 8); // gesamt länge
    int N = MinSeitenLaenge(totalBits); // min seiten länge
    int KapazitaetBits = N * N;
    int fillPos = 0; 
    while ((int)(dataLen * 8) < KapazitaetBits) {   // Solange die Anzahl der Bits im datenPuffer kleiner als die Kapazität des Barcodes ist:
        if (fillPos == 0) {
            datenPuffer[dataLen++] = 0xEC;
            fillPos = 1;
        } else {
            datenPuffer[dataLen++] = 0x11;
            fillPos = 0;
        }
    }
    int finalBits = (int)(dataLen * 8); // Berechne die finale Bitanzahl im datenPuffer
    if (finalBits > KapazitaetBits) { // Falls mehr Bits vorhanden sind als exakt in das N x N Raster passen:
        int excess = finalBits - KapazitaetBits;
        int NibbleRest = excess % 4; // berechnet wie viele Nibble dazu müssen bzw entfernt werden in den nächsten zeilen
        int NibblesZuLoeschen = (excess / 4) + (NibbleRest > 0 ? 1 : 0);
        int bitsZuLoeschen = NibblesZuLoeschen * 4;
        int FinaleBits = finalBits - bitsZuLoeschen;
        int newDataLen = FinaleBits / 8;
        int RestBits = FinaleBits % 8;
        dataLen = newDataLen;
        if (RestBits > 0) { // Falls Restbits vorhanden sind:
            unsigned char lastByte = datenPuffer[newDataLen];
            unsigned char mask = (unsigned char)(0xFF << (8 - RestBits));
            lastByte &= mask;
            datenPuffer[newDataLen] = lastByte;
            dataLen = newDataLen + 1;
        }
    }

    
    *barcodeBits = (unsigned char*)calloc(N * N, sizeof(unsigned char)); // legt speicer für den barcode fest basirend auf der N*N "matrix"
    if (!(*barcodeBits)) {  // Überprüft ob die Speicherallokierung erfolgreich war
        errorMessage("Speicherallokierung fehlgeschlagen"); // wenns nicht geklappt hat kommt eine fehler meldung 
        return;
    }

    int bitIndex = 0;  // Laufender Zähler für die zu besetzenden Bits

    {
        unsigned char lengthByte = datenPuffer[0];
        for (int b = 0; b < 8; b++) {
            // b=0 entspricht dem MSB, b=7 dem LSB
            (*barcodeBits)[0 * N + b] = (lengthByte >> (7 - b)) & 0x01;
        }
        bitIndex = 8;  // 8 Bits der ersten Zeile sind bereits belegt
    }

 
    for (int byteIdx = 1; byteIdx < (int)dataLen; byteIdx++) { // schreibt die restlichen bits ins barcode feld  aus dem datapuffer
        unsigned char currentByte = datenPuffer[byteIdx];
        for (int b = 0; b < 8; b++) {
            if (bitIndex >= N * N) break;
            unsigned char bit = (currentByte >> b) & 0x01;
            int row = bitIndex / N;
            int col = bitIndex % N;
            if (row == 0) {
                /* Für den Rest der 1. Zeile (nach Spalte 7) füllen wir in natürlicher Reihenfolge,
                   da die ersten 8 Spalten bereits belegt sind. */
                (*barcodeBits)[row * N + col] = bit;
            } else {
                /* Für alle Zeilen ab Zeile 2: Die Barcodezeilen werden von rechts nach links interpretiert.
                   Das heißt, wir legen das Bit in Spalte (N-1 - (bitIndex mod N)) ab. */
                int realCol = N - 1 - (bitIndex % N);
                (*barcodeBits)[row * N + realCol] = bit;
            }
            bitIndex++;
        }
    }

    *size = N;
}


 /*
  * Funktion:  schreibeBarcode
  * Zweck:     Schreibt das N*N-Bitfeld als Barcode (bestehend aus '0' und '1') 
  *            in eine Textdatei, jeweils eine Zeile pro Barcodezeile.
  * Parameter:
  *   - barcodeBits: Zeiger auf das N*N-Bitfeld
  *   - N:           Seitenlänge des Barcodes (Anzahl Zeilen/Spalten)
  *   - filename:    Ziel-Dateiname, in den der Barcode geschrieben wird
  * Rückgabewert:
  *   - Kein Rückgabewert (void)
  */
 void schreibeBarcode(const unsigned char *barcodeBits, int N, const char *filename)
 {
     FILE *fp = fopen(filename, "w");
     if (!fp) {
         errorMessage("Konnte Ausgabedatei nicht oeffnen");
         return;
     }
     for (int row = 0; row < N; row++) {
         for (int col = 0; col < N; col++) {
             fprintf(fp, "%d", barcodeBits[row * N + col]);
         }
         fprintf(fp, "\n");
     }
     fclose(fp);
 }
 
 /*
  * Funktion:  readBarcode
  * Zweck:     Liest einen Barcode aus einer Textdatei, in der jede Zeile 
  *            '0' oder '1' enthält. Die gelesenen Zeilen werden in ein 
  *            N*N-Bitfeld umgewandelt und zurückgegeben.
  * Parameter:
  *   - filename: Pfad/Name der zu lesenden Textdatei
  *   - size:     Zeiger auf einen int, in dem die ermittelte Seitenlänge N 
  *               zurückgegeben wird
  * Rückgabewert:
  *   - Ein Pointer auf das dynamisch allokierte N*N-Bitfeld 
  *     (oder NULL bei Fehlern). Der Aufrufer muss diesen Speicher per free() freigeben.
  */
 unsigned char* readBarcode(const char *filename, int *size)
 {
     FILE *fp = fopen(filename, "r");
     if (!fp) {
         errorMessage("Konnte Eingabedatei nicht oeffnen");
         return NULL;
     }
 
     char **lines = NULL;
     int maxLineLen = 0;
     int lineCount = 0;
     {
         char buffer[1024];
         while (fgets(buffer, sizeof(buffer), fp)) {
             size_t len = strlen(buffer);
             if (len > 0 && buffer[len - 1] == '\n') {
                 buffer[len - 1] = '\0';
                 len--;
             }
             if ((int)len > maxLineLen) {
                 maxLineLen = (int)len;
             }
             lines = (char**)realloc(lines, sizeof(char*) * (lineCount + 1));
             lines[lineCount] = (char*)malloc(len + 1);
             strcpy(lines[lineCount], buffer);
             lineCount++;
         }
     }
     fclose(fp);
 
     if (lineCount != maxLineLen) {
         fprintf(stderr, "[Warnung] Eingelesener Barcode ist nicht quadratisch (Zeilen=%d, Spalten=%d)\n",
                 lineCount, maxLineLen);
     }
     
     int N = (lineCount < maxLineLen) ? lineCount : maxLineLen;
     unsigned char *barcodeBits = (unsigned char*)calloc(N * N, sizeof(unsigned char));
     if (!barcodeBits) {
         errorMessage("Speicherallokierung fehlgeschlagen (readBarcode)");
         for (int i = 0; i < lineCount; i++) {
             free(lines[i]);
         }
         free(lines);
         return NULL;
     }
 
     for (int row = 0; row < N; row++) {
         int len = (int)strlen(lines[row]);
         for (int col = 0; col < N; col++) {
             if (col < len) {
                 barcodeBits[row * N + col] = (lines[row][col] == '1') ? 1 : 0;
             } else {
                 barcodeBits[row * N + col] = 0;
             }
         }
     }
     
     for (int i = 0; i < lineCount; i++) {
         free(lines[i]); 
     }
     free(lines);
     
     *size = N;
     return barcodeBits;
 }
 
/*
 * Funktion:  decode
 * Zweck:     Dekodiert ein N*N-Barcode-Bitfeld in seine ursprünglichen 
 *            Nutzdaten (Zeichenkette, Endekennung, Checksummenprüfung). 
 *            Dabei wird die Länge ausschließlich anhand des Längenbytes bestimmt.
 * Parameter:
 *   - barcodeBits: Zeiger auf das N*N-Bitfeld (Barcode)
 *   - N:           Seitenlänge des Barcodes
 * Rückgabewert:
 *   - Kein Rückgabewert (void). Alle Ergebnisse (Zeichenkette, Warnungen) 
 *     werden auf der Konsole ausgegeben.
 */

 void decode(const unsigned char *barcodeBits, int N)
{
    unsigned char datenPuffer[260]; // puffer für dekodirung 
    int datenZaehler = 0; // zähler variable für dekodirung 
    int totalBits = N * N;
    uint16_t LeseCheckSumme = 0; 
    uint16_t calc = 0;
    
    if (N < 8) { //Lese das Längenbyte (erste 8 Zellen der ersten Zeile) in MSB-first Reihenfolge
        printf("Barcode hat nicht genügend Spalten in der ersten Zeile.\n");
        return;
    }
    unsigned char lengthAcc = 0; // Da in der ersten Zeile die ersten 8 Spalten die Bits enthalten, entspricht die Position einfach: row = 0, col = 0 .. 7.
    for (int col = 0; col < 8; col++) {
        unsigned char bit = barcodeBits[0 * N + col] & 0x01;
        lengthAcc |= (bit << (7 - col));
    }
    datenPuffer[datenZaehler++] = lengthAcc;

    
    int bitIndex = 8; //Lese die restlichen Bytes (beginnend ab Bit-Index 8) in LSB-first Reihenfolge
    unsigned char aktuelleByt = 0;
    int bitsCollected = 0;
    while (bitIndex < totalBits) { // Solange noch Bits im Barcode vorhanden sind:
        int row = bitIndex / N; // berechnet die aktuelle zeile anhand von bitIndex
        int col = bitIndex % N;
        int realCol;

      
        if (row == 0)  // In der ersten Zeile werden die Bits normal gelesen
            realCol = col;
        else // Ab der zweiten Zeile werden die Bits in umgekehrter Reihenfolge gelesen
            realCol = N - 1 - col;

        unsigned char bit = barcodeBits[row * N + realCol] & 0x01; // ließt die bits in der ersten possition ein und stellt sicher das es 0 oder 1 ist 
        aktuelleByt |= (bit << bitsCollected);  // LSB-first Anordnung
        bitsCollected++; // erhöht den bit zähler
        if (bitsCollected == 8) {
            datenPuffer[datenZaehler++] = aktuelleByt;
            aktuelleByt = 0;
            bitsCollected = 0;
        }
        bitIndex++;
        if (datenZaehler >= (int)sizeof(datenPuffer))
            break;
    }
   
    if (datenZaehler < 4) {  // mindestens 1 Byte Länge, Nutzdaten, 2 Byte Endekennung, 2 Byte Checksumme
        printf("Zu wenige Daten, um Dekodierung durchzufuehren.\n");
        return;
    }

    unsigned char laenge = datenPuffer[0];  // Länge, wie im Längenbyte angegeben
    int effectiveLength = laenge;             // Wir verwenden ausschließlich das Längenbyte

    //Überprüfe, ob an der erwarteten Stelle die Endekennung (0x00, 0x00) vorliegt
    int expectedTerminationIndex = 1 + laenge;  // 1 Byte Länge + laenge Bytes Nutzdaten
    if (expectedTerminationIndex + 1 < datenZaehler) {
        if (datenPuffer[expectedTerminationIndex] != 0x00 ||
            datenPuffer[expectedTerminationIndex + 1] != 0x00) {
            printf("[Warnung] Die Endekennung fehlt oder ist fehlerhaft.\n");
        }
    } else {
        printf("[Warnung] Nicht genuegend Daten fuer die Endekennung vorhanden.\n");
    }

    printf("Dekodierte Laenge: %d\n", effectiveLength);
    
    //Kopiere die Nutzdaten (Zeichenkette) in decodedString
    unsigned char decodedString[256];
    memset(decodedString, 0, sizeof(decodedString));
    for (int i = 0; i < effectiveLength && i < 255; i++) {
        decodedString[i] = datenPuffer[1 + i];
    }
    decodedString[effectiveLength] = '\0';

    //Checksumme ermitteln
    size_t benoetigt = 1 + laenge + 2 + 2; // 1 Byte Länge, laenge Bytes Nutzdaten, 2 Byte Endekennung, 2 Byte Checksumme
    if (benoetigt > (size_t)datenZaehler)
        benoetigt = datenZaehler;
    
    
    if (benoetigt >= 2) { // Falls genügend Bytes für die Checksumme vorhanden sind werden die aus den letzen beiden bytes eingelesen 
        LeseCheckSumme = (datenPuffer[benoetigt - 2] << 8) | (datenPuffer[benoetigt - 1]);
    }
    
    if (benoetigt - 2 > 0) {     // Berechnet die Checksumme über alle Bytes im datenPuffer, ausgenommen die letzten 2 Bytes (Checksumme selbst)
        calc = berechneQuersumme(datenPuffer, benoetigt - 2);
    }

    printf("Dekodierte Zeichenkette: %s\n", decodedString); // Gibt die dekodierte Zeichenkette auf der Konsole aus

    if (calc != LeseCheckSumme) {  // Vergleicht die berechnete Checksumme mit der ausgelesenen Checksumme
        printf("[Warnung] Quersumme stimmt nicht ueberein! Die Zeichenkette kann fehlerhaft sein.\n");
        printf("  Erwartet (berechnet): 0x%04X, Gelesen: 0x%04X\n", calc, LeseCheckSumme);
    }
}

 

 /*
  * Funktion:  main
  * Zweck:     Hauptfunktion des Programms. Bietet ein Menü zur Auswahl zwischen
  *            Kodierung (Erstellung eines 2D-Barcodes aus einer Zeichenkette) und 
  *            Dekodierung (Auslesen eines 2D-Barcodes aus einer Datei).
  * Parameter:
  *   - Keine Parameter (Standard-Signatur)
  * Rückgabewert:
  *   - int: Gibt 0 bei regulärem Programmablauf zurück, 1 bei Fehler.
  */
 int main(void)
 {
     int option;
     while (1) {
         printf("\nBitte waehlen Sie eine Option:\n");
         printf("1: Kodierung (2D-Barcode erstellen)\n");
         printf("2: Dekodierung (2D-Barcode aus Datei lesen)\n");
         printf("3: Programm beenden\n");
         printf("Ihre Auswahl: ");
         if (scanf("%d", &option) != 1) {
             errorMessage("Ungueltige Eingabe.");
             while (getchar() != '\n');  // Eingabepuffer leeren
             continue;
         }
         while (getchar() != '\n'); // Restliche Eingabezeile entfernen
 
         if (option == 1) {
             // Kodierung
             char eingabe[1024];
             printf("Bitte eine Zeichenkette (1..255 Zeichen) eingeben: ");
             if (!fgets(eingabe, sizeof(eingabe), stdin)) {
                 errorMessage("Fehler beim Lesen der Eingabe");
                 continue;
             }
             size_t len = strlen(eingabe);
             if (len > 0 && eingabe[len - 1] == '\n')
                 eingabe[len - 1] = '\0';
 
             unsigned char *barcode = NULL;
             int size = 0;
             encode(eingabe, &barcode, &size);
             if (!barcode || size == 0) {
                 errorMessage("Barcode konnte nicht erstellt werden.");
                 continue;
             }
 
             const char *filename = "barcode_output.txt";
             schreibeBarcode(barcode, size, filename);
             free(barcode);
             printf("Barcode wurde in '%s' geschrieben.\n", filename);
         }
         else if (option == 2) {
             // Dekodierung
             const char *filename = "barcode_output.txt";
             int leseGroesse = 0;
             unsigned char *leseBits = readBarcode(filename, &leseGroesse);
             if (!leseBits || leseGroesse == 0) {
                 errorMessage("Barcode konnte nicht gelesen werden.");
                 continue;
             }
             printf("\n--- Dekodierung ---\n");
             decode(leseBits, leseGroesse);
             free(leseBits);
         }
         else if (option == 3) {
             printf("Programm wird beendet.\n");
             break;
         }
         else {
             printf("Ungueltige Auswahl. Bitte waehlen Sie 1, 2 oder 3.\n");
         }
     }
     return 0;
 }
 