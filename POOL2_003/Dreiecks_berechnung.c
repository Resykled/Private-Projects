/*
 Dateiname : Dreiecks_berechnung.c
 Programm  : Berechnet alle werte eines 3 dimensionales dreiecks, transformiert es in 2D und zurück. Seitenlänge
Umfang
Flächeninhalt
Innenwinkel ( alle 3 )
Schwerpunk
Umkreismittelpunkt
 
Eingaben  :dreiecks punkte in dem fenster, wird überoprüft ob das dreieck real ist 
 Ausgaben  : Seitenlänge
Umfang
Flächeninhalt
Innenwinkel ( alle 3 )
Schwerpunk
Umkreismittelpunkt

 Autor     : Linke Dominik 11371232
 Version   : 02   vom   2025.02.19
 */


#include <stdio.h> //  standart C funtionen
#include <stdlib.h> // standart C funktionen
#include <math.h> // mathematische funktionen , in diesem fall acos sqrt etc 

#define EPS 1e-12 // kleine tolleranz um rechenfehler "aufzuhalten", wenn ein wertt kleiner als EPS ist wird er als 0 gesehen


/*
 * Struktur:  Vector3
 * Zweck:     Repräsentiert einen Vektor oder Punkt im dreidimensionalen Raum mit Koordinaten (x, y, z).
 */
typedef struct { // struktur zur dahrstellung eines 3D vektors
    double x;
    double y;
    double z;
} Vector3; 

/*
 * Funktion:  vadd
 * Zweck:     Vektoraddition: Addiert zwei 3D-Vektoren a und b komponentenweise.
 * Parameter:
 *   - a, b: Zwei Vektoren, die addiert werden sollen
 * Rückgabewert:
 *   - Ein Vector3, dessen x-, y- und z-Komponente den Summen der jeweiligen 
 *     Komponenten von a und b entsprechen
 */
