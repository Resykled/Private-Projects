#include <stdio.h>

int main(){
    while(1){
    char eingabe[50]; // array für die eingegebenen zeichen 
    int gueltig = 1; // variable für ungültige eingaben 
    int laenge = 0; // variable für die länge der zahlenkette ( also von dem array)
    int zaehler[256] = {0}; // array für alle askii zeichen 
    char temp; // themporere variable fürs sortieren nach dem alphabet
    
    printf("bitte geb ein zeichen ein(nur C,D,H,Q,T,Y)");
    scanf ("%s", eingabe);



  for( int i = 0; eingabe[i] != '\0'; i++ ){  
    zaehler[(unsigned char)eingabe[i]]++;
     if (eingabe[i] != 'C' && eingabe[i] != 'D' && eingabe[i] != 'H' && eingabe[i] != 'Q' && eingabe[i] != 'T' && eingabe[i] != 'Y') {
       // überprüfung das die eingegebenen zeichen auch gültig sind 
        gueltig = 0; // setzt die variable auf 0 bzw low wenn ein zeichen ungültig ist 
        break; // break stopt dann den code 
     }

  }

 if(gueltig){
    
      while(eingabe[laenge] != '\0'){
      laenge++;
      }
      for(int i = 0; i< laenge -1; i++){       // hier wird mit dem ASCII code sortiert
        for(int j = 0; j < laenge -i -1; j++){ // ascii zeichen Wie  text oder zb "!"  haben alle eine zugewiesene zahl 
            if(eingabe[j] > eingabe[j +1]){    // diese zahlen sind für buchstaben in alphabetischer reinvolge größer werdend
               temp = eingabe[j];              // was in diesem fall von mir für das sorteren genutzt wird 
               eingabe[j] = eingabe[j +1];
               eingabe[j + 1 ] = temp;

            }
        }
      }
     printf("Sortierte zeichenkette : %s\n", eingabe);
     printf("die zeichenkette ist: %d ", laenge );
     printf("stellen lang\n");
     printf("In der Zeichenkette vorkommende Zeichen und deren Anzahl: ");
     int ersteZeichen = 1;  // damit das erste zeichen kein komma davor hat , also so eine art platzhalter 
     for(int i = 0; i< 256; i++){
      if(zaehler[i] > 0) {
        if(!ersteZeichen){
          printf(", ");  // komma und lehrzeichen außer bei dem ersten zeichen 
        }
        printf("%c(%d)",i, zaehler[i]);
        ersteZeichen = 0;   // wird null gesetzt wenn das erste zeichen ausgegeben wurde 
      }
     }
     printf("\n");
     
    }
    else{
        printf("falsche eingabe, ");
    }
    
    }
 return 0;
}