#include "loesung.c"

void test_sort(){
  for(int i=0; i<anzKacheln-1; i++){
    if(binCompare(Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]) < 0){
      printf("FEHLER in sortierung: %d,%d \n",i,i+1);
      printf("%u,%u > %u,%u \n",Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]);
    }
  }
}

int testus(void){
    einlesen();
    sort();
    test_sort();
}