Vector3 vadd(Vector3 a, Vector3 b) 
{
    Vector3 r = {0.0, 0.0, 0.0};
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
/*
 * Funktion:  vsub
 * Zweck:     Vektorsubtraktion: Subtrahiert b von a, also a - b.
 * Parameter:
 *   - a, b: Zwei Vektoren
 * Rückgabewert:
 *   - Ein Vector3, dessen Komponenten (a.x - b.x, a.y - b.y, a.z - b.z) sind
 */
Vector3 vsub(Vector3 a, Vector3 b) 
{
    Vector3 r = {0.0, 0.0, 0.0};
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}
/*
 * Funktion:  vscale
 * Zweck:     Skaliert einen Vektor um den Faktor s.
 * Parameter:
 *   - v: Ein Vector3, der skaliert werden soll
 *   - s: Skalierungsfaktor (double)
 * Rückgabewert:
 *   - Ein neuer Vector3, dessen Komponenten mit s multipliziert wurden
 */
Vector3 vscale(Vector3 v, double s) 
{
    Vector3 r = {0.0, 0.0, 0.0};
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}
/*
 * Funktion:  vdot
 * Zweck:     Berechnet das Skalarprodukt (Punktprodukt) zweier Vektoren a und b.
 * Parameter:
 *   - a, b: Zwei Vector3-Objekte
 * Rückgabewert:
 *   - Das Skalarprodukt a.x*b.x + a.y*b.y + a.z*b.z als double
 */
double vdot(Vector3 a, Vector3 b) 
{
    double result = 0.0;
    result = a.x*b.x + a.y*b.y + a.z*b.z;
    return result;
}
/*
 * Funktion:  vcross
 * Zweck:     Berechnet das Kreuzprodukt zweier Vektoren a und b.
 * Parameter:
 *   - a, b: Zwei Vector3-Objekte
 * Rückgabewert:
 *   - Ein Vector3, der das Ergebnis des Kreuzprodukts darstellt
 *     (x = a.y*b.z - a.z*b.y, etc.)
 */
Vector3 vcross(Vector3 a, Vector3 b) 
{
    Vector3 r = {0.0, 0.0, 0.0};
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
/*
 * Funktion:  vlength
 * Zweck:     Berechnet die Länge (den Betrag) eines Vektors v im dreidimensionalen Raum.
 * Parameter:
 *   - v: Der Vektor, dessen Länge benötigt wird
 * Rückgabewert:
 *   - Die Länge, also sqrt(v.x^2 + v.y^2 + v.z^2)
 */
 
 double vlength(Vector3 v)  // läng eines vektors, sqrt = square root oder quadrat wurzel
{
    double length = 0.0;
    length = sqrt(vdot(v, v));
    return length;
}
    
/*
 * Funktion:  toDeg
 * Zweck:     Wandelt einen Winkel von Bogenmaß (radians) in Grad um.
 * Parameter:
 *   - rad: Winkel im Bogenmaß (double)
 * Rückgabewert:
 *   - Entsprechender Winkel in Grad (double)
 * Hinweise:
 *   - Das Programm verwendet hier die Konstante PI = 3.141592653589793
 */
double toDeg(double rad) // wert von rad zu grad umrechnen
{
    double degrees = 0.0;
    degrees = rad * 180.0 / 3.141592653589793;//pi mit 12 stellen da die funktion von Math.h nicht funktioniert hat 
    return degrees;
}
/*
 * Funktion:  readPoint
 * Zweck:     Liest drei Koordinatenwerte (x, y, z) für einen Vektor/Punkt vom Nutzer ein.
 * Parameter:
 *   - label: Ein Buchstabe, der den Punkt bezeichnet (z. B. 'A', 'B' oder 'C')
 * Rückgabewert:
 *   - Ein Vector3 mit den eingegebenen x-, y-, z-Werten
 */
Vector3 readPoint(char label)  // ließt die länge ab
{
    Vector3 p = {0.0, 0.0, 0.0};
    printf("Koordinaten von %c (x y z): ", label);
    scanf("%lf %lf %lf", &p.x, &p.y, &p.z);
    return p;
}
/*
 * Funktion:  checkTriangle
 * Zweck:     Prüft, ob die drei Punkte A, B, C im Raum ein gültiges, nicht-degeneriertes Dreieck bilden.
 * Parameter:
 *   - A, B, C: Punkte im dreidimensionalen Raum
 * Rückgabewert:
 *   - 1 (true), wenn ein gültiges Dreieck vorliegt
 *   - 0 (false), wenn das "Dreieck" ungültig ist 
 *      (z.B. wenn zwei Punkte identisch sind oder alle drei auf einer Linie liegen)
 */
int checkTriangle(Vector3 A, Vector3 B, Vector3 C) {   // checkt ob die punkte ein dreieck bilden oder ob random punkte gegeben wurden die nich klappen
    Vector3 AB = {0.0, 0.0, 0.0};
    Vector3 AC = {0.0, 0.0, 0.0};
    AB = vsub(B, A);
    AC = vsub(C, A);
    if (vlength(AB) < EPS || vlength(AC) < EPS || vlength(vsub(C, B)) < EPS) { // prüfe ob die punkte extrem klein sind , mit EPS was am anfang erwähnt wurde 
        return 0;
    }
    Vector3 crossABAC = {0.0, 0.0, 0.0};
    crossABAC = vcross(AB, AC);
    // guckt ob sich die punkte schneiden  
    if (vlength(crossABAC) < EPS) {
        return 0;
    }
    return 1;
}
/*
 * Funktion:  triangleSideLengths
 * Zweck:     Berechnet die drei Seitenlängen (AB, BC, CA) eines Dreiecks, 
 *            definiert durch die Punkte A, B, C.
 * Parameter:
 *   - A, B, C:           Punkte des Dreiecks
 *   - AB, BC, CA:        Pointer auf double-Variablen, in die die Seitenlängen eingetragen werden
 * Rückgabewert:
 *   - keiner (void); die Resultate werden über die Zeiger in AB, BC, CA geschrieben
 */
void triangleSideLengths(Vector3 A, Vector3 B, Vector3 C, 
                         double* AB, double* BC, double* CA) { // länge der vektoren
    *AB = vlength(vsub(B, A)); // Länge AB
    *BC = vlength(vsub(C, B)); // Länge BC 
    *CA = vlength(vsub(A, C)); // Länge CA
}


/*
 * Funktion:  triangleArea
 * Zweck:     Berechnet die Fläche (den Flächeninhalt) eines Dreiecks im 3D-Raum.
 * Parameter:
 *   - A, B, C: Punkte des Dreiecks
 * Rückgabewert:
 *   - Flächeninhalt des Dreiecks als double
 * Formel:
 *   - (1/2) * |AB × AC|
 *     -> Kreuzprodukt AB×AC liefert das Parallelogramm, die Dreiecksfläche ist die Hälfte davon.
 */
double triangleArea(Vector3 A, Vector3 B, Vector3 C) { // flächeninhalt des vektors 
    Vector3 AB = {0.0, 0.0, 0.0};
    Vector3 AC = {0.0, 0.0, 0.0};
    AB = vsub(B, A);
    AC = vsub(C, A);
    // kreuzprodukt AB x AC 
    Vector3 crossABAC = {0.0, 0.0, 0.0};
    crossABAC = vcross(AB, AC);
    return 0.5 * vlength(crossABAC); // 0.5 fache länge 
}
/*
 * Funktion:  trianglePerimeter
 * Zweck:     Berechnet den Umfang eines Dreiecks anhand seiner Seitenlängen AB, BC, CA.
 * Parameter:
 *   - AB, BC, CA: Seitenlängen
 * Rückgabewert:
 *   - Umfang (summe aller Seitenlängen) als double
 */
double trianglePerimeter(double AB, double BC, double CA) { // umfang des dreiecks berechnen mit den drei seitenlängen
    {
    double perimeter = 0.0;
    perimeter = AB + BC + CA;
    return perimeter;
}
}
/*
 * Funktion:  triangleAngles
 * Zweck:     Ermittelt die drei Innenwinkel eines Dreiecks in Bogenmaß, 
 *            je nachdem, welcher Punkt als "Scheitel" gewählt wird.
 * Parameter:
 *   - A, B, C: Punkte des Dreiecks
 *   - phiA, phiB, phiC: Pointer, die die Winkel an den Ecken A, B, C speichern
 * Methode:
 *   - Winkel an A: Zwischen den Vektoren BA und CA (B->A, C->A)
 *   - Winkel an B: Zwischen den Vektoren AB und CB
 *   - Winkel an C: Zwischen den Vektoren AC und BC
 */
void triangleAngles(Vector3 A, Vector3 B, Vector3 C,    // berechnung der innenwinkel phiA phiB phiC an den ecken bzw punkten A B C 
                    double *phiA, double *phiB, double *phiC) {
   // winkel an A BA = A-B, CA A-C
   Vector3 BA = {0.0, 0.0, 0.0};
    Vector3 CA = {0.0, 0.0, 0.0};
    BA = vsub(A, B);
    CA = vsub(A, C);
    // skalarprodukt BA*CA
    double lenBA = vlength(BA);
    //cos(phiA)
    double lenCA = vlength(CA);
    // winkel in RAD 
    *phiA = acos(vdot(BA, CA) / (lenBA * lenCA));
    // Winkel an B: AB = B - A, CB = B - C 
    Vector3 AB = {0.0, 0.0, 0.0};
    Vector3 CB = {0.0, 0.0, 0.0};
    AB = vsub(B, A);
    CB = vsub(B, C);
    // skalarprodukt BA*BC
    double lenAB = vlength(AB);
    double lenCB = vlength(CB);
    *phiB = acos(vdot(AB, CB) / (lenAB * lenCB));
     // Winkel an C: AC = C - A, BC = C - B
     Vector3 AC = {0.0, 0.0, 0.0};
     Vector3 BC = {0.0, 0.0, 0.0};
     AC = vsub(C, A);
     BC = vsub(C, B);
     // skalarprodukt CA*CB
    double lenAC = vlength(AC);
    double lenBC = vlength(BC);
    *phiC = acos(vdot(AC, BC) / (lenAC * lenBC));
}
/*
 * Funktion: incenterAndRadius
 * Zweck:     Berechnet den Inkreis (Innenkreis) eines Dreiecks.
 *            Dazu gehören der Inkreismittelpunkt MI und der Inkreisradius rI.
 * Parameter:
 *   - A, B, C:   Eckpunkte des Dreiecks
 *   - AB, BC, CA: Seitenlängen
 *   - area:       Flächeninhalt des Dreiecks
 *   - MI:         Pointer auf Vector3, in den der Inkreismittelpunkt geschrieben wird
 *   - rI:         Pointer auf double, in das der Inkreisradius gespeichert wird
 * Formel:
 *   - MI = (a*A + b*B + c*C) / (a + b + c), 
 *     wobei a=BC, b=CA, c=AB
 *   - rI = (2*Fläche) / (Umfang)
 */
void incenterAndRadius(Vector3 A, Vector3 B, Vector3 C, 
                       double AB, double BC, double CA,
                       double area,
                       Vector3 *MI, double *rI) { // innenkreis , mittelpunkt M_I und Radius RI

    //Seite a = |BC|, b = |AC|, c = |AB|
    // //M_I = (a*A + b*B + c*C)/(a+b+c)
    //rI  = 2*Fläche / (a+b+c)
    double a = BC;
    double b = CA;
    double c = AB;
    // umfang
    double perimeter = a + b + c;
    // inkreismittelpunkt
    MI->x = (a * A.x + b * B.x + c * C.x) / perimeter;
    MI->y = (a * A.y + b * B.y + c * C.y) / perimeter;
    MI->z = (a * A.z + b * B.z + c * C.z) / perimeter;
    // radius 
    *rI = (2.0 * area) / perimeter;
}
/*
 * Funktion:  centroid
 * Zweck:     Berechnet den Schwerpunkt (Massenmittelpunkt) eines Dreiecks.
 * Parameter:
 *   - A, B, C: Punkte des Dreiecks
 * Rückgabewert:
 *   - Ein Vector3 mit den Koordinaten des Schwerpunkts S = (A + B + C) / 3
 */
Vector3 centroid(Vector3 A, Vector3 B, Vector3 C) { // schwerpunkt S = (A + B + C)/3
    Vector3 S;
    S.x = (A.x + B.x + C.x) / 3.0;
    S.y = (A.y + B.y + C.y) / 3.0;
    S.z = (A.z + B.z + C.z) / 3.0;
    return S;
}
/*
 * Funktion:  circumcenterAndRadius
 * Zweck:     Berechnet den Umkreis (Mittelpunkt und Radius) eines Dreiecks im 3D-Raum.
 * Beschreibung:
 *   - Im 2D-Fall (xy-Ebene) liegt der Umkreismittelpunkt dort, wo die 
 *     Mittelsenkrechten (Perpendikularen) der Dreiecksseiten sich schneiden.
 *   - Da unsere Punkte A, B, C aber im 3D liegen, projiziert die Funktion das 
 *     Dreieck in ein lokales 2D-Koordinatensystem (z.B. mit e1, e2 in der Dreiecksebene).
 *   - Anschließend wird im 2D die Mittelsenkrechte berechnet und 
 *     zurück in den 3D-Raum transformiert.
 * Parameter:
 *   - A, B, C: Eckpunkte des Dreiecks
 *   - MU:      Pointer auf Vector3, in das der Umkreismittelpunkt geschrieben wird
 *   - rU:      Pointer auf double, in das der Umkreisradius gespeichert wird
 */
void circumcenterAndRadius(Vector3 A, Vector3 B, Vector3 C, Vector3 *MU, double *rU) {  
                           // umkreis vom mittelpunt ;U und radius RU in 3D 
                           // 2D koordinaten in der ebene vom dreieck B und C vom uhrsprung A "zeichnen"
                           // danach 2D mittelpunkt wieder in 3D 
                           // RU ist der abstand bon MU zu A / B / C 

    // Vektor AB & AC                        
    Vector3 ABv = {0.0, 0.0, 0.0};
    Vector3 ACv = {0.0, 0.0, 0.0};
    ABv = vsub(B, A);
    ACv = vsub(C, A);
    // normalenvektor für die ebene AB*AC 
    Vector3 normal = {0.0, 0.0, 0.0};
    normal = vcross(ABv, ACv);
    double normalLen = vlength(normal);
    // e1 = Einheitsvektor in Richtung AB
    double lenAB = vlength(ABv);
    Vector3 e1 = {0.0, 0.0, 0.0};
    if (lenAB > EPS) {
        e1 = vscale(ABv, 1.0 / lenAB);
    }
    // e2 = normal x e1 (danach normieren)
    Vector3 e2raw = {0.0, 0.0, 0.0};
    e2raw = vcross(normal, ABv);
    double e2len = vlength(e2raw);
    Vector3 e2 = {0.0, 0.0, 0.0};
    if (e2len > EPS) {
        e2 = vscale(e2raw, 1.0 / e2len);
    }
    // Nun werden   B und C ins (e1, e2)-Koordinatensystem projiziert
    // B' = (b_x, b_y), C' = (c_x, c_y)
    // Indem  das Skalarprodukt mit e1 bzw. e2 genutz wird 
    double b_x = vdot(ABv, e1);// Projektionskoordinate B' auf e1
    double b_y = vdot(ABv, e2);// Projektionskoordinate B' auf e2
    double c_x = vdot(ACv, e1);// Projektionskoordinate C' auf e1
    double c_y = vdot(ACv, e2);// Projektionskoordinate C' auf e2
    // A' ist in diesem Koordinatensystem (0,0)

    // gesucht ist der Schnitt zweier Mittelsenkrechten:
    //   1) die Mittelsenkrechte von A'B'
    //   2) die Mittelsenkrechte von B'C'

    // Mittelpunkt von A'B': 
    // A'=(0,0), B'=(b_x, b_y)
    // => midAB = (b_x/2, b_y/2)

    double midAB_x = b_x / 2.0;
    double midAB_y = b_y / 2.0;
    // Mittelpunkt von B'C':
    // B'=(b_x, b_y), C'=(c_x, c_y)
    // => midBC = ((b_x+c_x)/2, (b_y+c_y)/2)

    double midBC_x = (b_x + c_x) / 2.0;
    double midBC_y = (b_y + c_y) / 2.0;
    // Richtungsvektoren AB' und BC':

    double ABp_x = b_x;
    double ABp_y = b_y;
    double BCp_x = (c_x - b_x);
    double BCp_y = (c_y - b_y);
    // Die Senkrechten erhält man durch 90°-Rotation (u, v) -> (-v, u).
    double dirABp_x = -ABp_y;
    double dirABp_y = ABp_x;
    double dirBCp_x = -BCp_y;
    double dirBCp_y = BCp_x;
    //   midAB + alpha*(dirAB) = midBC + beta*(dirBC)
    // => alpha*dirAB - beta*dirBC = (midBC - midAB)

    // midBC - midAB:
    double rx = midBC_x - midAB_x;
    double ry = midBC_y - midAB_y;
    
    // p = dirAB, q = -dirBC, r = (rx, ry)
    double p_x = dirABp_x;
    double p_y = dirABp_y;
    double q_x = -dirBCp_x;
    double q_y = -dirBCp_y;

    double det = p_x * q_y - p_y * q_x;
     // Falls det ~ 0, ist das Dreieck degeneriert oder fast kollinear
    if (fabs(det) < 1e-14) {
        *MU = A;
        *rU = 0.0;
        return;
    }

    double alpha = (rx * q_y - ry * q_x) / det; // alpha bestimmen

    double O2D_x = midAB_x + alpha * p_x; // koordinaten des mittelpunkts in 2D
    double O2D_y = midAB_y + alpha * p_y;

    Vector3 tmp = vadd(vscale(e1, O2D_x), vscale(e2, O2D_y)); // zurück transformieren in 3D
    Vector3 O3D = vadd(A, tmp);   //     O3D = A + O2D_x*e1 + O2D_y*e2

    double radius = vlength(vsub(O3D, A)); //Radius = Abstand von O3D zu A (bzw. zu B oder C – alle gleich)

    *MU = O3D;   // werte zurückgeben
    *rU = radius;
}



/*
 * Hauptfunktion: main
 * Zweck:         Fragt wiederholt drei Punkte (A, B, C) vom Nutzer ab und 
 *                berechnet wesentliche Kenngrößen des Dreiecks:
 *                - Seitenlängen
 *                - Umfang
 *                - Fläche
 *                - Innenwinkel
 *                - Inkreis (Mittelpunkt, Radius)
 *                - Schwerpunkt
 *                - Umkreis (Mittelpunkt, Radius)
 *                Das Programm läuft in einer Endlosschleife und erwartet 
 *                nach jeder Berechnung einen Tastendruck (ENTER), 
 *                bevor es erneut startet.
 */
int main(void) {
      
      Vector3 A = {0.0, 0.0, 0.0};
      Vector3 B = {0.0, 0.0, 0.0};
      Vector3 C = {0.0, 0.0, 0.0};                // Eckpunkte
      double AB = 0.0, BC = 0.0, CA = 0.0;        // Seitenlängen
      double U  = 0.0;                            // Umfang
      double F  = 0.0;                           // Fläche
      double phiA = 0.0, phiB = 0.0, phiC = 0.0; // Innenwinkel in Bogenmaß
      Vector3 MI = {0.0, 0.0, 0.0};
      double rI = 0.0;                          // Inkreismittelpunkt
      double rI;                                // Inkreisradius
      Vector3 S = {0.0, 0.0, 0.0};              // Schwerpunkt
      Vector3 MU = {0.0, 0.0, 0.0};             // Umkreismittelpunkt
      double rU = 0.0;                          // Umkreisradius
      
      // Endlosschleife, damit man mehrere Dreiecke nacheinander testen kann
      while (1) 
      {
          printf("Programm zur Dreiecksberechnung im Raum.\n");
          
          // 1) A, B, C einlesen
          A = readPoint('A');
          B = readPoint('B');
          C = readPoint('C');
          
          // 2) Prüfen, ob ein gültiges Dreieck vorliegt
          if (!checkTriangle(A, B, C)) {
              printf("Fehler: Die Punkte ergeben kein gueltiges Dreieck.\n");
              return 0;
          }
  
          // 3) Seitenlängen berechnen
          triangleSideLengths(A, B, C, &AB, &BC, &CA);
          printf("Seitenlaengen: |AB|=%.4f, |BC|=%.4f, |CA|=%.4f\n", AB, BC, CA);
  
          // 4) Umfang
          U = trianglePerimeter(AB, BC, CA);
          printf("Umfang U = %.4f\n", U);
  
          // 5) Fläche
          F = triangleArea(A, B, C);
          printf("Flaecheninhalt F = %.4f\n", F);
  
          // 6) Winkel (in Bogenmaß) ermitteln und gleich in Grad ausgeben
          triangleAngles(A, B, C, &phiA, &phiB, &phiC);
          printf("Innenwinkel (in Grad):\n");
          printf("  phiA = %.2f°\n", toDeg(phiA));
          printf("  phiB = %.2f°\n", toDeg(phiB));
          printf("  phiC = %.2f°\n", toDeg(phiC));
  
          // 7) Inkreis (Mittelpunkt & Radius)
          incenterAndRadius(A, B, C, AB, BC, CA, F, &MI, &rI);
          printf("Inkreismittelpunkt MI = (%.4f, %.4f, %.4f), rI = %.4f\n",
                 MI.x, MI.y, MI.z, rI);
  
          // 8) Schwerpunkt
          S = centroid(A, B, C);
          printf("Schwerpunkt S = (%.4f, %.4f, %.4f)\n", S.x, S.y, S.z);
  
          // 9) Umkreis (Mittelpunkt & Radius)
          circumcenterAndRadius(A, B, C, &MU, &rU);
          printf("Umkreismittelpunkt MU = (%.4f, %.4f, %.4f), rU = %.4f\n",
                 MU.x, MU.y, MU.z, rU);
  
          // Warten auf ENTER, damit man danach neue Werte eingeben kann
          getchar();
      }
  
      return 0;
}

