#include <stdio.h>
#include <stdint.h> // für unit64_t da alle anderen werte zu klein sind 
// unit64_t ist einfach ein anderer datentyp der nur positive ganz zahlen speichern kann 
// aber anders als zb int welcher auf 32 bit basiert und nur 2.1 millionen werte speicher kann
// kann unit64_t 18 trillionen speichern da es auf 64 bit bassiert welches für dieses projekt notwendig ist 
uint64_t NummerZuByte(double wert, char faktor){
       switch (faktor) {
        case 'b' : 
             return (uint64_t)(wert / 8);  
          case 'B' : 
             return (uint64_t)(wert * 1); 
          case 'K' :                                // die einzelnen fälle für den multiplikator ( also das ASCII zeichen)
             return (uint64_t)(wert * 1000);         // unit64_t steht hier damit die rückgabe zu 100% in unit64_t ist da sie ansonsten zu groß wäre 
          case 'k' :                                // die einzelnen fälle für den multiplikator ( also das ASCII zeichen)
             return (uint64_t)(wert * 1000 / 8);
          case 'M' :                              
             return (uint64_t)(wert * 1000000);
          case 'm' :                              
             return (uint64_t)(wert * 1000000 / 8);
          case 'G' :
             return (uint64_t)(wert * 1000000000);
          case 'g' :
             return (uint64_t)(wert * 1000000000 / 8);
          case 'T' :
             return (uint64_t)(wert * 1000000000000);
          case 't' :
             return (uint64_t)(wert * 1000000000000 / 8);
          default :
             return 0;
       }
  }
            
 void TransferZeitBerechnung(uint64_t sizeInBytes, double rateInBitsProSekunde) {
    double rateInBytesProSekunde = rateInBitsProSekunde / 8.0;
    double SekundenRoh = (double)sizeInBytes / rateInBytesProSekunde;
    
    uint64_t Stunden = (uint64_t)(SekundenRoh / 3600);                     // eine stunde hat 3600 sekunden
    uint64_t Minuten = (uint64_t)((uint64_t)SekundenRoh % 3600) / 60;    // % ( modulo) berechnet den Rest nachdem die stunden abgezogen wurden und gibt die minuten danach aus ( 1 min = 60s)
    uint64_t sekunden = (uint64_t)SekundenRoh % 60;                       // % ( modulo) berechnet den Rest nachdem die minuten abgezogen wurden und gibt die Sekunden danach aus
                                                                      // unit64_t cast wird bei allen verwendet um sicherzugehen das es im richtigen datentyp gespeichert wird   
    printf("%02llu h %02llu m %02llu s bei %.0f kbit/s\n", Stunden, Minuten, sekunden, rateInBitsProSekunde / 1000);
     // 02 ist sozusagen die format angabe für die mindeststellen 11u ist ein platzgalter fpr Unsingend long long oder auch unit64_t
 }
 
 
 int main(){
      char eingabe[256];
      // eingabe der daten größe 
      printf("Geben sie bitte eine Datei groese ein (zb 10M fuer 10MByte, 10m fuer 10Mbit): ");
      fgets(eingabe, sizeof(eingabe), stdin); // für die eingabe mehrerer größen
      char *current = eingabe;
      while (*current != '\0'){
          double sizeWert;
          char sizeEinheit;
         while (*current == ' '){
         current++;
        } 
       if(sscanf(current,"%lf%c", &sizeWert, &sizeEinheit) !=2){
         printf("Ungueltige Eingabe: %s\n", current);
         break;
       }
       //umrechnung in Bytes 
       uint64_t sizeInBytes = NummerZuByte(sizeWert, sizeEinheit); // die funtktion NummerZuByte (ganz am anfang) wird aufgerufen mit den beiden Variablen.
       if (sizeInBytes == 0) {  // bei falscher bzw leher eingabe kommt diese Error meldung 
        printf("Ungueltige einheit, bitte verwende nur B K,M,G oder T \n");
       break;
       }

        // transfergeschwindigkeit in bit pro sekunde    
       double TransferGeschwindigkeit[] = {64000, 2000000, 16000000, 100000000}; // array mit den verschiedenen geschwindigkeiten
       printf("Die Transferdazer fuer die Datei von %.2lf%c betraegt:\n", sizeWert, sizeEinheit);
        for(int i = 0; i < 4; i++){
        TransferZeitBerechnung(sizeInBytes, TransferGeschwindigkeit[i]);
       }
       while(*current != ',' && *current != '\0'){
       current++;
       }
        if(*current == ','){
        current++;
       }
      }     
    printf("\nDruecken sie die eingabe um das programm zu beenden");
    getchar();
    return 0;
    }

   
  