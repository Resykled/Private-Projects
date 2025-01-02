#include <stdio.h>
#include <math.h>

int main()
{
    for(int i = 33; i<127; i++){
    printf("%c #%d #0x%x\n",i,i,i);
    }
    char eingabe ; 
    printf("Bitte gebe ein ASCIIzeichen ein ");
   
    while(1){
       printf("Eingabe:");
       eingabe = getchar();
      while(eingabe == '\n'){
         eingabe = getchar();
        }
        
        
     printf("%c #%d #0x%x\n",eingabe,eingabe,eingabe);
    }
    return 0;
    
}
