#include <stdio.h>
#include <stdlib.h> // für abs() oder llabs(), falls nötig

int main(void)
{
 while(1){
    long long number;  // Verwende long long, um auch größere Zahlen verarbeiten zu können
    long long temp;
    int sum = 0;

    // Benutzer auffordern, eine Zahl einzugeben
    printf("Geben Sie eine n-stellige Zahl ein: ");
    scanf("%lld", &number);

    // Falls die Zahl negativ ist, betrachten wir ihren Betrag
    // (Quersumme ist üblicherweise eine nicht-negative Zahl).
    temp = (number < 0) ? -number : number;

    // Solange temp > 0 ist, werden die Ziffern sukzessive abgetrennt.
    while (temp > 0)
    {
        sum += temp % 10; // letzte Ziffer addieren
        temp /= 10;       // letzte Ziffer entfernen
    }

    // Ergebnis ausgeben
    printf("Die Quersumme von %lld ist: %d\n", number, sum);
 }
    return 0;
}
