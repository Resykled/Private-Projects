#include <stdio.h>

int main(void)
{
 while(1){
    int dezimal;
    
    printf("Geben Sie eine Zahl zwischen 0 und 255 ein: ");
    scanf("%d", &dezimal);

    // Falls der Eingabewert außerhalb des gewünschten Bereichs liegt,
    // kann man hier entsprechend reagieren:
    if (dezimal < 0 || dezimal > 255) {
        printf("Die eingegebene Zahl liegt nicht zwischen 0 und 255.\n");
        return 1; // Abbruch mit Fehlercode
    }

    // Spezialfall: 0
    if (dezimal == 0) {
        printf("0\n");
        return 0;
    }

    // Solange die Zahl > 0 ist:
    // - Bestimme das aktuelle Bit (LSB), indem man den Wert % 2 nimmt
    // - Gib es aus
    // - Teile die Zahl durch 2 (Ganzzahldivision), um das nächste Bit zu bestimmen
    while (dezimal > 0) {
        int bit = dezimal % 2;
        printf("%d", bit);
        dezimal /= 2;
    }

    // Zeilenumbruch am Ende
    printf("\n");
   
 }
    return 0;
}
