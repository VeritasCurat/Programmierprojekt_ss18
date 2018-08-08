/*
 ============================================================================
 Name        : eingabe.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright noticeo,,,
 Description : Programmierprojekt SS18
 Test der Eingabe
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//Phase1: Initialisieren der DS, Einlesen von String und umwandeln in unsigned int Element_liste[][2]:
 static void EL_reallozieren();
 static void EL_allozieren();
 static void EL_einfuegen(unsigned int x, unsigned int y);
 static void EL_ausgabe();
 static void Array_freigabe(unsigned int **array, size_t size);
 void init();
 void einlesen();
 void koordinate_einlesen(char* zeile);
 int chartoint(char c);
 int legalesZeichen(char c);

 unsigned int **Element_liste = 0; int EL_platz=1000; int EL_anz=0;//hier werden alle Koordinaten gespeichert

 int strlen(char *eingabe) {
     int i = 0;
     while (eingabe[i] != '\0') {
         i++;
     }
     return i;
 }

 //allozierten speicher freigeben
 void beenden(){
   Array_freigabe(Element_liste, EL_anz);
 }

 static void Array_freigabe(unsigned int **array, size_t size){
     for (size_t i = 0; i < size; i++)
         free(array[i]);
     free(array);
 }

 static void EL_allozieren(){
   Element_liste = (unsigned int **)malloc(EL_platz * sizeof(*Element_liste));
   if(Element_liste == NULL){
     printf("Elementliste konnte nicht alloziert werden!");
     exit(0);
   }
 }

 static void EL_reallozieren(){
   int newnum;
   if(EL_platz < 100000) newnum = (EL_platz + 2) * 2;   /* 4, 12, 28, 60, ... */
   else{  newnum = (EL_platz + 2) + 100000; }
   unsigned int **newptr = (unsigned int **)realloc(Element_liste, newnum * sizeof(*Element_liste));
   printf("EL_platz %d\n", EL_platz);
   if (newptr == NULL){
       Array_freigabe(Element_liste, EL_anz);
       exit(1);
   }
   EL_platz = newnum;
   Element_liste = newptr;
 }

 static void EL_einfuegen(unsigned int x, unsigned int y){
   if (EL_anz == EL_platz)
   {
     EL_reallozieren();
   }
   Element_liste[EL_anz] = (unsigned int *)malloc(2 * sizeof(unsigned int));
   if (Element_liste[EL_anz] == 0)
   {
       Array_freigabe(Element_liste, EL_anz);
       exit(1);
   }
   Element_liste[EL_anz][0] = x;
   Element_liste[EL_anz][1] = y;
   EL_anz++;
 }

 static void EL_ausgabe(){
   for(int zeile=0; zeile<EL_anz; zeile++){
     printf("%d: (%u,%u)\n",zeile, Element_liste[zeile][0], Element_liste[zeile][1]);
   }
 }

 void init(){
   EL_allozieren();
 }

 void einlesen(){
   char buffer[1000000];
   while(fgets(buffer, sizeof(buffer), stdin) != NULL){
     koordinate_einlesen(buffer);
   }
 }

 void koordinate_einlesen(char* zeile){
   unsigned int a,b; a=b=0;
   int a_len = 0; int b_len =0;
   int i=0;
   for(;i<strlen(zeile); i++){
     if(zeile[i]==' ')continue;
     if(chartoint(zeile[i]) != -1) break;
   }

   for(;i<strlen(zeile); i++){
     if(zeile[i]==' ')break;
     else if(chartoint(zeile[i]) != -1){
       if(a>0)++a_len;
       a*=(unsigned) 10;
       a+=(unsigned) chartoint(zeile[i]);
     }
     else {
       printf("illegales Zeichen! (%c)\n",zeile[i]);
       exit(-1);
     }
     if(a_len > 10){
       printf("%s (%s)\n", "zu lange Zahlen!",zeile);
       exit(-1);
     }
   }

   for(;i<strlen(zeile); i++){
     if(zeile[i]==' ')continue;
     if(chartoint(zeile[i]) != -1) break;
   }

   for(;i<strlen(zeile); i++){
     if(zeile[i]==' ' || zeile[i]=='\n')break;
     else if(chartoint(zeile[i]) != -1){
       if(b>0)++b_len;
       b*=(unsigned) 10;
       b+=(unsigned) chartoint(zeile[i]);
     }
     else {
       printf("illegales Zeichen! (%c)\n",zeile[i]);
       exit(-1);
     }
     if(b_len > 10){
       printf("%s (%s)\n", "zu lange Zahlen! ",zeile);
       exit(-1);
     }
   }

   for(;i<strlen(zeile); i++){
     if(zeile[i]==' ')continue;
     if(chartoint(zeile[i]) != -1) break;
   }

   if(a >= 4294967296 || b >= 4294967296 || a < 0 || b < 0){
     printf("%s\n", "zu lange Zahlen!");
     exit(-1);
   }
   EL_einfuegen(a,b);
 }

 int legalesZeichen(char c){
   if(chartoint(c) >= 0 && chartoint(c) <= 9)return 1;
   if(c == ' ' || c == '\n') return 1;
   return 0;
 }

 int chartoint(char c){
  switch(c){
    case '0': return 0; break;
    case '1': return 1; break;
    case '2': return 2; break;
    case '3': return 3; break;
    case '4': return 4; break;
    case '5': return 5; break;
    case '6': return 6; break;
    case '7': return 7; break;
    case '8': return 8; break;
    case '9': return 9; break;
    default: return -1; break;
  }
 }

int main(){
  init();
  printf("initialisiert ...\n");

  einlesen();
  printf("eingelesen ...\n");
}
