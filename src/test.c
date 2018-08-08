#include "loesung_binSearch.c"
#include <sys/resource.h>

//Testfunktionen
  //Phase1: Einlesen
  void test_eingabe();

  //Phase2: Sortieren
  void test_sort();

  //Phase3: Raumeinteilung
  int validate_raeume();
  void binSearchTEST();
  int lineare_suche(unsigned int x, unsigned int y);
  int abs(int a);
  int Benachbart(unsigned int x[], unsigned int y[]);
  int validate_raeume();

  //Phase4: Lösen
  int validate();
  int validate2();

//Phase 2:
void test_sort(){
  printf("Sortierung ...");
  for(int i=0; i<EL_anz-1; i++){
    if(compare(Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]) > 0){
      printf("FEHLER in sortierung: %d,%d \n",i,i+1);
      printf("%u,%u > %u,%u \n",Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]);
    }
  }
  printf(" OK!\n");
}

//Phase 3:
void binSearchTEST(){
  printf("binSearch TEST ...");
  int fehlgeschlagen=0;
  for(int i=0; i<EL_anz; i++){
    //printf("(%u,%u): \n",Element_liste[i][0],Element_liste[i][1]);

    if(binSearch(Element_liste[i][0],Element_liste[i][1]) == -1){
      fehlgeschlagen = 1;
      debug2 = 1;
      binSearch(Element_liste[i][0],Element_liste[i][1]);
      debug2 = 0;
      printf("lineare Suche: %d \n",lineare_suche(Element_liste[i][0],Element_liste[i][1]));
      printf("nicht gefunden!\n\n\n");
    }
    else {
      continue;
      //printf("gefunden!\n");
    }
  }
  if(fehlgeschlagen == 0)printf("%s\n", " OK!" );
}

int lineare_suche(unsigned int x, unsigned int y){
  for(int i=0; i< EL_anz; i++){
    if(Element_liste[i][0] == x && Element_liste[i][1] == y){
      return i;

    }
  }
  return -1;
}

int abs(int a){
  if(a < 0) return a*-1;
  return a;
}

int Benachbart(unsigned int x[], unsigned int y[]){
  if(abs((x[0] - y[0]) + (x[1] - y[1])) == 1) return 1;
  return 0;
}

int validate_raeume(){
  printf("Raeume ...");
  int valid=1;
  //ist jedes Element aus Raemen in Elementliste?

  if(gruppiert != EL_anz)valid = 0;

  //Raumgroesse = Elementliste
  if(valid == 0)return valid;

  //genügt jeder Raum den Raumeigenschaften (benachbart Eigenschaften) => jedes ELement hat MIN einen Nachbarn
  for(int i=0; i<R_anz; i++){
    if(binSearch(Element_liste[Raeume[i][1]][0],Element_liste[Raeume[i][1]][1]+1) > -1) continue;
    if(binSearch(Element_liste[Raeume[i][1]][0],Element_liste[Raeume[i][1]][1]-1) > -1) continue;
    if(binSearch(Element_liste[Raeume[i][1]][0]+1,Element_liste[Raeume[i][1]][1]) > -1) continue;
    if(binSearch(Element_liste[Raeume[i][1]][0]-1,Element_liste[Raeume[i][1]][1]) > -1) continue;
    valid =0;
  }

  if(valid == 1)printf(" OK!\n");
  else printf(" nicht OK!\n");

  return valid;
}



//Phase 4:

int validate_loesung(){
  printf("Loesung ... ");
  int valid = 1;
  //geloest == EL_anz
  if(index_loesung*2 != EL_anz)valid = 0;

  unsigned int Loesung[gruppiert][2];
  for(int i=0; i<R_anz; i++){
    Loesung[i][0]=i;
    Loesung[i][1]=geloest_liste[i];
  }

  qsort(Loesung, EL_anz, sizeof(int)*2, compL);

  if(index_loesung*2 < EL_anz){
    printf("index_loesung < EL_anz: %d < %d\n",index_loesung*2, EL_anz);
    for(int i=0; i<EL_anz; i++){
      if(geloest_liste[i] == -1){
        printf("%d nicht in Loesung\n", i);
        //rintf("nicht in Loesungs: (%u,%u)\n",Element_liste[geloest_liste[i]][0], Element_liste[geloest_liste[i]][1]);
      }
    }
  }

  for(int i=0; i<gruppiert; i+=2){
    int benachbart = Benachbart(Element_liste[Loesung[i][0]], Element_liste[Loesung[i+1][0]]);
    if(benachbart!= 1){
      valid =0;
      printf("nicht benachbart: (%u,%u)(%u,%u)\n",Element_liste[Loesung[i][0]][0], Element_liste[Loesung[i][0]][1], Element_liste[Loesung[i+1][0]][0], Element_liste[Loesung[i+1][0]][1]);
    }
  }

  if(valid == 1)printf(" OK!\n");
  else printf(" nicht OK!\n");

  return valid;
}







static void raume_print(){
  for(int i=0; i<gruppiert; i++){
    printf("%d Raum: %d, Index:%d, (%u,%u)\n",i,Raeume[i][0], Raeume[i][1], Element_liste[Raeume[i][1]][0],Element_liste[Raeume[i][1]][1]);
  }
}

void printlist(){
  for(int i=0; i<EL_anz; i++){
    printf("%u: A: %u, B: %u\n", i, Element_liste[i][0] , Element_liste[i][1]);
  }
}

int testus(void){
  init();
  //printf("initialisiert ...\n");

  einlesen();
  //printf("eingelesen ...\n");

  EL_sort();
  //printf("sortiert ...\n");

  //EL_ausgabe();
  raeume_linearH();
  R_sort();
  //printf("raeume ...\n");

  //raume_print();
  loesung_master();
  //printf("geloest ...\n");


  // test_sort();
  // validate_raeume();
  // validate_loesung();
  return 0;
}

void setStackSize(){
  const rlim_t kStackSize = 500 * 1024 * 1024;   // min stack size = 500 MB
   struct rlimit rl;
   int result;

   result = getrlimit(RLIMIT_STACK, &rl);
   if (result == 0)
   {
       if (rl.rlim_cur < kStackSize)
       {
           rl.rlim_cur = kStackSize;
           result = setrlimit(RLIMIT_STACK, &rl);
           if (result != 0)
           {
               fprintf(stderr, "setrlimit returned result = %d\n", result);
           }
       }
   }

}


int main(){setStackSize();testus();}
