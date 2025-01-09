#include <stdio.h>   // Für printf, scanf
#include <math.h>    // Für sqrt, atan2, M_PI

int main(void)
{
    double x, y;
    
    // Eingabe der x- und y-Koordinate
    printf("Geben Sie die x-Koordinate ein: ");
    scanf("%lf", &x);

    printf("Geben Sie die y-Koordinate ein: ");
    scanf("%lf", &y);

    // Länge des Vektors berechnen: Länge = sqrt(x^2 + y^2)
    double laenge = sqrt(x * x + y * y);

    // Winkel (in Bogenmaß) mit atan2 berechnen
    // atan2(y, x) liefert den Winkel in Radianten zwischen -π und +π
    double winkel_radiant = atan2(y, x);

    // Optional: Winkel in Grad umrechnen
    // Möchte man den Winkel in Grad ausgeben, multipliziert man mit 180 / π.
    double winkel_grad = winkel_radiant * (180.0 / M_PI);

    // Ausgabe mit drei Nachkommastellen
    printf("Laenge des Vektors: %.3f\n", laenge);
    printf("Winkel zur x-Achse (in Grad): %.3f\n", winkel_grad);

    return 0;
}
