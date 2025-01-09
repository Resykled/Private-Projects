#include <stdio.h>

int main(void)
{
 while(1){
    short firstNumber, secondNumber;
    int opCode;

    // Einlesen der beiden short-Variablen
    printf("Geben Sie die erste Zahl (short) ein: ");
    scanf("%hd", &firstNumber);

    printf("Geben Sie die zweite Zahl (short) ein: ");
    scanf("%hd", &secondNumber);

    // Einlesen des opCodes
    printf("Geben Sie den opCode (1 +, 2 -, 3 *, 4 /) ein: ");
    scanf("%d", &opCode);

    // Verarbeitung basierend auf dem opCode
    switch (opCode)
    {
        case 1:
        {
            // 1  Berechne firstNumber + secondNumber
            short sum = firstNumber + secondNumber;-
            printf("Ergebnis (Addition): %hd\n", sum);
            break;
        }

        case 2:
        {
            // 2  Berechne firstNumber - secondNumber, falls firstNumber > secondNumber
            //      sonst secondNumber - firstNumber
            short diff;
            if (firstNumber > secondNumber) {
                diff = firstNumber - secondNumber;
            } else {
                diff = secondNumber - firstNumber;
            }
            printf("Ergebnis (Differenz): %hd\n", diff);
            break;
        }

        case 3:
        {
            // 3  Berechne firstNumber * secondNumber
            short produkt = firstNumber * secondNumber;
            printf("Ergebnis (Multiplikation): %hd\n", produkt);
            break;
        }

        case 4:
        {
            // 4  Berechne Ganzzahldivision und Modulo
            // Achtung: Division durch 0 prüfen
            if (secondNumber == 0) {
                printf("Fehler: Division durch 0 nicht möglich!\n");
            } else {
                short quotient = firstNumber / secondNumber;
                short rest     = firstNumber % secondNumber;
                printf("Ergebnis (Division): %hd\n", quotient);
                printf("Ergebnis (Modulo): %hd\n", rest);
            }
            break;
        }

        default:
        {
            // Ungültiger opCode
            printf("Ungueltiger opCode: %d\n", opCode);
            break;
        }
    }
 }
    return 0;
}
