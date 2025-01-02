#include <stdio.h>
#include <time.h>

int main () {

 char eingabe;                        // char für die eingabe, also die buchstaben
 time_t start_zeit = 0;               // startzeit der Stoppuhr
 time_t letze_runde_zeit = 0;         // zeit der letzten runde
 time_t aktuelle_zeit = 0;            // 3 variablen die auf die Unix zeit gesetzt werden und dann auf 0
 time_t runden_zeit[100];             // array für die rundenzeiten
 int runde = 0;                       // int um die runden zu zählen

 printf("Stoppuhr starten mit 'S', Rundenzeit mit 'Z' und stoppen mit 'X':\n");
  
  while(1){
    
    scanf("%c%*c", &eingabe); // eingabe eines ASCII zeichens, backspace ( \n) wird rausgefiltert mit %*c

    if(eingabe == 'S'){               // stoppuhr starten
       time(&start_zeit);             // aktuelle zeit wird als start zeit festgelegt 
       letze_runde_zeit = start_zeit;
       struct tm *start_tm = localtime(&start_zeit);
        printf("Stoppuhr gestartet! Startzeit: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);  // gibt die aktuelle startzeit in hh:mm:ss format aus 
     }
     else if (eingabe == 'Z'){                     // wenn die eingabe Z ist ( für eine runde ) aber die zeit noch 0 ist kommt ein error 
     if (start_zeit == 0){
        printf("Uhr wurde noch nicht gestartet");
        continue;
     }
     time(&aktuelle_zeit); // aktuelle zeit abrufen
     runde++;             // rundenzähler um 1 erhöhen
     time_t  runden_differrenz = aktuelle_zeit - letze_runde_zeit;  // diffrerenz zur letzten runde 
     time_t  gesamt_differrenz = aktuelle_zeit - start_zeit;        // gesamtzeit 
     runden_zeit[runde - 1] = aktuelle_zeit - start_zeit;           // zeit der aktuellen runde speichern im array 
     printf("%d Zwischenzeit: Gesamtzeit: %02ldm %02lds, Differenz: %02ldm %02lds\n",runde, gesamt_differrenz / 60, gesamt_differrenz % 60, runden_differrenz / 60, runden_differrenz % 60);  // aktuelle zeit als runde setezen
        letze_runde_zeit = aktuelle_zeit;
     } 
     else if(eingabe == 'X') {   // stoppuhr stoppen
     if(start_zeit == 0){        // fehler überprüfen, läuft die uhr schon ? 
      printf("Stoppuhr noch nicht gestartet!\n");
      continue;
     } 
     time(&aktuelle_zeit);                           // ruft die aktuelle zeit ab 
     printf("Stoppuhr gestoppt!\n");                 
     printf("Rundenuebersicht\n");
     struct tm *start_tm = localtime(&start_zeit);
        printf("Startzeit: %02d:%02d:%02d\n", start_tm->tm_hour, start_tm->tm_min, start_tm->tm_sec);  // gibt die startzeit der uhr aus  in hh:mm:ss
       
     for(int i = 0; i<= runde; i++){
        
        printf("%d. Zwischenzeit: %02ldm %2lds\n", i + 1,runden_zeit[i] / 60, runden_zeit[i] % 60);  // gibt die einzelnen runden aus 
     }
     getchar();
     break;
     }
    else {
        printf("Falsche eingabe, nur 'S' 'Z' 'X' eingeben\n");
    }
  }
 return 0;


}