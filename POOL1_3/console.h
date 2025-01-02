//#define UNIX
//#define WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef UNIX
    #include <termios.h>
    #include <unistd.h>
    #define UNIXCONSOLEIO ; // do nothing, already enabled
#endif

#ifdef WINDOWS
    #include <conio.h>
    #include <windows.h>
    // Erlaube der Windows Console auf ESC Steuersequenzen zu reagieren
    #define UNIXCONSOLEIO { HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); DWORD dwMode = 0; GetConsoleMode(hOut, &dwMode); SetConsoleMode(hOut, dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING);}
#endif

 /* Unix kompatible Defines zur Positionierung des Cursors auf der Konsole */
 /* ANSI Escape Sequences start by ESC \x1b[ followed by an escape code */
 /* See also "https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797" */
 /* CLS = Clear Screen. Use: CLS; */
 #define CLS printf("\x1b[2J");fflush(stdout)
 /* CURPOS(L,C) = Positioniere den Cursor in Zeile L und Spalte C.
  * Use: to print a text in line L starting at column C
  * CURPOS(L,C); printf("place your text here"); */
 #define CURPOS(L,C) printf("\x1b[%d;%df",L,C);fflush(stdout)
 /* CURPOSTEXT(L,C,T) = Positioniere den Cursor in Zeile L und Spalte C.
  * Use: to print text T in line L starting at column C
  * CURPOSTEXT(L,C,T); T="place your text here" */
 #define CURPOSTEXT(L,C,T) printf("\x1b[%d;%df",L,C);printf("\x1b[0m%s",T);fflush(stdout)
 /* CURPOSGREENTEXT(L,C,T) = Positioniere den Cursor in Zeile L und Spalte C.
  * Use: to print a green colored text T in line L starting at column C
  * CURPOSGREENTEXT(L,C,T); T="place your text here" */
 #define CURPOSGREENTEXT(L,C,T) printf("\x1b[%d;%df",L,C);printf("\x1b[32m%s\x1b[0m",T);fflush(stdout)
 #define CURPOSGREENFTEXT(L,C,F,T) printf("\x1b[%d;%df\x1b[32m",L,C);printf(F,T);printf("\x1b[0m");fflush(stdout)
 /* CURPOSGREENSTR(L,C,F,S) = Positioniere den Cursor in Zeile L und Spalte C.
  * Use: to print a green colored String (F) followed by String S in line L starting at column C
  * CURPOSGREENSTR(L,C,F,S); F=StringF, S=StringS */
 #define CURPOSGREENSTR(L,C,F,S) printf("\x1b[%d;%df",L,C);printf("\x1b[32m%s %s\x1b[0m",F,S);fflush(stdout)
 /* CURUP(L) = Bewege den Cursor um L Zeilen nach oben. */
 #define CURUP(L) printf("\x1b[%dA",L);fflush(stdout)
 /* CURDOWN(L) = Bewege den Cursor um L Zeilen nach unten. */
 #define CURDOWN(L) printf("\x1b[%dB",L);fflush(stdout)
 /* CURRIGHT(C) = Bewege den Cursor um C Spalten nach rechts. */
 #define CURRIGHT(C) printf("\x1b[%dC",C);fflush(stdout)
 /* CURLEFT(C) = Bewege den Cursor um C Spalten nach links. */
 #define CURLEFT(C) printf("\x1b[%dD",C);fflush(stdout)
 /* CUR2EOL = Lösche alle Zeichen rechts ab aktuelle CURPOS */
 #define CURCLR2EOL printf("\x1b[K");fflush(stdout)

#ifdef WINDOWS
    #define kbhit _kbhit
    #define getch _getch
    #define getche _getche
#endif

 #ifdef UNIX
 /*
 Funktion  : int kbhit(void)
 Aufgabe   : Prueft, ob eine Taste gedrueckt wurde. Gibt die Taste nicht aus.
 Eingaben  : keine
 Ausgaben  : 0 falls keine Taste gedrueckt wurde
             1 falls eine beliebige Taste gedrueckt wurde
 Autor     : Internetquelle
 Version   : 02
 Datum der letzten Änderung: 2016-01-22, SK
 */
int kbhit(void){
    struct termios newterm, oldterm;
    int ch = -1, fd = 0;
    fd = fileno(stdin);
    if(tcgetattr(fd, &oldterm)==-1)
    	return(-1);
	memcpy(&newterm, &oldterm, sizeof(struct termios));
    newterm.c_lflag &= ~(ICANON); /* disable canonical mode */
    newterm.c_cc[VMIN] = 0; /* get at least 0 chars */
    newterm.c_cc[VTIME] = 1; /* timeout = 100ms */
    if(tcsetattr(fd, TCSANOW, &newterm)==-1)
    	return(-1);
    ch = getchar();
    if(tcsetattr(fd, TCSANOW, &oldterm)==-1)
    	return(-1);
    if (ch != -1)
        ungetc(ch, stdin); /* flush stdin */
    return ((ch != -1) ? 1 : 0);
}

 /*
 Funktion  : int getch(void)
 Aufgabe   : Liest ein Zeichen von stdin. Gibt das Zeichen nicht aus.
 Eingaben  : keine
 Ausgaben  : ASCII-Code des eingelesenen Zeichens
             -1 falls kein Zeichen in stdin
 Autor     : Internetquelle
 Version   : 02
 Datum der letzten Änderung: 2016-01-22, SK
 */
 int getch(void){
     struct termios newterm, oldterm;
     int ch = -1, fd = 0;
     fd = fileno(stdin);
     if(tcgetattr(fd, &oldterm)==-1)
    	return(-1);
     memcpy(&newterm, &oldterm, sizeof(struct termios));
     newterm.c_lflag &= ~(ICANON|ECHO); /* disable canonical mode and echoing */
     newterm.c_cc[VMIN] = 1; /* get at least 1 chars */
     newterm.c_cc[VTIME] = 0; /* timeout = infinite */
     if(tcsetattr(fd, TCSANOW, &newterm)==-1)
    	return(-1);
     ch = getchar();
     if(tcsetattr(fd, TCSANOW, &oldterm)==-1)
    	return(-1);
     return ch;
}

/*
 Funktion  : int getche(void)
 Aufgabe   : Liest ein Zeichen von stdin. Gibt das Zeichen aus.
 Eingaben  : keine
 Ausgaben  : ASCII-Code des eingelesenen Zeichens
             -1 falls kein Zeichen in stdin
 Autor     : Internetquelle
 Version   : 02
 Datum der letzten Änderung: 2016-01-22, SK
 */
int getche(void){
    struct termios newterm, oldterm;
    int ch = -1, fd = 0;
    fd = fileno(stdin);
    if(tcgetattr(fd, &oldterm)==-1)
    	return(-1);
    memcpy(&newterm, &oldterm, sizeof(struct termios));
    newterm.c_lflag &= ~(ICANON); /* disable canonical mode */
    newterm.c_lflag |= ECHO; /* enable echo mode */
    newterm.c_cc[VMIN] = 1; /* get at least 1 chars */
    newterm.c_cc[VTIME] = 0; /* timeout = infinite */
    if(tcsetattr(fd, TCSANOW, &newterm)==-1)
    	return(-1);
    ch = getchar();
    if(tcsetattr(fd, TCSANOW, &oldterm)==-1)
    	return(-1);
    return ch;
}
#endif
